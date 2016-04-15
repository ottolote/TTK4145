#pragma once

#include "elevator.h"
#include "message_format.h"
#include "Generic_thread.hpp"
#include <string>


class Control: public Generic_thread{
private:
	Elevator internal_elevator;
	std::map<std::string, Elevator> external_elevators;
        
public:
    //Interthread communication functions
    void deliver_button(int button, bool value); //Used by hardware thread
    void deliver_floor_sensor_signal(floor_t floor); //Used by hardware thread
    void deliver_status(status_msg_t message); //Used by communication thread
    void deliver_order(order_msg_t message); //Used by communication thread

    //Algorithm functions
    std::string find_closest_elevator(int order); //uses external orders
    void send_order_to_closest_elevator(int order);
    void set_elevator_direction(direction_t dir); //might be removed

    //Routines
    void button_routine(int button, bool value); //Called by deliver_button
    void stop_button_routine(bool value); 
    void floor_sensor_routine(floor_t floor); // Called by deliver_floor_sensor_signal
    void order_button_routine(int button, bool button_value);
};
