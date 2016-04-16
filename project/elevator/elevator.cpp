#include "elevator.h"
#include <cmath>

//Implemented in control as well. Fix this.
direction_t direction_of_order(int order);
//Implemented in control as well. Fix this.
int button_to_floor(int button);

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

int button_to_floor(int button){
	//Button is on the outside
	if (button < N_OUTSIDE_BUTTONS){
		return (button + 1) / 2;
	}
	//Button is on the inside
	else{
		return button - N_OUTSIDE_BUTTONS;
	}
}

//Orders going up are odd numbers while downward orders are even.
//Orders at the end are treated as being in the opposite direction
direction_t direction_of_order(int order){
	if ((order == FIRST_UP || order % 2) && order != FOURTH_DOWN){
		return DIR_DOWN;
	}
	else{
		return DIR_UP;
	}
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
