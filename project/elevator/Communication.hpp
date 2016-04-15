/*
 *
 * Communication.hpp
 *
 * */

#pragma once

//#include "Generic_thread.hpp"
#include "message_handler.hpp"
//#include "Generic_thread.hpp"
#include "Network.hpp"
#include "Control.hpp"

#include <boost/asio.hpp>




class Communication : public Network {
    public:
        Communication();
        void reliable_send(int test);
        void receive_routine(encoded_msg_t message, std::string message_ip);

    private:
//        void handle_receive(
//                const boost::system::error_code& e,
//                std::size_t bytes_transferred);
        void timeout(const boost::system::error_code &e);

}; //end of class Communication
