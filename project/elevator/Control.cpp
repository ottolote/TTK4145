#include "Control.hpp"
#include "Hardware.hpp"
#include "Communication.hpp"
#include "helper_functions.h"
#include <iterator>

/*********THINGS TO WATCH OUT FOR************/
//Several identical functions are implemented
//in both elevator and control
/********************************************/

//Will be removed later.
Hardware *hardware_thread;
Communication *communication_thread;

//Constructors
//this is ok
Control::Control()
    : open_door_timer(io, boost::posix_time::seconds(DOOR_TIMEOUT)),
      stranded_timer(io, boost::posix_time::seconds(STRANDED_TIMEOUT))
{
    for(int i = 0; i < N_ORDER_BUTTONS; i++){
        pending_orders[i] = false;
    }
}

//this is ok
void Control::add_new_external_elevator(status_msg_t msg, std::string ip){
	boost::shared_ptr<Elevator> new_elevator(new Elevator(msg));
	external_elevators[ip] = new_elevator;
}


//Routine functions//
//Only called when button value has changed
//this is ok
void Control::button_routine(int button, bool button_value){
    if (button == STOP_BUTTON){
        stop_button_routine(button_value);
        return;
    }
    else{
        order_button_routine(button, button_value);
    }
}

//Elevator will hold if a button at floor is continously pressed
//this is ok
void Control::order_button_routine(int button, bool button_value){
	bool order_is_for_current_floor = internal_elevator.get_previous_floor() == button_to_floor(button) 
        							  && direction_of_order(button) == internal_elevator.get_dir();

	if(order_is_for_current_floor){
		//button is pressed
        if(button_value){
			open_door_timer.cancel(); //elevator stays while the button is held

        }
        //button is released
        else{
            refresh_open_door_timer(); //Timer starts when the button is released
        }
    }
    //Order wasn't to current floor and should be sent to closest elevator
    else{
		this->send_order_to_closest_elevator(button);
    }
}

//this is ok
void Control::stop_button_routine(bool button_value){
	//Stop button triggered
    if (button_value){
		set_internal_elevator_direction(STRANDED); //Unavailable for orders until timeout
        bool empty_order_list[N_ORDER_BUTTONS] = { 0 };
        internal_elevator.exchange_order_list(empty_order_list);
        hardware_thread->set_door_open_lamp(1);
        hardware_thread->set_stop_lamp(1);
		open_door_timer.cancel();
    }
    else{
		refresh_open_door_timer(); //start door_open_timer on button release
    }
}

//this is ok
void Control::floor_sensor_routine(floor_t floor){
	refresh_stranded_timer(); //Motor still working if floor sensor changes
    
	//Update status variables to this floor
	if (floor != -1){
		set_internal_elevator_floor(floor);

		//Current floor is in order list
		if (internal_elevator.is_current_floor_in_order_list(floor)){
			refresh_open_door_timer(); //Stay at this floor
			hardware_thread->set_motor_direction(DIR_STOP); //don't change current_dir
			clear_orders_at_floor(floor);
		}
	}
}


//Algorithm functions//
//Working but should probably clean this up
//Maybe use find_closest_external_elevator() or something
std::string Control::find_closest_elevator(int order){
    //Order comes from inside
    if (order > N_OUTSIDE_BUTTONS){
        return "Internal elevator";
    }

    std::string closest_elevator_ip = "";
    int least_distance = N_FLOORS, temp;

    //Check if internal elevator is valid
	int internal_elevator_distance = internal_elevator.distance_from_order(order);

    if (internal_elevator_distance != -1){
        least_distance = internal_elevator_distance;
        closest_elevator_ip = "Internal elevator";
    }

    //Check if external elevators are valid
    std::map<std::string, boost::shared_ptr<Elevator>>::iterator it = external_elevators.begin();
    for(it; it != external_elevators.end(); it++){
        temp = it->second->distance_from_order(order);
        if (temp != -1 && temp < least_distance){
            closest_elevator_ip = it->first;
        }
    }
    
    return closest_elevator_ip;
}

//this is ok
void Control::send_order_to_closest_elevator(int order){
    std::string closest_elevator_ip = find_closest_elevator(order);

    //All elevators going in the wrong direction
    //send to pending list
    if (closest_elevator_ip.empty()){
    	pending_orders[order] = true;
    	communication_thread->send_pending_order(order); //Should be implemented sometime
    }
    
    //Internal elevator is closest or order came from inside
    else if (closest_elevator_ip == "Internal elevator"){
    	set_internal_elevator_order(order, true);
    }

    //An external elevator is closest
    else{
        communication_thread->send_order(order, closest_elevator_ip);
    }
}

//should work nicely
//Uses direction of order though
//which should be removed
//should be working correctly now
void Control::pick_from_pending_orders(){
    //iterate through pending list
    //find order farthest away in opposite direction
	bool pending_orders_empty = true;
    for(int order = 0; order < N_OUTSIDE_BUTTONS; order++){
        //Take all pending orders headed in opposite direction
        if(pending_orders[order] && direction_of_order(order) != internal_elevator.get_dir()){
        	pending_orders_empty = false;
            communication_thread->clear_pending_order(order); //Should be implemented sometime
            set_internal_elevator_order(order, true);
            reverse_elevator_direction();
        }

    }
    //No more orders in pending list
    if(pending_orders_empty){
    	set_internal_elevator_direction(DIR_STOP);
    }
}

//Direction functions
//this is ok
void Control::reverse_elevator_direction(){
    if(internal_elevator.get_dir() == DIR_UP){
        this->set_internal_elevator_direction(DIR_DOWN);
    }
    else{
        this->set_internal_elevator_direction(DIR_UP);
    }
}

