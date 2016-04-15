/*
 *
 * main.hpp
 *
 * */

//#include "Generic_thread.hpp"
#include "Communication.hpp"

#include <iostream>
#include <unistd.h>


#include <bitset>

using namespace boost;
using boost::asio::ip::udp;

int main(int argc, char* argv[]) {

if (argc != 1) {
    std::cout << "no parameters here, just " << argv[0] << std::endl;
}
//    int verboselevel = 0;
//    for (int i = 1; i<argc; i++) {
//        if (argv[i] == "-v") {
//            verboselevel++;
//        }
//    }
//
//    if (verboselevel > 4) {
//        std::cout << "verbose level set to " << verboselevel << std::endl;
//    }

    //boost::shared_ptr<Network> NH( new Network );
    boost::shared_ptr<Communication> comms( new Communication );;


    //thread NetworkThread([&NH, &cout_lock]  {
    //    NH->init_thread(cout_lock);   // Thread function
    //});

    thread comms_thread([&comms]  {
        comms->run();   // Thread function
    });





//    NH->add_reference_to(GT);
//    GT->add_reference_to(NH);
//    GT->run(); // Main blocks at this call and becomes the ThreadManager instead of starting a seperate thread and then idling
     

    // create shared pointer for message to be sent (boost::shared_ptr)
    //shared_ptr<std::string> message(
    //        new std::string("test")); 
    
//    for (uint16_t i = 0; i<255; i++) {
//        unsigned char t = {(unsigned char)i};
//        std::bitset<16> binary_i(i);
//        std::bitset<16> binary_t(t);
//        std::cout << (uint8_t)t << " - " << binary_t << " - "  
//            << i << " - " << binary_i << std::endl;
//        usleep(50000);
//    }


    for (uint16_t i = 0; i<800; i++) {
       // if (i > 4) { cout_lock.lock_shared(); }
        comms->reliable_send((encoded_msg_t)(i));
        std::cout << "Main: Sendt message - " << i << std::endl;
        usleep(50000);
    }


    //message.reset();


    comms_thread.join();

    return 0;
}
