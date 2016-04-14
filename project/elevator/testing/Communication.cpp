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
// ASYNCRONOUS NETWORK HANDLER
//
// FEATURES: 
// - Receives continously but can send if asked to (and not busy)
// - timeout() is run when timeout_timer expires
// - send() and post() is thread safe
// 
// TODO:
// - use remote_endpoint_ for something useful, like checking senders ip
// - Actually handle timeouts
// - Get rid of the remaining boost::binds by using lambdas (c++11 ftw)
// - Somehow improve readability
//
// Optional todo: 
// - Handle message truncation
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



//void Communication::add_reference_to( const boost::shared_ptr<generic_thread> &TM) {
//    this->_TM = TM;
//}






//void Communication::doStuff() {
//    std::cout << "handler schmandler is doing stuff\n";
//    refresh_timeout_timer();
//    return;
//}



//void Communication::send(std::string host, shared_ptr<std::string> message) {
//
//    udp::resolver resolver(io);
//    udp::resolver::query query(udp::v4(), host, "30000");
//    udp::resolver::iterator iter = resolver.resolve(query);
//    target_endpoint_ = *iter;
//
//    socket_.async_send_to(asio::buffer(*message), target_endpoint_,
//            boost::bind(&Communication::handle_send, this, message,
//                asio::placeholders::error,
//                asio::placeholders::bytes_transferred));
//
//    refresh_timeout_timer();
//}
//
//
//
//
//void Communication::send(std::string host, encoded_msg_t msg) {
//
//    udp::resolver resolver(io);
//    udp::resolver::query query(udp::v4(), host, std::to_string(MYPORT));
//    udp::resolver::iterator iter = resolver.resolve(query);
//    target_endpoint_ = *iter;
//
//    shared_ptr<std::string> message(
//            new std::string( { (char)(msg >> 8), (char)msg } ));
//
//    socket_.async_send_to(asio::buffer(*message), target_endpoint_,
//            boost::bind(&Communication::handle_send, this, message,
//                asio::placeholders::error,
//                asio::placeholders::bytes_transferred));
//
//    refresh_timeout_timer();
//}
//
//
//
//
//
//
//
//
//void Communication::send(udp::endpoint target_endpoint_, shared_ptr<std::string> message) {
//
//    socket_.async_send_to(asio::buffer(*message), target_endpoint_,
//            boost::bind(&Communication::handle_send, this, message,
//                asio::placeholders::error,
//                asio::placeholders::bytes_transferred));
//
//    refresh_timeout_timer();
//}
//
//
//
//
//
//void Communication::doStuff() {
//    std::cout << PROMPT "doing stuff";
//    //_TM->post();
//}
//
//
//
//void Communication::post(encoded_msg_t msg) {
//    io.post([=] { send("127.0.0.1", msg); });
//    //io.post([&] { doStuff(); });
//    return;
//}
//
//
//
//void Communication::start_receive() {
//    socket_.async_receive_from(
//            asio::buffer(recv_buffer_), remote_endpoint_,
//            boost::bind(&Communication::handle_receive, this,
//                asio::placeholders::error, 
//                asio::placeholders::bytes_transferred));
//}
//
//
//
//
//void Communication::handle_send(shared_ptr<std::string> message,
//        const boost::system::error_code& e,
//        std::size_t bytes_transferred) {
//    if(e == asio::error::host_not_found) { exit(1) ; }
//
//    std::cout << PROMPT "sent message: " << *message << " - " 
//            << bytes_transferred << " bytes\n";
//}
//
//
//
//
//void Communication::handle_receive(const boost::system::error_code& e,
//        std::size_t bytes_transferred) {
//    if(e == asio::error::network_unreachable) { exit(1); }
//    std::cout << PROMPT "received message: '" << std::string(recv_buffer_.begin(), recv_buffer_.begin() + bytes_transferred) << "' - " << bytes_transferred << " bytes\n";
//
//    start_receive();
//}
//



void Communication::timeout(const system::error_code &e) {
    // don't do anything if the timeout was pushed 
    //      (aborted and refreshed by refresh_timeout_timer() )
    if (e == asio::error::operation_aborted) {return;}
    // Handle the timeout
    std::cout << PROMPT "Communication thread timed out, we should probably deal with this and tell main something is wrong\n";
    return;
}



