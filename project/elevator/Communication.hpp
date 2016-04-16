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


class timed_msg_container {
    boost::asio::deadline_timer message_timer;
    encoded_msg_t message;
    int count;
};



class Communication : public Network {
    public:
        Communication();
        void reliable_send(encoded_msg_t encoded_message, std::string target_ip);
        void receive_routine(encoded_msg_t message, std::string message_ip);

    private:
        std::map<std::string, timed_msg_container> pending_acks;
        boost::asio::deadline_timer is_online_timer;
        void timeout(const boost::system::error_code &e);
        void refresh_is_online_timer();
        void add_to_pending_sends();

}; //end of class Communication
