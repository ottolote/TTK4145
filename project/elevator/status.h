//FILE NOT IN USE
#pragma once

#include "elevator.h"
#include "elevator_properties.h"
#include "message_handler.hpp"

#include <map>
#include <string>


class Status {
private:
	Elevator internal_elevator;
	std::map<std::string, Elevator> external_elevators;

public:
        //Does not work
	//External elevators
	//Update functions
	void update_elevator_direction(direction_t dir, std::string ip);
	void update_elevator_previous_floor(floor_t floor, std::string ip);
	void update_elevator_order_list(bool *order_list, std::string ip);

	//Get functions
	std::map<std::string, Elevator> get_external_elevators(){ return this->external_elevators; }
	Elevator get_internal_elevator(){ return this->internal_elevator; }
};
