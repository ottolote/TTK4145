#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#define NETWORK_RECVBUFFER_LEN 40

using namespace boost;
using boost::asio::ip::udp;

class Network {
    private:
        asio::io_service io;
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        udp::endpoint target_endpoint_;
        boost::array<char, NETWORK_RECVBUFFER_LEN> recv_buffer_;
        asio::deadline_timer timeout_timer;


    public:
        void run();
        void doStuff();
        void send(std::string host, shared_ptr<std::string> message);
        void post();
        void start_receive();
        void handle_send(shared_ptr<std::string> message,
                const boost::system::error_code& e,
                std::size_t bytes_transferred);
        void handle_receive(const boost::system::error_code& e,
                std::size_t bytes_transferred);
        void timeout(const system::error_code &e);
        void refresh_timeout_timer();

}; //end of class Network
