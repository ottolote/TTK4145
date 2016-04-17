//DENNE ER ROTETE SOM ET HELVETE
//HUSK Å RYDDE OPP
// _button_channels = |outside_buttons|inside_buttons|other_buttons|

#pragma once



#include "elevator_properties.h"
#include "Generic_thread.hpp"
//#include "Control.hpp"

class Hardware: public Generic_thread{
private:
    bool _previous_button_values[N_BUTTONS];
    floor_t previous_floor_sensor_value;
    boost::asio::deadline_timer poll_tick_timer;

public:
    //Get functions
        Hardware();
    bool get_button_signal(int button);
    floor_t get_floor_sensor_signal();
    bool get_stop_signal();
    bool get_obstruction_signal();

    //Set functions
    void set_motor_direction(direction_t dir);
    void set_button_lamp(int button, bool light_value);
    void set_floor_indicator(floor_t floor);
    void set_door_open_lamp(bool val);
    void set_stop_lamp(bool val);

    //Poll function
    void poll_buttons();
    void poll_floor_sensor_changes();
    void restart_poll_timer();
    void poll(const boost::system::error_code &e);

    
};
