#pragma once 

#include "message_handler.hpp"
#include "elevator_properties.h"

//#include <boost/asio.hpp>
#include <string>

class Elevator{
private:
	std::string _ip;
	direction_t _dir;
	floor_t _previous_floor;
	bool _current_orders[N_FLOORS];

public:
	//Constructors
	Elevator(){ ; }
	Elevator(std::string ip,
		direction_t dir,
		floor_t floor,
		int seconds)
		: _ip(ip), _dir(dir), _previous_floor(floor){ ; }

	//Get functions
	std::string get_ip(){ return this->_ip; }
	direction_t get_dir(){ return this->_dir; }
	floor_t get_previous_floor(){ return this->_previous_floor; }

	//Set functions
	void set_ip(std::string ip){ this->_ip = ip; }
	void set_order(int order, bool value){ this->_current_orders[order] = value; }
	void set_dir(direction_t dir){ this->_dir = dir; }
	void set_previous_floor(floor_t floor){ this->_previous_floor = floor; }

	int distance_from_order(int order);
	bool eligible_for_order(int order);
	void exchange_order_list(bool *order_list);
    bool is_order_in_list(int order){return _current_orders[order];} //Add acceptance test
    bool is_order_list_empty();
};
