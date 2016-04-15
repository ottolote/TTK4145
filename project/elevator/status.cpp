#include "status.h"
//FILE NOT IN USE

void Status::update_elevator_direction(direction_t dir, std::string ip){
    this->external_elevators[ip].set_dir(dir);
}

void Status::update_elevator_previous_floor(floor_t floor, std::string ip){
    this->external_elevators[ip].set_previous_floor(floor);
}

void Status::update_elevator_order_list(bool *order_list, std::string ip){
    this->external_elevators[ip].exchange_order_list(order_list);
}
