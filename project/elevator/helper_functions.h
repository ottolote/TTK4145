int button_to_floor(int button);
//Every second order is in the same direction
direction_t direction_of_order(int order);
bool is_outside_order(int order){return order < N_OUTSIDE_BUTTONS;}