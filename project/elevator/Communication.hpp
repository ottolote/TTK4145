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
//#include <map>





class timed_msg_container {
    public:
        ~timed_msg_container();
        std::map<encoded_msg_t, boost::shared_ptr<boost::asio::deadline_timer>> messages;
        void new_message(boost::asio::io_service &io, encoded_msg_t msg);
        int count;
};




class timed_status_message {
    public:
        timed_status_message(
                boost::asio::io_service &io, int timeout_in_ms, encoded_msg_t message);
        boost::asio::deadline_timer status_message_timer;
        encoded_msg_t status_message;
};



class Communication : public Network {
    public:
        Communication();
        void send_order(int order_msg, std::string target_ip);
        void send_pending_order(int order, bool clear_or_set);
        void update_internal_status(status_msg_t status_message);

        // for pure testing
        void send_status(encoded_msg_t message, std::string target_ip);

        // public because testing, move to private
        void reliable_send(encoded_msg_t encoded_message, std::string target_ip);


    private:
        std::map<std::string, boost::shared_ptr<timed_msg_container>> pending_acks;
        std::map<std::string, boost::shared_ptr<timed_status_message>> previous_status;
        boost::asio::deadline_timer is_online_timer;
        void timeout(const boost::system::error_code &e);
        void refresh_is_online_timer();


        void add_to_pending_acks(encoded_msg_t message, std::string target_ip);
        void remove_pending_ack(encoded_msg_t message, std::string target_ip);
        void refresh_ack_timer(encoded_msg_t message, std::string target_ip);
        void pending_ack_timeout(const boost::system::error_code &e,
                encoded_msg_t message,
                std::string ip);


        void receive_routine(encoded_msg_t message, std::string message_ip);
        void update_previous_status(encoded_msg_t message, std::string target_ip);
        void remove_previous_status(encoded_msg_t message, std::string target_ip);
        void print_pending_acks();
        void refresh_status_timer(encoded_msg_t message, std::string target_ip);
        void statusupdate_timeout(const boost::system::error_code &e,
                std::string ip);

}; //end of class Communication
