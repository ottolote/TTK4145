#include "Thread_handler.hpp"
#include "terminalcolors.h"

#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

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
//      cout << NH "message to be written" 
//
#define NH "[" TCOLOR_LIGHTBLUE "Threadmanager" TCOLOR_NC "] : "


Threadmanager::Threadmanager() 
    : timeout_timer(io, posix_time::milliseconds(10000))
{
    timeout_timer.async_wait([&](const boost::system::error_code& e) {
            //this function will be run at the end of the timer
            timeout(e);
    });
}


void Threadmanager::init_parent_channel( 
        boost::shared_ptr<boost::asio::io_service> parent_io) {
    this->_parent_io = parent_io;
}


void Threadmanager::run() {
    io.run();
    std::cout << "networkthread reached end\n";
    return;
}




//void Threadmanager::doStuff() {
//    std::cout << "handler schmandler is doing stuff\n";
//    refresh_timeout_timer();
//    return;
//}




//void Threadmanager::post() {
//    io.post([&] { doStuff(); });
//    return;
//}



void Threadmanager::timeout(const system::error_code &e) {
    // don't do anything if the timeout was pushed 
    //      (aborted and refreshed by refresh_timeout_timer() )
    if (e == asio::error::operation_aborted) {return;}

    // Handle the timeout
    std::cout << NH "Threadmanager timed out, we should probably deal with this and tell main something is wrong\n";
    return;
}




void Threadmanager::refresh_timeout_timer() {
    timeout_timer.cancel(); 
    timeout_timer.expires_from_now(posix_time::milliseconds(1000));
    timeout_timer.async_wait([&](const boost::system::error_code& e){ 
            // function to be run at timeout
            timeout(e); });
    return;
}

