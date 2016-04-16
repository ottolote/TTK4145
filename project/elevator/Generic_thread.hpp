/*
 *
 * Generic_thread.hpp
 *
 * */

#pragma once

#include <boost/asio.hpp>

class Communication;
class Control;
class Hardware;


class Generic_thread {
    public:
        Generic_thread();
//        static safeprint safe_cout;
        void post();
        void run();

    protected:
        boost::asio::io_service io;
        boost::asio::io_service::work work;
//        boost::asio::deadline_timer timeout_timer;

        boost::shared_ptr<Communication> communication;
        boost::shared_ptr<Control> control;
        boost::shared_ptr<Hardware> hardware;




}; //end of class Network 
