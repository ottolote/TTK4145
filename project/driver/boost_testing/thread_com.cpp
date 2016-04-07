#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#define RECVBUFFER_LEN 40
#define MYPORT 30000
//#define TARGETPORT "30000"

// using c++11 (lambdas instead of boost::bind)
//
// ASYNCRONOUS NETWORK HANDLER
//
// FEATURES: 
// - Receives continously but can send if asked to (and not busy)
// - timeout() is run when timeout_timer expires
// - send() and post() is thread safe
// 
// TODO:
// - use remote_endpoint_ for something useful, like checking senders ip
// - Actually handle timeouts
// - Get rid of the remaining boost::binds by using lambdas (c++11 ftw)
// - Somehow improve readability
//
// Optional todo: 
// - Handle message truncation
//

using namespace boost;
using boost::asio::ip::udp;


class Network {
    public:
        Network() 
            : socket_(io, udp::endpoint(udp::v4(), MYPORT)),
              timeout_timer(io, posix_time::milliseconds(1000))
        {
            timeout_timer.async_wait([&](const boost::system::error_code& e) {
                    //this function will be run at the end of the timer
                    timeout(e);
            });

            start_receive();
        }


        void run() {
            io.run();
            std::cout << "networkthread reached end\n";
            return;
        }


        void doStuff() {
            std::cout << "handler schmandler is doing stuff\n";
            refresh_timeout_timer();
            return;
        }

        void send(std::string host, shared_ptr<std::string> message) {

            udp::resolver resolver(io);
            udp::resolver::query query(udp::v4(), host, "30000");
            udp::resolver::iterator iter = resolver.resolve(query);
            target_endpoint_ = *iter;

            socket_.async_send_to(asio::buffer(*message), target_endpoint_,
                    boost::bind(&Network::handle_send, this, message,
                        asio::placeholders::error,
                        asio::placeholders::bytes_transferred));

            refresh_timeout_timer();
        }


        void post() {
            io.post([&] { doStuff(); });
            return;
        }


    private:
        asio::io_service io;
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        udp::endpoint target_endpoint_;
        boost::array<char, RECVBUFFER_LEN> recv_buffer_;
        asio::deadline_timer timeout_timer;


        void start_receive() {
            socket_.async_receive_from(
                    asio::buffer(recv_buffer_), remote_endpoint_,
                    boost::bind(&Network::handle_receive, this,
                        asio::placeholders::error, 
                        asio::placeholders::bytes_transferred));
                    
        }

        void handle_send(shared_ptr<std::string> message,
                const boost::system::error_code& e,
                std::size_t bytes_transferred) {
            std::cout << "sent message: " << *message << std::endl;;
        }


        void handle_receive(const boost::system::error_code& e,
                std::size_t bytes_transferred) {
            std::cout << "received message: '" << std::string(recv_buffer_.begin(), recv_buffer_.begin() + bytes_transferred) << "'\n";

            // TRYING TO CHECK FOR TRUNCATION, NOT WORKING
            // std::cout << e << std::endl;
            // if (!e || e == boost::asio::error::message_size) {
            //     std::cout << "wat?\n";
            // }

            // restart receive
            start_receive();
        }

        void timeout(const system::error_code &e) {
            // don't do anything if the timeout was pushed 
            //      (aborted and refreshed by refresh_timeout_timer() )
            if (e == asio::error::operation_aborted) {return;}

            // Handle the timeout
            std::cout << "NetworkThread timed out, we should probably deal with this and tell main something is wrong\n";
            return;
        }

        
        void refresh_timeout_timer() {
            timeout_timer.cancel(); 
            timeout_timer.expires_from_now(posix_time::milliseconds(1000));
            timeout_timer.async_wait([&](const boost::system::error_code& e){ 
                    // function to be run at timeout
                    timeout(e); });
            return;
        }

}; //end of class Network


int main() {

    // Start ayncronous Networkhandler in new thread
    Network Networkhandler; // Thread object
    thread NetworkThread([&] { 
            Networkhandler.run(); });

    // create shared pointer for message to be sent (boost::shared_ptr)
    shared_ptr<std::string> message(
            new std::string("test")); 

    for (int i = 0; i<8; i++) {
        Networkhandler.send("localhost", message);
        usleep(500000);
    }

    message.reset();


    NetworkThread.join();
    return 0;
}

