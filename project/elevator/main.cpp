/*
 *
 * main.hpp
 *
 * */

#include "Generic_thread.hpp"
#include "Communication.hpp"
//#include "Hardware.hpp"

#include <iostream>
#include <unistd.h>


#include <bitset>


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
    //boost::shared_ptr<Hardware> hardware( new Hardware );;


    //thread NetworkThread([&NH, &cout_lock]  {
    //    NH->init_thread(cout_lock);   // Thread function
    //});

    boost::thread comms_thread([&]  {
        comms->run();   // Thread function
    });




//    for (uint16_t i = 1000; i<60000; i++) {
//        std::bitset<16> binary_i(i);
//        std::cout << binary_i << std::endl;
//
//        comms->reliable_send(i, "127.0.0.1");
//        usleep(50000);
//    }




    comms_thread.join();

    return 0;
}