//should be renamed
//this is ok
bool* Control::determine_button_lights_to_set(){
	bool lights_to_set[N_OUTSIDE_BUTTONS] = { 0 };
	std::map<std::string, boost::shared_ptr<Elevator>>::iterator it = external_elevators.begin();
	for (it; it != external_elevators.end(); it++){
		for(int i = 0; i < N_OUTSIDE_BUTTONS; i++){
			lights_to_set[i] |= it->second->get_order(i);
		}		
	}
	return lights_to_set;
}

//this is ok
void Control::set_order_button_lights(){
	bool *lights_to_set = determine_button_lights_to_set();
	for (int i = 0; i < N_OUTSIDE_BUTTONS; i++){
		hardware_thread->set_button_lamp(i, lights_to_set[i]);
	}
}


//Interthread communication functions//
//Hardware thread
//this is ok
void Control::deliver_button(int button, bool button_value){
    button_routine(button, button_value);
}

//this is ok
void Control::deliver_floor_sensor_signal(floor_t floor){
    floor_sensor_routine(floor);
}

//Communication thread    
//Update status of elevator
//this is ok
void Control::deliver_status(status_msg_t message, std::string ip){
    bool new_order_list[N_ORDER_BUTTONS] = {0};
    for(int i = 0; i < N_OUTSIDE_BUTTONS; i++){
        new_order_list[i] = message.order_list[i];
    }

	//Ip is not in map
	if (external_elevators.count(ip) == 0){
		add_new_external_elevator(message, ip);
	}
	else{
		external_elevators[ip]->exchange_order_list(new_order_list);
		external_elevators[ip]->set_dir(message.dir);
		external_elevators[ip]->set_previous_floor(message.floor);
	}

}

//Remove pending order or add order to current_orders
//this is ok
void Control::deliver_order(order_msg_t message, std::string ip){
	//Remove order from pending list
    if(message.clear_pending){
        pending_orders[message.order] = false; //may exchange this for set_order
    }
	//Add order to pending list
	else if (message.add_to_pending){
		pending_orders[message.order] = true;
	}
	//Add order to order list
    else{
		set_internal_elevator_order(message.order, true);
    }
}

//Remove useless elevator
//this is fantastic
void Control::report_useless_elevator(std::string ip){
    external_elevators.erase(ip);
}

//Timer functions
//this is ok
void Control::refresh_open_door_timer(){
	stranded_timer.cancel();
	hardware_thread->set_door_open_lamp(1);
	open_door_timer.cancel();
	open_door_timer.expires_from_now(boost::posix_time::seconds(DOOR_TIMEOUT));
	open_door_timer.async_wait([&](const boost::system::error_code &e){
		door_close(e); });
}





//this is ok
void Control::refresh_stranded_timer(){
	stranded_timer.cancel();
	stranded_timer.expires_from_now(boost::posix_time::seconds(STRANDED_TIMEOUT));
	stranded_timer.async_wait([&](const boost::system::error_code &e){
		elevator_stranded(e); });
}





//this is ok
void Control::door_close(const boost::system::error_code &e){

    if (e == boost::asio::error::operation_aborted) {return;}

    refresh_stranded_timer();
    hardware_thread->set_door_open_lamp(0);

    //Elevator stops if no more orders are waiting
    //Will move on to orders from pending list
    if (internal_elevator.is_order_list_empty()){
            pick_from_pending_orders();
    }
    //Will either enter stop mode
    //Or continue after pausing at floor
    hardware_thread->set_motor_direction(internal_elevator.get_dir());
}




//this is ok
void Control::elevator_stranded(const boost::system::error_code &e){

    if (e == boost::asio::error::operation_aborted) {return;}

	set_internal_elevator_direction(STRANDED);

	//Send all orders from outside to external elevators
	for (int i = 0; i < N_OUTSIDE_BUTTONS; i++){
		if (internal_elevator.get_order(i)){
			send_order_to_closest_elevator(i);
		}
	}
}



//Set functions
//Send only if values change!!
//Might put the three recurring lines in it's own function

//this is ok
void Control::set_internal_elevator_direction(direction_t dir){
	internal_elevator.set_dir(dir);

	if (dir == DIR_STOP || dir == STRANDED){
		stranded_timer.cancel();
	}

	//Send updated data to other threads
	hardware_thread->set_motor_direction(dir);
	communication_thread->update_status(internal_elevator.get_status()); //Should be implemented sometime
}

//this is ok
void Control::set_internal_elevator_order(int order, bool value){
	internal_elevator.set_order(order, value);
	
	hardware_thread->set_button_lamp(order, value);

	if(is_outside_order(order)){
		communication_thread->update_status(internal_elevator.get_status());
	}	
}

//this is ok
void Control::set_internal_elevator_floor(floor_t floor){
	status_msg_t msg;
	internal_elevator.set_previous_floor(floor);

	hardware_thread->set_floor_indicator(floor); //Might set this in hardware
	communication_thread->update_status(internal_elevator.get_status());
}

//this is ok but in a painful way
void Control::clear_orders_at_floor(floor_t floor){
	if (floor == FIRST){
		internal_elevator.set_order(FIRST_UP, false);
		set_internal_elevator_order(N_OUTSIDE_BUTTONS + FIRST, false);
	}
	else if (floor == FOURTH){
		internal_elevator.set_order(FOURTH_DOWN, false);
		set_internal_elevator_order(N_OUTSIDE_BUTTONS + FOURTH, false);
	}
	else{
		internal_elevator.set_order(floor * 2 - 1, false);
		internal_elevator.set_order(N_OUTSIDE_BUTTONS + floor, false);
		set_internal_elevator_order(floor * 2, false);
	}
}
