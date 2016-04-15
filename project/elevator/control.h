#pragma once

#include "status.h"
#include "message_format.h"
#include "generic_thread.hpp"
#include <string>


class Control: public Generic_thread{
private:
	Status status;

public:
	//Interthread communication functions
	void deliver_button(int button, bool value); //Used by hardware thread
	void deliver_floor_sensor_signal(floor_t floor); //Used by hardware thread
	void deliver_status(status_msg_t message); //Used by communication thread
	void deliver_order(order_msg_t message); //Used by communication thread

	//Algorithm functions
	std::string find_closest_elevator(int order); //uses external orders
	void send_order_to_closest_elevator(int order);
	void set_elevator_direction(direction_t dir);

	void stop_button_routine(bool value);
};