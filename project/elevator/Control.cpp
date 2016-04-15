#include "Control.hpp"
#include <iterator>
#include <map>

//Helper functions
int button_to_floor(int button){
	return (button + 1) / 2;
}




std::string Control::find_closest_elevator(int order){
    //Order from inside
    if (order > N_OUTSIDE_BUTTONS){
        return "Internal elevator";
    }

    std::string closest_elevator_ip = "";
    int least_distance = N_FLOORS, temp;

    //Check if internal elevator is valid
    int internal_elevator_distance = internal_elevator.distance_from_order(order)

    if (internal_elevator_distance != -1){
        least_distance = internal_elevator_distance;
        closest_elevator_ip = "Internal elevator";
    }

    //Check if external elevators are valid
    std::map<std::string, Elevator>::iterator it = external_elevators.begin();
    for(it; it != external_elevators.end(); it++){
        temp = it->second.distance_from_order(order);
        if (temp != -1 && temp < least_distance){
            closest_elevator_ip = it->first;
        }
    }
    
    return closest_elevator_ip;
}

void Control::send_order_to_closest_elevator(int order){
    std::string closest_elevator_ip = find_closest_elevator(order);

    //All elevators going in the wrong direction
    if (closest_elevator_ip.empty()){
        //Try again later
    }
    
    //Internal elevator is closest or order came from inside
    else if (closest_elevator_ip == "Internal elevator"){
        //Add order to internal elevator queue
        internal_elevator.set_order(order, true);
    }

    //An external elevator is closest
    else{
        //communication_thread->reliable_send(closest_elevator_ip);
    }
}

//Might remove this function
void Control::set_elevator_direction(direction_t dir){
    //hardware_thread->set_motor_direction(dir);
    internal_elevator.set_dir(dir);

}

//Change name from button to order?
void Control::deliver_button(int button, bool value){
    this->button_routine(button, value);

}

//Only called when button value has changed
void Control::button_routine(int button, bool value){
    if (button == STOP){
        stop_button_routine(value);
        return;
    }
    else{
        if(value){ //a NOT stop button has been pressed
            //previous_floor is equal to floor of button
            if(internal_elevator.get_previous_floor() == button_to_floor(button)){
                //open door, delete timer

            }
            //No orders for current floor
            else{
                this->send_order_to_closest_elevator(button); 
            }
        }
        else{ // a NOT stop button has been released
            //refresh timer
        }
    }
}

void Control::order_button_routine(int button, bool button_value){
   if(button_value){ //a NOT stop button has been pressed
        //previous_floor is equal to floor of button
        if(internal_elevator.get_previous_floor() == button_to_floor(button)){
            //open door, delete timer

        }
        //No orders for current floor
        else{
            this->send_order_to_closest_elevator(button); 
        }
    }
    else{ // a NOT stop button has been released
        //refresh timer
    }
}

//Correct behaviour
void Control::stop_button_routine(bool value){
    if (value){
        this->set_elevator_direction(DIR_STOP); //Stop elevator
        internal_elevator.set_dir(STRANDED); //Unavailable for orders until timeout
        bool empty_order_list[N_FLOORS] = { 0 };
        internal_elevator.exchange_order_list(empty_order_list);
        //hardware_thread->set_door_open_lamp(1); //Clear on timeout
        //hardware_thread->set_stop_lamp(1); //Clear on timeout
        //delete timer
    }
    else{
        //this->refresh_timeout_timer(); //start timer on button release
    }
}

void Control::floor_sensor_routine(floor_t floor){
    internal_elevator.set_previous_floor(floor);
    //hardware_thread->set_floor_indicator(floor); //Might set this in hardware

    //Current floor is in order list
    if(internal_elevator.is_order_in_list(floor)){
        //this->refresh_timeout_timer(); 
        //hardware_thread->set_motor_direction(STOP); 
        internal_elevator.set_order(floor, false); //Clear order from order list
        
    }
}

