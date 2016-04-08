#include "networkhandler.hpp"

#include <iostream>
#include <unistd.h>

using namespace boost;
using boost::asio::ip::udp;

int main() {

    // Start ayncronous Networkhandler in new thread
    Network Networkhandler; // Thread object
    thread NetworkThread([&] {
        Networkhandler.run();   // Thread function
    });


    // create shared pointer for message to be sent (boost::shared_ptr)
    shared_ptr<std::string> message(
            new std::string("test")); 


    for (int i = 0; i<8; i++) {
        Networkhandler.send("127.0.0.1", message);
        usleep(500000);
    }


    message.reset();


    NetworkThread.join();
    return 0;
}
