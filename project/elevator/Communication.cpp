/*
 *
 * Communication.cpp
 *
 * */

#include "Communication.hpp"
#include "terminalcolors.h"

#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#define RECVBUFFER_LEN 40
#define MYPORT 30000

#define NO_CONNECTION_TIMEOUT_MS 1000
#define RESEND_TIMEOUT_MS 500
#define MAX_RESEND_ATTEMPTS 15
#define STATUS_UPDATE_TIMEOUT_MS 2000

// using c++11 (lambdas instead of boost::bind)
//
// COMMUNICATION MODULE
//
// FEATURES: 
// 
// TODO:
// - Handle acks in receive_routine
// - add map<ip, timed_msg_container*> to check if they have timed out or changed status
// - send status updates regularly, find a way
// - clear order encoding
//
//

using namespace boost;
using boost::asio::ip::udp;


// This is for colorized tidy output
// usage:
//
//      cout << PROMPT "message to be written" 
//
#define PROMPT "[" TCOLOR_LIGHTBLUE "Communication" TCOLOR_NC "] : "




//timed_msg_container::timed_msg_container() 
//     {
////
//}


// Destructor makes sure to cancel all timers
timed_msg_container::~timed_msg_container() {
    for (std::map<encoded_msg_t, boost::shared_ptr<boost::asio::deadline_timer>>::iterator it = messages.begin(); it != messages.end(); it++) {
       it->second->cancel(); 
    }
}


void timed_msg_container::new_message(boost::asio::io_service &io, encoded_msg_t msg) {

    // return if message is already pending
    if (messages.count(msg) > 0)  {
        return;
    }
    boost::shared_ptr<boost::asio::deadline_timer> timer(
            new boost::asio::deadline_timer(
                io, boost::posix_time::milliseconds(RESEND_TIMEOUT_MS)));

    messages[msg] = timer;


}





timed_status_message::timed_status_message(
        boost::asio::io_service &io, int status_timeout, encoded_msg_t message) 
    : status_message_timer(io, boost::posix_time::milliseconds(status_timeout)),
      status_message(message)
{

}


Communication::Communication() 
    : Network(),
      is_online_timer(io, boost::posix_time::milliseconds(NO_CONNECTION_TIMEOUT_MS))
{
    is_online_timer.async_wait([&](const boost::system::error_code& e) {
            //this function will be run when the timer is triggered
            timeout(e);
    });

}








void Communication::update_previous_status(encoded_msg_t message, std::string ip) {


    // Add another elevator to map of pending acks
    if(previous_status.count(ip) == 0) {
        std::cout << PROMPT "Adding new elevator " << ip << std::endl;
        boost::shared_ptr<timed_status_message> new_elevator(
                new timed_status_message(io, STATUS_UPDATE_TIMEOUT_MS, message) );
        previous_status[ip] = new_elevator;
    }

    // Add new pending message to said elevator
    std::cout << PROMPT "Updating previous status for target " << ip 
        << ": " << message << std::endl;
    previous_status[ip]->status_message_timer.async_wait(
            [this, ip](const boost::system::error_code &e){
                statusupdate_timeout(e, ip);});
    std::cout << PROMPT " new status " << message << " to status_message_timeout\n";
}






void Communication::add_to_pending_acks(encoded_msg_t message, std::string target_ip) {


    // Add another elevator to map of pending acks
    if(pending_acks.count(target_ip) == 0) {
        std::cout << PROMPT "Adding new target " << target_ip << std::endl;
        boost::shared_ptr<timed_msg_container> new_target(
                new timed_msg_container() );
        pending_acks[target_ip] = new_target;
    }

    // Add new pending message to said elevator
    std::cout << PROMPT "Adding pending ack for target " << target_ip 
        << " for message " << message << std::endl;
    pending_acks[target_ip]->new_message(io, message);
    static std::string *test(
            new std::string(target_ip));
    std::cout << PROMPT " pushing message " << message << " to pending_ack_timeout\n";
    pending_acks[target_ip]->messages[message]->async_wait(
            [message, this](const boost::system::error_code &e) {
                pending_ack_timeout(e, message, *test);});
}







