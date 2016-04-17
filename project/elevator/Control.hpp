#pragma once

#include "elevator.h"
#include "message_format.h"
#include "Generic_thread.hpp"
#include "elevator_properties.h"
#include <map>
#include <string>


class Control: public Generic_thread{
private:
	Elevator internal_elevator;
	std::map<std::string, boost::shared_ptr<Elevator>> external_elevators;
    bool pending_orders[N_OUTSIDE_BUTTONS];
	boost::asio::deadline_timer open_door_timer;
	boost::asio::deadline_timer stranded_timer;
        
public:
    //Constructors
    //Might implement map_constructor in case 
    //backup process has to take over.
    //Also one that initializes internal_elevator
    //with non-default constructor.
    Control();
	void add_new_external_elevator(status_msg_t msg, std::string ip);


    //Interthread communication functions
    void deliver_button(int button, bool button_value); //Used by hardware thread
    void deliver_floor_sensor_signal(floor_t floor); //Used by hardware thread
    void deliver_status(status_msg_t message, std::string ip); //Used by communication thread
    void deliver_order(order_msg_t message, std::string ip); //Used by communication thread
    void report_useless_elevator(std::string ip);


    //Algorithm functions
    std::string find_closest_elevator(int order); //uses external orders
    void send_order_to_closest_elevator(int order);

	//Lights
	void set_order_button_lights(bool *lights_to_set);
	void determine_button_lights_to_set();
    
    void reverse_elevator_direction();

	//Internal elevator
	void clear_orders_at_floor(floor_t floor);
	void set_internal_elevator_direction(direction_t dir);
	void set_internal_elevator_order(int order, bool value);
	void set_internal_elevator_floor(floor_t floor);

    //Routines
    void button_routine(int button, bool button_value); //Called by deliver_button
    void stop_button_routine(bool button_value); 
    void floor_sensor_routine(floor_t floor); // Called by deliver_floor_sensor_signal
    void order_button_routine(int button, bool button_value);
    void pick_from_pending_orders();

	//Timer functions
	void refresh_open_door_timer();
	void refresh_stranded_timer();

	//Timeout
	void door_close();
	void elevator_stranded();

};
