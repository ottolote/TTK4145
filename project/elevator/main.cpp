/*
 *
 * main.hpp
 *
 * */


#include "Generic_thread.hpp"
#include "Communication.hpp"

#include "Control.hpp"

#include "message_handler.hpp"

#include <iostream>
#include <unistd.h>


#include <bitset>


int main(int argc, char* argv[]) {

std::string ip;


if (argc > 1) {
    ip = argv[1];
} else {
    ip = "127.0.0.1";
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
//    boost::shared_ptr<Communication> comms( new Communication );;
    //boost::shared_ptr<Hardware> hardware( new Hardware );;


    //thread NetworkThread([&NH, &cout_lock]  {
    //    NH->init_thread(cout_lock);   // Thread function
    //});

//    boost::thread comms_thread([&]  {
//        comms->run();   // Thread function
//    });

    //comms->init_thread_pointers(comms, control, hardware);
    

//
//    boost::shared_ptr<std::string> test(
//            new std::string(ip));
//
//
//    for (uint16_t i = 519; i<520; i++) {
//        std::bitset<16> binary_i(i);
//        std::cout << binary_i << std::endl;
//
//        comms->reliable_send(i, *test);
//        usleep(500000);
//    }
//    sleep(1000000);
//



//    comms_thread.join();

    return 0;
}
