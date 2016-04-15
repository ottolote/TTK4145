#include "elevator.h"
#include <cmath>

//Implemented in control as well. Fix this.
direction_t direction_of_order(int order);
//Implemented in control as well. Fix this.
int button_to_floor(int button);

//Should also return -1 if order is under elevator
int Elevator::distance_from_order(int order){
	//elevator is stopped or headed in right direction
	if (this->eligable_for_order(order)){
		return abs(this->_previous_floor - button_to_floor(order));
	}

	//Elevator is stranded or headed in the wrong direction
	else{
		return -1;
	}

}

bool Elevator::eligible_for_order(int order){
	bool order_in_right_direction = direction_of_order(order) == this->_dir;
	bool elevator_is_stopped = this->_dir == DIR_STOP;
	bool headed_away_from_order;
		if(this->_dir == DIR_UP){
			headed_away_from_order = button_to_floor(order) > _previous_floor;
		}
		else{
			headed_away_from_order = button_to_floor(order) < _previous_floor;
		}

	return (!headed_away_from_order && order_in_right_direction) || elevator_is_stopped;
}

bool Elevator::is_order_list_empty(){
	for(int i = 0; i < N_FLOORS; i++){
		if(_current_orders[i]){
			return false;
		}
	}
	return true;
}

int button_to_floor(int button){
	return (button + 1) / 2;
}

//Orders going up are odd numbers while downward orders are even.
direction_t direction_of_order(int order){
	return static_cast<direction_t>(order % 2);
}