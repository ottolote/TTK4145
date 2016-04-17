#include "Hardware.hpp"
#include "channels.h"
#include "io_old.h"
#include "status.h"
#include "Control.hpp"

#include "terminalcolors.h"

#include <iostream>


#define POLL_TICK_DURATION_MS 500

#define PROMPT "[" TCOLOR_LIGHTGREEN "Hardware" TCOLOR_NC "] : "

int _button_channels[N_BUTTONS] = 
    { BUTTON_UP1, 
      BUTTON_DOWN2, BUTTON_UP2, 
      BUTTON_DOWN3, BUTTON_UP3, 
      BUTTON_DOWN4,
      BUTTON_COMMAND1, BUTTON_COMMAND2, BUTTON_COMMAND3, BUTTON_COMMAND4, STOP, OBSTRUCTION}; 

int light_channels[N_BUTTONS] = 
	{ LIGHT_UP1,
	  LIGHT_DOWN2, LIGHT_UP2,
	  LIGHT_DOWN3, LIGHT_UP3,
	  LIGHT_DOWN4,
	  LIGHT_COMMAND1, LIGHT_COMMAND2, LIGHT_COMMAND3, LIGHT_COMMAND4, LIGHT_STOP, LIGHT_DOOR_OPEN };


Hardware::Hardware() 
    :  Generic_thread(),
       poll_tick_timer(io, boost::posix_time::milliseconds(POLL_TICK_DURATION_MS)){


        poll_tick_timer.async_wait([&] (const boost::system::error_code &e) {
                    poll(e);});
        io_init();
    	previous_floor_sensor_value = NONE;
        for(int i = 0; i < N_BUTTONS; i++){
        	_previous_button_values[i] = 0;
        }
}

//Get functions
bool Hardware::get_button_signal(int button){
	return io_read_bit(_button_channels[button]);
}



void Hardware::restart_poll_timer() {
    //poll_tick_timer.cancel();
    poll_tick_timer.expires_from_now(boost::posix_time::milliseconds(POLL_TICK_DURATION_MS));
    poll_tick_timer.async_wait([&](const boost::system::error_code &e) {
        poll(e); });
}


void Hardware::poll(const boost::system::error_code &e) {

    if (e == boost::asio::error::operation_aborted) {return;}

    poll_buttons();
    poll_floor_sensor_changes();
    
    restart_poll_timer();

    //std::cout << PROMPT "polling\n";
}


floor_t Hardware::get_floor_sensor_signal(){
	if (io_read_bit(SENSOR_FLOOR1)) {
		return FIRST;
	}
	else if (io_read_bit(SENSOR_FLOOR2)) {
		return SECOND;
	}
	else if (io_read_bit(SENSOR_FLOOR3)) {
		return THIRD;
	}
	else if (io_read_bit(SENSOR_FLOOR4)) {
		return FOURTH;
	}
	else {
		return NONE;
	}

}

bool Hardware::get_stop_signal(){
	return io_read_bit(STOP);
}

bool Hardware::get_obstruction_signal(){
	return io_read_bit(OBSTRUCTION);
}

//Set functions
void Hardware::set_motor_direction(direction_t dir){
	switch (dir){

	case DIR_STOP:
	case STRANDED:
		io_write_analog(MOTOR, 0);
		break;

	case DIR_UP:
		io_clear_bit(MOTORDIR);
		io_write_analog(MOTOR, MOTOR_SPEED);
		break;

	case DIR_DOWN:
		io_set_bit(MOTORDIR);
		io_write_analog(MOTOR, MOTOR_SPEED);
		break;

	//Invalid input
	default:
		break;

	}
}

void Hardware::set_floor_indicator(floor_t floor){
	// Binary encoding. One light must always be on.
	if (floor & 0x02) {
		io_set_bit(LIGHT_FLOOR_IND1);
	}
	else {
		io_clear_bit(LIGHT_FLOOR_IND1);
	}

	if (floor & 0x01) {
		io_set_bit(LIGHT_FLOOR_IND2);
	}
	else {
		io_clear_bit(LIGHT_FLOOR_IND2);
	}
}

void Hardware::set_door_open_lamp(bool val){
    if (val) {
	io_set_bit(LIGHT_DOOR_OPEN);
    } else {
        io_clear_bit(LIGHT_DOOR_OPEN);
    }
}

void Hardware::set_stop_lamp(bool val){
    if (val) {
	io_set_bit(LIGHT_STOP);
    } else {
	io_clear_bit(LIGHT_STOP);
    }
}


void Hardware::set_button_lamp(int button, bool light_value){
	if(light_value){
		io_set_bit(light_channels[button]);
	}
	else{
		io_clear_bit(light_channels[button]);
	}

}

//this is ok
void Hardware::poll_buttons(){
	bool current_button_value;
	for (int button = 0; button < N_BUTTONS; button++){
		current_button_value = this->get_button_signal(button);
		//Status of button changed
		if (current_button_value != _previous_button_values[button]){
			_previous_button_values[button] = current_button_value;
                        if(control == nullptr) {
                            std::cout << PROMPT " control is a nullpointer, here comes the segfault!\n";
                        }
			control->deliver_button(button, current_button_value);
		}
	}
}


void Hardware::poll_floor_sensor_changes(){
	floor_t current_floor = get_floor_sensor_signal();
	if (current_floor != previous_floor_sensor_value){
		previous_floor_sensor_value = current_floor;
		//control->deliver_floor_sensor_signal(current_floor);
	}
}

