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

#define TIMEOUT_TIMER_DURATION_MS 1000

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
    : Network()
{
    timeout_timer.async_wait([&](const boost::system::error_code& e) {
            //this function will be run when the timer is triggered
            timeout(e);
    });

}





void Communication::reliable_send(int test) {
    // try until successfully acked
    send("127.0.0.1",test);
}





void Communication::receive_routine(encoded_msg_t encoded_message, std::string message_ip ) {
    std::cout << PROMPT << "message from " <<  message_ip << " containing '"
        << encoded_message << "' is being handled" << std::endl;


    
    if (Message_handler::read_message_type(encoded_message) == STATUS_MESSAGE_TYPE) {
        status_msg_t status_message = Message_handler::decode_status_message(encoded_message);
        control->deliver_status(status_message);
    } else {
        order_msg_t order_message; Message_handler::decode_order_message(encoded_message);
        control->deliver_order(order_message);
    }

    


}




void Communication::timeout(const system::error_code &e) {
    // don't do anything if the timeout was pushed 
    //      (aborted and refreshed by refresh_timeout_timer() )
    if (e == asio::error::operation_aborted) {return;}
    // Handle the timeout
    std::cout << PROMPT "Communication thread timed out, we should probably deal with this and tell main something is wrong\n";
    return;
}