void Communication::print_pending_acks() {
    std::cout << PROMPT "\n\n----Pending acks----\n";
    for (std::map<std::string, boost::shared_ptr<timed_msg_container>>::iterator ip = pending_acks.begin(); ip != pending_acks.end(); ip++) {
        std::cout << "IP: " << ip->first << " - messages: ";
        for (std::map<encoded_msg_t, boost::shared_ptr<boost::asio::deadline_timer>>::iterator msg = ip->second->messages.begin(); msg != ip->second->messages.end(); msg++) {
            std::cout << "\t" << msg->first;
        }
        std::cout << std::endl;
    }
    std::cout << "-----end of pending acks----\n\n";
}




// Pendig acks has to be map of class to call constructor of timers
//
// create timed_message_container class with constructor timed_message_container(boost::asio::io_service *io, encoded_msg_t msg) and call public (or friended) variable message_timer.async_wait([&](const boost::system::error_code& e) {
//      some_timeout_func_from_Communication.
void Communication::reliable_send(encoded_msg_t message, std::string target_ip) {

    if (Message_handler::is_ack_message(message)) {
        std::cout << PROMPT "reliable_send: message already contains an ack, refusing to send\n";
    } else {

        // Add checksum to message
        message = Message_handler::add_checksum(message);

        print_pending_acks();
        add_to_pending_acks(message, target_ip);
        print_pending_acks();

        send(target_ip,message);
    }
}



void Communication::send_status(encoded_msg_t message, std::string target_ip) {
    send(target_ip, message);
}





void Communication::remove_pending_ack(encoded_msg_t message, std::string target_ip) {
    if (pending_acks.count(target_ip) == 0) {
        std::cout << PROMPT << ERROR "Tried removing pending ack from deleted target "
            << target_ip << std::endl;
        return;
    }
    if (pending_acks[target_ip]->messages.find(message) == pending_acks[target_ip]->messages.end()) {
        std::cout << PROMPT 
            << ERROR "Tried removing pending ack from deleted message to ip:"
            << target_ip << std::endl;
        std::cout << pending_acks[target_ip]->messages.begin()->first << std::endl;
        std::cout << message << std::endl;

        print_pending_acks();
        return;
    }

    std::cout << PROMPT "removing ack for message " << message 
        << " from " << target_ip << std::endl;

    pending_acks[target_ip]->messages[message]->cancel();
    pending_acks[target_ip]->messages.erase(message);

    if (pending_acks[target_ip]->messages.size() == 0) {
        pending_acks.erase(target_ip);
    }

}










void Communication::receive_routine(encoded_msg_t encoded_message, std::string message_ip ) {

    print_pending_acks();

    std::cout << PROMPT << "message from " <<  message_ip << " : '"
        << encoded_message << "'" << std::endl;

    std::cout<< PROMPT "message checksum: " << Message_handler::checksum(encoded_message) << std::endl;
    std::cout<< PROMPT "checksum should be: " << Message_handler::read_checksum(encoded_message) << std::endl;


    // filter out invalid checksums
//    if (Message_handler::checksum(encoded_message) != Message_handler::read_checksum(encoded_message)) {
//            std::cout << PROMPT "message has invalid checksum\n";
//            return;
//    } 
    
    
//    else {

    refresh_is_online_timer();




    if (Message_handler::read_message_type(encoded_message) == STATUS_MESSAGE_TYPE) {


        std::cout << PROMPT "received status message from " << message_ip << std::endl;

        update_previous_status(encoded_message, message_ip);
        
        status_msg_t status_message = Message_handler::decode_status_message(encoded_message);
        //control->deliver_status(status_message);
        
        //remove compilerwarning:
        status_message.dir = DIR_STOP;



    } else if (Message_handler::is_ack_message(encoded_message)) {
        std::cout << PROMPT 
            "received ack for message from " << message_ip << std::endl;
        remove_pending_ack(
                Message_handler::remove_ack_from_message(encoded_message), message_ip);
        print_pending_acks();
        return;
    } else {

        send(message_ip, Message_handler::add_ack_to_message(encoded_message));

        std::cout << PROMPT "received order from " << message_ip << std::endl;

        order_msg_t order_message; Message_handler::decode_order_message(encoded_message);
        //control->deliver_order(order_message);
        
        //remove compilerwarning
        order_message.order = FIRST_UP; 
    }

    print_pending_acks();

}






