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
#include <stdlib.h>

#include <csignal>
#include <boost/asio.hpp>


#include <bitset>


bool is_backup;


void signalhandler(int signal) {
    is_backup = true;
    
}


int main(int argc, char* argv[]) {

    std::signal(SIGUSR1, signalhandler);

    do { 
        is_backup = false; sleep(3) ;
    } while ( is_backup );
        

    std::cout << " NO LONGER BACKUP \n ";

    system("gnome-terminal -x ./elevator");

    std::string ip;


    if (argc > 1) {
        ip = argv[1];
    } else {
        ip = "127.0.0.1";
    }



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
          

    std::bitset<16> binary_i(519);
    std::cout << binary_i << std::endl;

    comms->reliable_send(519, ip);
    comms->send_status(144, ip);

    while(true) {
        system("pkill --signal USR1 elevator");
        sleep(1);
    }



    comms_thread.join();
    control_thread.join();
    hardware_thread.join();


    return 0;
}
