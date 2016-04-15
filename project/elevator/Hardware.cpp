#include "hardware.h"
#include "channels.h"
#include "io.h"
#include "status.h"


//Get functions
bool Hardware::get_button_signal(int button){
    return io_read_bit(_button_channels[button]);
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

    case DIR_DOWN:
        io_write_analog(MOTOR, 0);
        break;

    case DIR_UP:
        io_clear_bit(MOTORDIR);
        io_write_analog(MOTOR, MOTOR_SPEED);
        break;

    case DIR_STOP:
        io_set_bit(MOTORDIR);
        io_write_analog(MOTOR, MOTOR_SPEED);
        break;

    //Invalid input or stranded
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
    io_set_bit(LIGHT_DOOR_OPEN);
}

void Hardware::set_stop_lamp(bool val){
    io_set_bit(LIGHT_STOP);
}


void Hardware::set_button_lamp(int button){
    io_set_bit(_button_channels[button]);

}

void Hardware::poll_buttons(){
    for (int button = 0; button < N_BUTTONS; button++){
        //Status of button changed
        bool current_button_value = this->get_button_signal(button);
        if (current_button_value != _previous_button_values[button]){
            _previous_button_values[button] = current_button_value;
            control_thread->deliver_button(button, current_button_value);
        }
    }
}

void Hardware::poll_floor_sensor_changes(){
    floor_t current_floor = get_floor_sensor_signal();
    if (current_floor != previous_floor_sensor_value){
        previous_floor_sensor_value = current_floor;
        control_thread->deliver_floor_sensor_signal(current_floor);
    }
}

