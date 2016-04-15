/*
 *
 * Generic_thread.hpp
 *
 * */

#pragma once

//#include "safeprint.hpp"
#include <boost/asio.hpp>


class Generic_thread {
    public:
        Generic_thread(int timer_duration_ms);
//        static safeprint safe_cout;
        void post();
        void run();

    protected:
        boost::asio::io_service io;
        boost::asio::io_service::work work;
        boost::asio::deadline_timer timeout_timer;
        virtual void timeout(const boost::system::error_code &e);
        void refresh_timeout_timer();

    private:
        int _timer_duration_ms;



}; //end of class Network 
