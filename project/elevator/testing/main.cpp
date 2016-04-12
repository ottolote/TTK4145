#include "Thread_handler.hpp"
#include "networkhandler.hpp"

#include <iostream>
#include <unistd.h>

using namespace boost;
using boost::asio::ip::udp;

int main() {

    asio::io_service io;


    
    // Start asyncronous Networkhandler in new thread
    shared_ptr<Network> Networkhandlerpointer( new Network ); // Thread object
    thread NetworkThread([&] {
        Networkhandlerpointer->run();   // Thread function
    });

    shared_ptr<Threadmanager> TM( new Threadmanager );;
    TM->init_parent_channel(Networkhandlerpointer);
    Networkhandlerpointer->init_parent_channel(TM);
    TM->start();
    TM->run();

     

    // create shared pointer for message to be sent (boost::shared_ptr)
    shared_ptr<std::string> message(
            new std::string("test")); 


//    for (int i = 0; i<8; i++) {
//        Networkhandler.send("127.0.0.1", message);
//        usleep(500000);
//    }


    message.reset();


    NetworkThread.join();
    return 0;
}
