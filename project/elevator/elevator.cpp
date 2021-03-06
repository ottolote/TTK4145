#include "elevator.h"
#include "helper_functions.h"
#include <iostream>
#include <cmath>
#include "terminalcolors.h"

#define PROMPT "[" TCOLOR_YELLOW "Ext.Elevator" TCOLOR_NC "] : "


//Construct timers
Elevator::Elevator()
    : _current_orders( new bool[N_ORDER_BUTTONS]())
{
    this->_dir = DIR_STOP;
}

//construct timers
Elevator::Elevator(status_msg_t msg)
    : _current_orders( new bool[N_ORDER_BUTTONS]() )
{
    this->_dir = msg.dir;
    this->_previous_floor = msg.floor;
    for (int i = 0; i < N_OUTSIDE_BUTTONS; i++){
        this->_current_orders[i] = msg.order_list[i];
    }
}

//this is ok;;
int Elevator::distance_from_order(int order){
    //elevator is stopped or headed in right direction
    if (this->eligible_for_order(order)){
        return abs(_previous_floor - button_to_floor(order));
    }

    //Elevator is stranded or headed in the wrong direction
    else{
        return -1;
    }
}

//this is ok
bool Elevator::eligible_for_order(int order){
    bool order_in_correct_direction = direction_of_order(order) == this->_dir;
    bool elevator_is_stopped = this->_dir == DIR_STOP;
    bool headed_to_order;
        if(this->_dir == DIR_UP){
            headed_to_order = button_to_floor(order) >= _previous_floor;
        }
        else{
            headed_to_order = button_to_floor(order) <= _previous_floor;
        }

    return ((headed_to_order && order_in_correct_direction) || elevator_is_stopped) && _dir != STRANDED;
}

//this is ok
bool Elevator::is_order_list_empty(){
    for(int i = 0; i < N_ORDER_BUTTONS; i++){
        if(_current_orders[i]){
            return false;
        }
    }
    return true;
}

////this is ok
//bool* Elevator::get_outside_orders() { 
//    bool outside_orders[N_OUTSIDE_BUTTONS];
//    for (int i = 0; i < N_OUTSIDE_BUTTONS; i++){
//        outside_orders[i] = _current_orders[i];
//    }
//
//    return outside_orders;
//}

//Will only be called when arriving at floor
//Should be ok
bool Elevator::is_current_floor_in_order_list(floor_t current_floor){
    bool is_in_list;
    if (current_floor == FOURTH || current_floor == FIRST){
        return true; // Always stop in fourth and first floor
    }
    else{
        if (_dir == DIR_UP){
            is_in_list = _current_orders[current_floor * 2];
        }
        else{
            is_in_list = _current_orders[current_floor * 2 - 1];
        }
    }
    return _current_orders[N_OUTSIDE_BUTTONS + current_floor] || is_in_list;

}
//this is ok
//
//return struct with pointer to new status message
status_msg_t Elevator::get_status(){
    status_msg_t msg;

    for (int i = 0; i<N_OUTSIDE_BUTTONS; i++) {
        msg.order_list[i] = _current_orders[i];
//        std::cout << PROMPT "looping: " << i 
//            << " - current orders: " << _current_orders[i] << std::endl;
    }
    msg.dir = get_dir();
    msg.floor = get_previous_floor();
//
    return msg;
}

void Elevator::exchange_order_list(bool *order_list){
    for(int i = 0; i < N_OUTSIDE_BUTTONS; i++){
        _current_orders[i] = order_list[i];
    }
}
