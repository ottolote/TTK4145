/*
 *
 * Communication.hpp
 *
 * */

#pragma once

//#include "generic_thread.hpp"
#include "message_handler.hpp"
#include "generic_thread.hpp"
#include "Network.hpp"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#define NETWORK_RECVBUFFER_LEN 40

//using namespace boost;
//using boost::asio::ip::udp;


class Communication : public Network {
    public:
        Communication();
//        void doStuff();
//        void post(encoded_msg_t msg);
        void reliable_send(int test);

    private:
        // variables
//        boost::asio::ip::udp::socket socket_;
//        boost::asio::ip::udp::endpoint remote_endpoint_;
//        boost::asio::ip::udp::endpoint target_endpoint_;
//        boost::array<char, NETWORK_RECVBUFFER_LEN> recv_buffer_;
//        void safeprint(std::string print);

        // functions
//        void send(std::string host, boost::shared_ptr<std::string> message);
//        void send(std::string host, encoded_msg_t msg);
//        void send(
//                boost::asio::ip::udp::endpoint target_endpoint_,
//                boost::shared_ptr<std::string> message);
//        void request_send(std::string host, 
//                boost::shared_ptr<std::string> message);
//        void start_receive();
//        void handle_send(boost::shared_ptr<std::string> message,
//                const boost::system::error_code& e,
//                std::size_t bytes_transferred);
        void handle_receive(const boost::system::error_code& e,
                std::size_t bytes_transferred);
        void timeout(const boost::system::error_code &e);
        //void refresh_timeout_timer();

}; //end of class Communication
