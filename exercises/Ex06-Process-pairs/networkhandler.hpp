#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#define NETWORK_RECVBUFFER_LEN 40

//using namespace boost;
//using boost::asio::ip::udp;

class Network {
    private:
        boost::asio::io_service io;
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint remote_endpoint_;
        boost::asio::ip::udp::endpoint target_endpoint_;
        boost::array<char, NETWORK_RECVBUFFER_LEN> recv_buffer_;
        boost::asio::deadline_timer timeout_timer;
        boost::asio::io_service *parent_io;


    public:
        Network();
        void run();
        void send(std::string host, boost::shared_ptr<std::string> message);
        void start_receive();
        void handle_send(boost::shared_ptr<std::string> message,
                const boost::system::error_code& e,
                std::size_t bytes_transferred);
        void handle_receive(const boost::system::error_code& e,
                std::size_t bytes_transferred);
        void timeout(const boost::system::error_code &e);
        void refresh_timeout_timer();

}; //end of class Network
