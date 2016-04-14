/*
 *
 * Network.cpp
 *
 * */

#include "Network.hpp"
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
#define PROMPT "[" TCOLOR_LIGHTBLUE "Network" TCOLOR_NC "] : "



Network::Network() 
    : generic_thread(TIMEOUT_TIMER_DURATION_MS),
      socket_(io, udp::endpoint(udp::v4(), MYPORT))
      //timeout_timer(io, posix_time::milliseconds(TIMEOUT_TIMER_DURATION_MS))
{
    timeout_timer.async_wait([&](const boost::system::error_code& e) {
            //this function will be run when the timer is triggered
            timeout(e);
    });

    start_receive();
}





//void Network::add_reference_to( const boost::shared_ptr<generic_thread> &TM) {
//    this->_TM = TM;
//}






//void Network::doStuff() {
//    std::cout << "handler schmandler is doing stuff\n";
//    refresh_timeout_timer();
//    return;
//}



void Network::send(std::string host, shared_ptr<std::string> message) {

    udp::resolver resolver(io);
    udp::resolver::query query(udp::v4(), host, "30000");
    udp::resolver::iterator iter = resolver.resolve(query);
    target_endpoint_ = *iter;

    socket_.async_send_to(asio::buffer(*message), target_endpoint_,
            boost::bind(&Network::handle_send, this, message,
                asio::placeholders::error,
                asio::placeholders::bytes_transferred));

    refresh_timeout_timer();
}




void Network::send(std::string host, encoded_msg_t msg) {

    udp::resolver resolver(io);
    udp::resolver::query query(udp::v4(), host, std::to_string(MYPORT));
    udp::resolver::iterator iter = resolver.resolve(query);
    target_endpoint_ = *iter;

    shared_ptr<std::string> message(
            new std::string( { (char)(msg >> 8), (char)msg } ));

    socket_.async_send_to(asio::buffer(*message), target_endpoint_,
            boost::bind(&Network::handle_send, this, message,
                asio::placeholders::error,
                asio::placeholders::bytes_transferred));

    refresh_timeout_timer();
}




//void network::send(std::string host, int message) {
//
//    udp::resolver resolver(io);
//    udp::resolver::query query(udp::v4(), host, "30000");
//    udp::resolver::iterator iter = resolver.resolve(query);
//    target_endpoint_ = *iter;
//
//    boost::shared_ptr<std::string> str(
//            new std::string("a"));
//
//    socket_.async_send_to(asio::buffer(*str), target_endpoint_,
//            boost::bind(&network::handle_send, this, message,
//                asio::placeholders::error,
//                asio::placeholders::bytes_transferred));
//
//    refresh_timeout_timer();
//}




void Network::send(udp::endpoint target_endpoint_, shared_ptr<std::string> message) {

    socket_.async_send_to(asio::buffer(*message), target_endpoint_,
            boost::bind(&Network::handle_send, this, message,
                asio::placeholders::error,
                asio::placeholders::bytes_transferred));

    refresh_timeout_timer();
}





void Network::doStuff() {
    std::cout << PROMPT "Network Wallaballa\n";
    //_TM->post();
}



//void Network::post(encoded_msg_t msg) {
//    io.post([=] { send("127.0.0.1", msg); });
//    //io.post([&] { doStuff(); });
//    return;
//}



void Network::start_receive() {
    socket_.async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&Network::handle_receive, this,
                asio::placeholders::error, 
                asio::placeholders::bytes_transferred));
}




void Network::handle_send(shared_ptr<std::string> message,
        const boost::system::error_code& e,
        std::size_t bytes_transferred) {
    if(e == asio::error::host_not_found) { exit(1) ; }

    std::cout << PROMPT "sent message: " << *message << " - " 
            << bytes_transferred << " bytes\n";
}




void Network::handle_receive(const boost::system::error_code& e,
        std::size_t bytes_transferred) {
    if(e == asio::error::network_unreachable) { exit(1); }
    std::cout << PROMPT "received message: '" << std::string(recv_buffer_.begin(), recv_buffer_.begin() + bytes_transferred) << "' - " << bytes_transferred << " bytes\n";

    start_receive();
}




//void Network::timeout(const system::error_code &e) {
//    // don't do anything if the timeout was pushed 
//    //      (aborted and refreshed by refresh_timeout_timer() )
//    if (e == asio::error::operation_aborted) {return;}
//    // Handle the timeout
//    std::cout << PROMPT "NetworkThread timed out, we should probably deal with this and tell main something is wrong\n";
//    return;
//}


