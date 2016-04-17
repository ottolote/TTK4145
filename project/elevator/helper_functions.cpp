#include "elevator_properties.h"
#include "helper_functions.h"

//this is ok
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
//Orders at the end exception. 
//Treated as being in the opposite direction
//this is ok
direction_t direction_of_order(int order){
	if ((order == (int)FIRST_UP || order % 2) && order != (int)FOURTH_DOWN){
		return DIR_DOWN;
	}
	else{
		return DIR_UP;
	}
}

bool is_outside_order(int order){return order < N_OUTSIDE_BUTTONS;}


int floor_to_order(floor_t floor, direction_t dir) {
    if (dir == DIR_UP) {
        return floor*2;
    } else {
        return floor*2-1;
    }
}


