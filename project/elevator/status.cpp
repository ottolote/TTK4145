#include "status.h"

//Recieved an order from another elevator
//Where to store orders to self?

void Status::update_internal_elevator_direction(direction_t dir){
    this->internal_elevator.set_dir(dir);
}

void Status::update_internal_elevator_previous_floor(floor_t floor){
    this->internal_elevator.set_previous_floor(floor);
}

void Status::update_internal_elevator_order_list(bool *order_list){
    this->internal_elevator.exchange_order_list(order_list);
}

void Status::update_external_elevator_direction(direction_t dir, std::string ip){
    this->external_elevators[ip].set_dir(dir);
}

void Status::update_external_elevator_previous_floor(floor_t floor, std::string ip){
    this->external_elevators[ip].set_previous_floor(floor);
}

void Status::update_external_elevator_order_list(bool *order_list, std::string ip){
    this->external_elevators[ip].exchange_order_list(order_list);
}
