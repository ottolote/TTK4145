#pragma once

#include "elevator_properties.h"
int button_to_floor(int button);
//Every second order is in the same direction
direction_t direction_of_order(int order);
bool is_outside_order(int order);
int floor_to_order(floor_t floor, direction_t dir);