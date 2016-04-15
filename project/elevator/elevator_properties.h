#ifndef _ELEV_PROPS_H_
#define _ELEV_PROPS_H_

//Buttons
#define N_OUTSIDE_BUTTONS 6
#define N_INSIDE_BUTTONS 4
#define N_ORDER_BUTTONS 10
#define N_OTHER_BUTTONS 2 //Stop and turbo
#define N_BUTTONS 12

#define N_ELEVATORS 4 //Probably not used
#define N_FLOORS 4

#define MOTOR_SPEED 2800

enum direction_t { DIR_DOWN, DIR_STOP, DIR_UP, STRANDED };
enum floor_t { NONE = -1, FIRST, SECOND, THIRD, FOURTH };

//Button types
enum outside_buttons_t { FIRST_UP, SECOND_DOWN, SECOND_UP, THIRD_DOWN, THIRD_UP, FOURTH_DOWN };
enum other_buttons_t { STOP, OBSTRUCTION };
typedef floor_t inside_buttons_t; //Same variable names

struct Buttons_t {
	bool outside_buttons[N_OUTSIDE_BUTTONS]; 
	bool inside_buttons[N_INSIDE_BUTTONS]; 
	bool other_buttons[N_OTHER_BUTTONS]; 
};

#endif
