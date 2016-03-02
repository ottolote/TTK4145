#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

using namespace boost;

class Network {
    public:
        Network() 
            : timeout_timer(io, posix_time::milliseconds(1000))
        {
            timeout_timer.async_wait(bind(&Network::timeout, this, 
                        asio::placeholders::error));
        }

        void run() {
            io.run();
            std::cout << "networkthread reached end\n";
        }

        void refresh_timeout_timer() {
            timeout_timer.cancel(); 
            timeout_timer.expires_from_now(posix_time::milliseconds(1000));
            timeout_timer.async_wait(bind(&Network::timeout,this,
                        asio::placeholders::error));
        }

        void doStuff() {
            //if(e) { std::cout << "wtf\n"; }
            std::cout << "handler schmandler is doing stuff\n";
            refresh_timeout_timer();
        }

        void post() {
            io.post(bind(&Network::doStuff, this));
        }

    private:
        asio::io_service io;
        asio::deadline_timer timeout_timer;

        void timeout(const system::error_code &e) {
            // don't do anything if the timeout was aborted
            if (e == asio::error::operation_aborted) {return;}

            std::cout << "NetworkThread timed out, we should probably tell main something is wrong\n";
        }

}; //end of class Network


int main() {

    Network Networkhandler;
    thread NetworkThread(bind( &Network::run, &Networkhandler));

    for (int i = 0; i<8; i++) {
        Networkhandler.post();
        usleep(500000);
    }

    NetworkThread.join();
    return 0;
}

