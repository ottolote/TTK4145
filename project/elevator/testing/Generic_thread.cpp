/*
 *
 * Generic_thread.hpp
 *
 * */

#include "Generic_thread.hpp"
#include "terminalcolors.h"

#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#define TIMEOUT_TIMER_DURATION_MS 3000

// using c++14 
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
#define PROMPT "[" TCOLOR_LIGHTGREEN "Generic_thread" TCOLOR_NC "] : "


Generic_thread::Generic_thread(int timeout_in_ms) 
    : io(),
      work(io),
      timeout_timer(io, boost::posix_time::milliseconds(timeout_in_ms)),
      _timer_duration_ms(timeout_in_ms)
{
    timeout_timer.async_wait([&](const boost::system::error_code& e) {
            //this function will be run at the end of the timer
            timeout(e);
    });
}




void Generic_thread::run() {
    io.run();
    //safe_cout << "&&&&&&&& thread reached end\n";
    std::cout << "thread reached end\n";
    return;
}






void Generic_thread::timeout(const system::error_code &e) {
    // don't do anything if the timeout was pushed 
    //      (aborted and refreshed by refresh_timeout_timer() )
    if (e == asio::error::operation_aborted) {return;}

    // Handle the timeout
    std::cout << "Generic_thread timed out, we should probably deal with this and tell main something is wrong\n";
    return;
}




void Generic_thread::refresh_timeout_timer() {
    timeout_timer.cancel(); 
    timeout_timer.expires_from_now(posix_time::milliseconds(_timer_duration_ms));
    timeout_timer.async_wait([&](const boost::system::error_code& e){ 
            // function to be run at timeout
            timeout(e); });
    return;
}

