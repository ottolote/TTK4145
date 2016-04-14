/*
 *
 * networkhandler.hpp
 *
 * */

#pragma once

#include "generic_thread.hpp"
#include "message_handler.hpp"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#define NETWORK_RECVBUFFER_LEN 40

//using namespace boost;
//using boost::asio::ip::udp;

class generic_thread;

class Network {
    public:
        Network();
        void init_thread(boost::shared_mutex &_cout_lock);
        void doStuff();
        void post(encoded_msg_t msg);
        void add_reference_to(const boost::shared_ptr<generic_thread> &TM);

    private:
        // variables
        boost::asio::io_service io;
        boost::shared_mutex * _cout_lock;
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint remote_endpoint_;
        boost::asio::ip::udp::endpoint target_endpoint_;
        boost::array<char, NETWORK_RECVBUFFER_LEN> recv_buffer_;
        boost::asio::deadline_timer timeout_timer;
        boost::shared_ptr<generic_thread> _TM;

        // functions
        void send(std::string host, boost::shared_ptr<std::string> message);
        void send(std::string host, encoded_msg_t msg);
        void send(
                boost::asio::ip::udp::endpoint target_endpoint_,
                boost::shared_ptr<std::string> message);
        void request_send(std::string host, 
                boost::shared_ptr<std::string> message);
        void start_receive();
        void handle_send(boost::shared_ptr<std::string> message,
                const boost::system::error_code& e,
                std::size_t bytes_transferred);
        void handle_receive(const boost::system::error_code& e,
                std::size_t bytes_transferred);
        void timeout(const boost::system::error_code &e);
        void refresh_timeout_timer();

}; //end of class Network
