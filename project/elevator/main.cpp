/*
 *
 * main.hpp
 *
 * */

//#include "generic_thread.hpp"
#include "networkhandler.hpp"

#include <iostream>
#include <unistd.h>

using namespace boost;
using boost::asio::ip::udp;

int main() {

    boost::shared_mutex cout_lock;

    boost::shared_ptr<Network> NH( new Network );
    //boost::shared_ptr<generic_thread> GT( new generic_thread );;


    thread NetworkThread([&NH, &cout_lock]  {
        NH->init_thread(cout_lock);   // Thread function
    });







//    NH->add_reference_to(GT);
//    GT->add_reference_to(NH);
//    GT->run(); // Main blocks at this call and becomes the ThreadManager instead of starting a seperate thread and then idling
     

    // create shared pointer for message to be sent (boost::shared_ptr)
    shared_ptr<std::string> message(
            new std::string("test")); 


    for (int i = 0; i<8; i++) {
        NH->post(26956+i);
        usleep(500000);
    }


    message.reset();


    NetworkThread.join();
    return 0;
}
