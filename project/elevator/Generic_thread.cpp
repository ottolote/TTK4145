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


Generic_thread::Generic_thread() 
    : io(),
      work(io)
{
}




void Generic_thread::run() {
    io.run();
    //safe_cout << "&&&&&&&& thread reached end\n";
    std::cout << "thread reached end\n";
    return;
}







