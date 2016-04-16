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
#define RESEND_TIMEOUT 500

// using c++11 (lambdas instead of boost::bind)
//
// COMMUNICATION MODULE
//
// FEATURES: 
// 
// TODO:
// - Actually handle timeouts
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



Communication::Communication() 
    : Network(),
      is_online_timer(io, boost::posix_time::milliseconds(NO_CONNECTION_TIMEOUT_MS))
{
    is_online_timer.async_wait([&](const boost::system::error_code& e) {
            //this function will be run when the timer is triggered
            timeout(e);
    });

}




// Pendig acks has to be map of class to call constructor of timers
//
// create timed_message_container class with constructor timed_message_container(boost::asio::io_service *io, encoded_msg_t msg) and call public (or friended) variable message_timer.async_wait([&](const boost::system::error_code& e) {
//      some_timeout_func_from_Communication.
void Communication::reliable_send(encoded_msg_t encoded_message, std::string target_ip) {
    encoded_message = Message_handler::add_checksum( encoded_message );

    //pending_acks[target_ip].message_timer.;
    //pending_acks[target_ip].message = encoded_message;

    send(target_ip,encoded_message);
}





void Communication::receive_routine(encoded_msg_t encoded_message, std::string message_ip ) {

    std::cout << PROMPT << "message from " <<  message_ip << " : '"
        << encoded_message << "'" << std::endl;


    // filter out invalid checksums
    if (Message_handler::checksum(encoded_message) != Message_handler::read_checksum(encoded_message)) {
            std::cout << PROMPT "message has invalid checksum\n";
    } 
    
    
    else {

    refresh_is_online_timer();

        if (Message_handler::is_ack_message(encoded_message)) {
            std::cout << PROMPT 
                "received ack for message from " << message_ip << std::endl;
            //stop resending
        } else {

            if (Message_handler::read_message_type(encoded_message) == STATUS_MESSAGE_TYPE) {
                status_msg_t status_message = Message_handler::decode_status_message(encoded_message);
                //control->deliver_status(status_message);
                
                //remove compilerwarning:
                status_message.dir = DIR_STOP;

            } else {
                order_msg_t order_message; Message_handler::decode_order_message(encoded_message);
                //control->deliver_order(order_message);
                
                //remove compilerwarning
                order_message.order = FIRST_UP; 
            }
        }
    }

    


}




void Communication::timeout(const system::error_code &e) {
    // don't do anything if the timer was cancelled
    //      (aborted and refreshed)
    if (e == asio::error::operation_aborted) {return;}
    // Handle the timeout
    std::cout << PROMPT "Communication thread timed out, we should probably deal with this and tell main something is wrong\n";
    return;
}



void Communication::refresh_is_online_timer() {
    is_online_timer.cancel(); 
    is_online_timer.expires_from_now(posix_time::milliseconds(NO_CONNECTION_TIMEOUT_MS));
    is_online_timer.async_wait([&](const boost::system::error_code& e){ 
            // function to be run at timeout
            timeout(e); });
    return;
}

