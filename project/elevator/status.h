//STATUS IS A BAD NAME
#pragma once

#include "elevator.h"
#include "elevator_properties.h"
#include "message_handler.h"

#include <map>
#include <string>


class Status{
private:
    Internal_elevator internal_elevator;
    std::map<std::string, Elevator> external_elevators;

public:
    //External elevators
    //Update functions
    void update_external_elevator_direction(direction_t dir, std::string ip);
    void update_external_elevator_previous_floor(floor_t floor, std::string ip);
    void update_external_elevator_order_list(bool *order_list, std::string ip);

    //Get functions
    std::map<std::string, Elevator> get_external_elevators(){ return this->external_elevators; }

    //Internal elevator
    //Update functions
    void update_internal_elevator_direction(direction_t dir);
    void update_internal_elevator_previous_floor(floor_t floor);
    void update_internal_elevator_order_list(bool *order_list);

    //Get functions
    Internal_elevator get_internal_elevator(){ return this->internal_elevator; }
};
