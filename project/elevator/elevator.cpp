#include "elevator.h"
#include "helper_functions.h"
#include <cmath>

Elevator::Elevator(){
	for(int i = 0; i < N_ORDER_BUTTONS; i++){
		_current_orders[i] = false;
	}
	this->_dir = DIR_STOP;
}

Elevator::Elevator(status_msg_t msg){
	this->_dir = msg.dir;
	this->_previous_floor = msg.floor;
	for (int i = 0; i < N_OUTSIDE_BUTTONS; i++){
		this->_current_orders[i] = msg.order_list[i];
	}
}

int Elevator::distance_from_order(int order){
	//elevator is stopped or headed in right direction
	if (this->eligible_for_order(order)){
		return abs(this->_previous_floor - button_to_floor(order));
	}

	//Elevator is stranded or headed in the wrong direction
	else{
		return -1;
	}

}

bool Elevator::eligible_for_order(int order){
	bool order_in_correct_direction = direction_of_order(order) == this->_dir;
	bool elevator_is_stopped = this->_dir == DIR_STOP;
	bool headed_away_from_order;
		if(this->_dir == DIR_UP){
			headed_away_from_order = button_to_floor(order) > _previous_floor;
		}
		else{
			headed_away_from_order = button_to_floor(order) < _previous_floor;
		}

	return ((!headed_away_from_order && order_in_correct_direction) || elevator_is_stopped) && _dir != STRANDED;
}

bool Elevator::is_order_list_empty(){
	for(int i = 0; i < N_ORDER_BUTTONS; i++){
		if(_current_orders[i]){
			return false;
		}
	}
	return true;
}

bool* Elevator::get_outside_orders() { 
	bool outside_orders[N_OUTSIDE_BUTTONS];
	for (int i = 0; i < N_OUTSIDE_BUTTONS; i++){
		outside_orders[i] = _current_orders[i];
	}

	return outside_orders;
}

//Will only be called when arriving at floor
bool Elevator::is_current_floor_in_order_list(floor_t floor){
	bool is_in_list;
	if (floor == FOURTH || floor == FIRST){
		return true;
	}
	else{
		if (_dir == DIR_UP){
			is_in_list = _current_orders[floor * 2];
		}
		else{
			is_in_list = _current_orders[floor * 2 - 1];
		}
	}
	return _current_orders[N_OUTSIDE_BUTTONS + floor] || is_in_list;
}
status_msg_t Elevator::get_status(){
	status_msg_t msg;
	*msg.order_list = *get_outside_orders();
	msg.dir = get_dir();
	msg.floor = get_previous_floor();

	return msg;
}


void Elevator::exchange_order_list( bool *list) {
    return;
}

