/*
 *
 * main.hpp
 *
 * */


#include "Generic_thread.hpp"
#include "Communication.hpp"
#include "Hardware.hpp"

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

    boost::shared_ptr<Communication> comms( new Communication );;
    boost::shared_ptr<Hardware> hardware( new Hardware );;
    boost::shared_ptr<Control> control( new Control );


    comms->add_pointers_to_all_other_threads( control, hardware, comms );
    hardware->add_pointers_to_all_other_threads( control, hardware, comms );
    control->add_pointers_to_all_other_threads( control, hardware, comms );


    boost::thread control_thread([&]  {
        control->run();   // Thread function
    });

    boost::thread comms_thread([&]  {
        comms->run();   // Thread function
    });

    boost::thread hardware_thread([&]{
            hardware->run(); });
          

    //comms->init_thread_pointers(comms, control, hardware);
    
//    while(true) {
//        hardware->set_stop_lamp(true);
//        sleep(1);
//        hardware->set_stop_lamp(false);
//        sleep(1);
//    }

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



    comms_thread.join();
    control_thread.join();
    hardware_thread.join();


    return 0;
}
