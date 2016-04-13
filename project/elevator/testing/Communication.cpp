/*
 *
 * Communication.hpp
 *
 * */

#include "networkhandler.hpp"
#include "terminalcolors.h"

#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#define TIMEOUT_TIMER_DURATION_MS 3000

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


// This is for colorized tidy output
// usage:
//
//      cout << PROMPT "message to be written" 
//
#define PROMPT "[" TCOLOR_LIGHTGREEN "Communication" TCOLOR_NC " ] : "


Communication::Communication() 
    : timeout_timer(io, posix_time::milliseconds(TIMEOUT_TIMER_DURATION_MS))
{
    timeout_timer.async_wait([&](const boost::system::error_code& e) {
            //this function will be run at the end of the timer
            timeout(e);
    });
}


void Communication::add_reference_to( const boost::shared_ptr<Network> network) {
    this->_NM = network;
}


void Communication::run() {
    io.run();
    std::cout << PROMPT "threadmanager thread reached end\n";
    return;
}






void Communication::doStuff() {
    std::cout << PROMPT "wallaballa\n";
    _NM->post(25956);
    refresh_timeout_timer();
}


void Communication::post() {
    io.post([&] { doStuff(); });
    return;
}


//void Communication::doStuff() {
//    std::cout << "handler schmandler is doing stuff\n";
//    refresh_timeout_timer();
//    return;
//}




//void Communication::post() {
//    io.post([&] { doStuff(); });
//    return;
//}



void Communication::timeout(const system::error_code &e) {
    // don't do anything if the timeout was pushed 
    //      (aborted and refreshed by refresh_timeout_timer() )
    if (e == asio::error::operation_aborted) {return;}

    // Handle the timeout
    std::cout << PROMPT "Communication timed out, we should probably deal with this and tell main something is wrong\n";
    return;
}




void Communication::refresh_timeout_timer() {
    timeout_timer.cancel(); 
    timeout_timer.expires_from_now(posix_time::milliseconds(TIMEOUT_TIMER_DURATION_MS));
    timeout_timer.async_wait([&](const boost::system::error_code& e){ 
            // function to be run at timeout
            timeout(e); });
    return;
}

