#include "elevator.h"
#include <cmath>

direction_t direction_of_order(int order);
//Implemented in control as well. Fix this.
int button_to_floor(int button);

int Elevator::distance_from_order(int order){
	//elevator is stopped or headed in right direction
	if (direction_of_order(order) == this->_dir || this->_dir == DIR_STOP){
		return abs(this->_previous_floor - button_to_floor(order));
	}

	//Elevator is stranded or headed in the wrong direction
	else{
		return -1;
	}

}

//Orders going up are odd numbers while downward orders are even.
direction_t direction_of_order(int order){
	return static_cast<direction_t>(order % 2);
}

int button_to_floor(int button){
	return (button + 1) / 2;
}
