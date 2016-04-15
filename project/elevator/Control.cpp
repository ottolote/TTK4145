#include "control.h"
#include <iterator>
#include <map>

//Should only be called for outside orders.
std::string Control::find_closest_elevator(int order){
    //Order from inside
    if (order > N_OUTSIDE_BUTTONS){
        return "Internal elevator";
    }

    std::string closest_elevator_ip = "";
    int least_distance = N_FLOORS, temp;

    //Check if internal elevator is valid
    int internal_elevator_distance = this->status.get_internal_elevator().distance_from_order(order);
    if (internal_elevator_distance != -1){
        least_distance = internal_elevator_distance;
        closest_elevator_ip = "Internal elevator";
    }

    //Check if external elevators are valid
    std::map<std::string, Elevator>::iterator it = this->status.get_external_elevators().begin();
    for(it; it != this->status.get_external_elevators().end(); it++){
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
        this->status.get_internal_elevator().set_order(order);
    }

    //An external elevator is closest
    else{
        //communication_thread->reliable_send(closest_elevator_ip);
    }
}

void Control::set_elevator_direction(direction_t dir){
    hardware_thread->set_motor_direction(dir);
    status.update_internal_elevator_direction(dir);

}
void Control::deliver_button(int button, bool value){
    this->button_routine(button, value);

}
//Only called when button value has changed
void Control::button_routine(int button, bool value){
    if (button == STOP){
        stop_button_routine(value);
        return;
    }
    this->send_order_to_closest_elevator(button); 

}

void Control::stop_button_routine(bool value){
    if (value){
        this->set_elevator_direction(DIR_STOP); //Stop elevator
        status.update_internal_elevator_direction(STRANDED); //Unavailable for orders until timeout
        bool empty_order_list[N_INSIDE_BUTTONS + N_OUTSIDE_BUTTONS] = { 0 };
        status.update_internal_elevator_order_list(empty_order_list);//remove all orders
    }
    else{
        //this->refresh_timeout_timer(); //start timer
    }
}

void Control::floor_sensor_routine(floor_t floor){
    if()
    //if floor is in order queue
    //  start timer and stop
    //
    //previous

}

