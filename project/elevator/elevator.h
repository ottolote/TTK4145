#pragma once 

#include "message_handler.hpp"
#include "message_format.h"
#include "elevator_properties.h"

//#include <boost/asio.hpp>
#include <string>

class Elevator{
private:
	direction_t _dir;
	floor_t _previous_floor;
	bool _current_orders[N_ORDER_BUTTONS];

public:
	//Constructors
	Elevator();
	Elevator(status_msg_t msg);

	//Get functions
	bool get_order(int order){ return _current_orders[order]; }
	bool* get_outside_orders();
	direction_t get_dir(){ return this->_dir; }
	floor_t get_previous_floor(){ return this->_previous_floor; }
	status_msg_t get_status();

	//Set functions
	void set_order(int order, bool value){ this->_current_orders[order] = value; }
	void set_dir(direction_t dir){ this->_dir = dir; }
	void set_previous_floor(floor_t floor){ this->_previous_floor = floor; }
	void exchange_order_list(bool *order_list);

	int distance_from_order(int order);

	//Logic functions
	bool is_current_floor_in_order_list(floor_t floor);
    bool is_order_list_empty();
	bool eligible_for_order(int order);
};