// encoded_t_msg might not be needed in this function
void Communication::statusupdate_timeout(
        const boost::system::error_code &e, std::string ip) {
    // don't do anything if the timer was cancelled
    //      (aborted and refreshed)
    if (e == asio::error::operation_aborted) {return;}

    if (previous_status.count(ip) == 0) {
        std::cout << PROMPT << ERROR "elevator " << ip 
            << " timed out but was not even in our list" << std::endl;;
        return;
    }


    std::cout << PROMPT << ERROR "elevator " << ip 
            << " timed out" << std::endl;;
    previous_status.erase(ip);

    return;
}












// encoded_t_msg might not be needed in this function
void Communication::pending_ack_timeout(
        const boost::system::error_code &e,  encoded_msg_t message, std::string target_ip) {
    // don't do anything if the timer was cancelled
    //      (aborted and refreshed)
    if (e == asio::error::operation_aborted) {return;}

    if (pending_acks.count(target_ip) == 0) {
        std::cout << PROMPT << ERROR "reached pending ack timeout for elevator not pending ack: " << target_ip << std::endl;;

        print_pending_acks();
        return;
    }

    // Handle the timeout
    std::cout << PROMPT "didn't receive ack from " << target_ip;

    if (pending_acks[target_ip]->count < MAX_RESEND_ATTEMPTS ) {

        pending_acks[target_ip]->count++;


        std::cout << " resending " << message << std::endl;

        refresh_ack_timer(message, target_ip);
    }  else {

        // delete all messages heading to this ip
        pending_acks.erase(target_ip);

        //control->report_useless_elevator(message.target_ip);
        std::cout << " reporting as useless elevator\n";

    }

    print_pending_acks();

    return;
}





void Communication::timeout(const system::error_code &e) {
    // don't do anything if the timer was cancelled
    //      (aborted and refreshed)
    if (e == asio::error::operation_aborted) {return;}
    // Handle the timeout
    std::cout << PROMPT "Communication thread timed out, we should probably deal with this and tell main something is wrong\n";
    return;
}





void Communication::refresh_ack_timer(encoded_msg_t message, std::string target_ip) {
    if (pending_acks.count(target_ip) == 0) {
        std::cout << PROMPT << ERROR "attempted refresh of timer for deleted target\n";
        return;
    }
    if (pending_acks[target_ip]->messages.count(message) == 0) {
        std::cout << "first message: " << pending_acks[target_ip]->messages[message] << std::endl;
        std::cout << PROMPT << ERROR "attempted refresh of timer for deleted message\n";
        return;
    }

    pending_acks[target_ip]->messages[message]->cancel();
    pending_acks[target_ip]->messages[message]->expires_from_now(posix_time::milliseconds(RESEND_TIMEOUT_MS));
    pending_acks[target_ip]->messages[message]->async_wait(
            [message, this, target_ip](const boost::system::error_code& e) {
                pending_ack_timeout(e, message, target_ip);});
}






void Communication::refresh_status_timer(encoded_msg_t message, std::string ip) {
    if (previous_status.count(ip) == 0) {
        std::cout << PROMPT << ERROR "attempted refresh of timer for deleted elevator\n";
        return;
    }

    std::cout << PROMPT "new update from " << ip << " :" << message << std::endl;

    previous_status[ip]->status_message_timer.cancel();
    previous_status[ip]->status_message_timer.expires_from_now(posix_time::milliseconds(STATUS_UPDATE_TIMEOUT_MS));
    previous_status[ip]->status_message_timer.async_wait(
            [ this, ip](const boost::system::error_code& e) {
                statusupdate_timeout(e, ip);});
}






void Communication::refresh_is_online_timer() {
    is_online_timer.cancel(); 
    is_online_timer.expires_from_now(posix_time::milliseconds(NO_CONNECTION_TIMEOUT_MS));
    is_online_timer.async_wait([&](const boost::system::error_code& e){ 
            // function to be run at timeout
            timeout(e); });
    return;
}




void Communication::update_internal_status(status_msg_t status_message) {
    std::cout << PROMPT " asked by control to update status, not implemented" 
        << status_message.dir << std::endl;
            
}

void Communication::send_pending_order(int order, bool clear_or_set) {
    std::cout << PROMPT "asked to send pending order to broadcast pending orders, not implemented " << order << clear_or_set << std::endl;
}

void Communication::send_order(int order_message, std::string target_ip) {
    return;
}
