#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

class Network {
    public:
//         Network() 
//             : timeout_timer(io, boost::posix_time::seconds(1)) 
//         {
//                     timeout_timer.async_wait([=] { this->doStuff();});
// 
//         }


        void run() {
            io.run();
        }

        void doStuff() {
                std::cout << "handler schmandler\n";
        }

        void post() {
                io.post([=] { Network::doStuff();});
        }
    private:
            boost::asio::io_service io;

//            boost::asio::deadline_timer timeout_timer;

}; //end of class Network



int main() {

    Network Networkhandler;
    Networkhandler.post();

    boost::thread t([=] { Network::run();})
        return 0;

}

