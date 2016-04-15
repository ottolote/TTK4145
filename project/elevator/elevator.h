#pragma once 

#include "message_handler.h"
#include "elevator_properties.h"

//#include <boost/asio.hpp>
#include <string>

class Elevator{
private:
	std::string _ip;
	bool _current_orders[N_OUTSIDE_BUTTONS + N_OTHER_BUTTONS];

protected:
	direction_t _dir;
	floor_t _previous_floor;
	//boost::asio::deadline_timer _timeout_timer;

public:
	//Constructors
	Elevator(){ ; }
	Elevator(std::string ip,
		direction_t dir,
		floor_t floor,
		//boost::asio::io_service & parent_channel,
		int seconds)
		: _ip(ip), _dir(dir), _previous_floor(floor){ ; } //timeout_timer(parent_channel, posix_time::seconds(seconds)){};

	//Get functions
	std::string get_ip(){ return this->_ip; }
	direction_t get_dir(){ return this->_dir; }
	floor_t get_previous_floor(){ return this->_previous_floor; }

	//Set functions
	void set_ip(std::string ip){ this->_ip = ip; }
	void set_order(int order){ this->_current_orders[order] = true; }
	void set_dir(direction_t dir){ this->_dir = dir; }
	void set_previous_floor(floor_t floor){ this->_previous_floor = floor; }

	//Timer functions
	void timeout();//Elevator stuck between floors. Do more stuff
	void refresh_timer();

	int distance_from_order(int order);
	void exchange_order_list(bool *order_list);
};

class Internal_elevator : public Elevator{
private:
	bool _current_orders[N_BUTTONS];

public:
	void set_order(int order){ _current_orders[order] = true; }
};