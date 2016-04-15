#pragma once
//GOTTA CHANGE SOME STUFF HERE

//Buttons
#define N_OUTSIDE_BUTTONS 6
#define N_INSIDE_BUTTONS 4
#define N_OTHER_BUTTONS 2 //Stop and turbo
#define N_BUTTONS 12

#define N_ELEVATORS 4 //Probably not used
#define N_FLOORS 4

#define MOTOR_SPEED 2800

typedef enum direction_t { DIR_DOWN, DIR_UP, DIR_STOP, STRANDED };
typedef enum floor_t { NONE = -1, FIRST, SECOND, THIRD, FOURTH };

//Button types
typedef enum outside_buttons_t { FIRST_UP, SECOND_DOWN, SECOND_UP, THIRD_DOWN, THIRD_UP, FOURTH_DOWN }; //remove
typedef enum other_buttons_t { STOP = N_BUTTONS-2/*Stop button index*/, OBSTRUCTION }; //change
typedef floor_t inside_buttons_t; //Same variable names

/*PROBABLY NOT USED
struct Buttons_t {
	bool outside_buttons[N_OUTSIDE_BUTTONS]; 
	bool inside_buttons[N_INSIDE_BUTTONS]; 
	bool other_buttons[N_OTHER_BUTTONS]; 
};
*/
