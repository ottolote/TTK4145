#pragma once

#include "networkhandler.hpp"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#define NETWORK_RECVBUFFER_LEN 40

//using namespace boost;
//using boost::asio::ip::udp;

class Network;

class Threadmanager {
    private:
        boost::asio::io_service io;
        boost::asio::deadline_timer timeout_timer;
        boost::shared_ptr<Network> _NM;


    public:
        Threadmanager();
        void init_parent_channel(
                boost::shared_ptr<Network> _NM);
        void start();
        void doStuff();
        void post();
        void run();
        void timeout(const boost::system::error_code &e);
        void refresh_timeout_timer();

}; //end of class Network
