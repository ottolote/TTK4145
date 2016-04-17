#include "Control.hpp"
#include "Hardware.hpp"
#include "Communication.hpp"
#include "helper_functions.h"
#include "terminalcolors.h"
#include <iterator>

#define PROMPT "[" TCOLOR_PURPLE "Control" TCOLOR_NC "] : "



//Constructors
Control::Control()
    : stranded_timer(io, boost::posix_time::milliseconds(STRANDED_TIMEOUT)),
      dooor_timer(io, boost::posix_time::milliseconds(1000)),
      pending_orders(new bool[N_ORDER_BUTTONS]()){}







void Control::door_timeout(const boost::system::error_code &e) {
    if (e == boost::asio::error::operation_aborted) {return;}
    set_internal_elevator_direction(internal_elevator.get_dir());
    hardware->set_door_open_lamp(0);
    std::cout << PROMPT "door closed\n";
}





void Control::open_door() {
    hardware->set_door_open_lamp(1);
    dooor_timer.expires_from_now(boost::posix_time::milliseconds(1000));
    dooor_timer.async_wait([&](const boost::system::error_code &e) {
        door_timeout(e); });
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

    std::cout<< PROMPT "entering button_routine, button: " 
        << button << " is " << button_value << std::endl;

    if (button == STOP_BUTTON){
       stop_button_routine(button_value);
    }
    else{
        order_button_routine(button, button_value);
    }
}

//Elevator will hold if a button at floor is continously pressed
//this is ok
void Control::order_button_routine(int button, bool button_value){
    bool order_is_for_current_floor = internal_elevator.get_previous_floor() == button_to_floor(button);
                                      //&& direction_of_order(button) == internal_elevator.get_dir();

    if(order_is_for_current_floor){

        std::cout << PROMPT "order is for current floor\n";
        //button is pressed
        if(button_value){
            hardware->set_door_open_lamp(1);
            dooor_timer.cancel();
        }
        //button is released
        else{
            open_door();
        }
    }
    //Order wasn't to current floor and should be sent to closest elevator
    else{
        if(button_value) {
            this->send_order_to_closest_elevator(button);
        }
    }
}

//this is ok
void Control::stop_button_routine(bool button_value){
    //Stop button triggered
    if (button_value){
        set_internal_elevator_direction(STRANDED); //Unavailable for orders until timeout
        bool empty_order_list[N_ORDER_BUTTONS] = { 0 };
        internal_elevator.exchange_order_list(empty_order_list);
        hardware->set_stop_lamp(1);
    }
    else{
         //stop button release
    }
}

//this is ok
void Control::floor_sensor_routine(floor_t floor){
    //stranded_timer.cancel();
    //start_stranded_timer(); //Motor still working if floor sensor changes
    
    //Update status variables to this floor
    if (floor != NONE){
        set_internal_elevator_floor(floor);

        //Current floor is in order list
        std::cout << PROMPT "is_order_in_l: " << internal_elevator.is_current_floor_in_order_list(floor) <<" floor: " << std::endl;
        std::cout << PROMPT "is_order_in_d: " << is_order_in_direction(floor, internal_elevator.get_dir()) << " - direction: " << internal_elevator.get_dir() << std::endl;
        if (
                internal_elevator.is_current_floor_in_order_list(floor) ) {
                //|| !is_order_in_direction(floor, internal_elevator.get_dir()  )){
            std::cout << PROMPT "Found order in order list\n";
            hardware->set_motor_direction(DIR_STOP);
            open_door();
            clear_orders_at_floor(floor);

            //No more orders. Check pending list
            if(internal_elevator.is_order_list_empty()){
                pick_from_pending_orders();
            }


        }
    }
}


//Algorithm functions//
//Working but should probably clean this up
//Maybe use find_closest_external_elevator() or something

//This one should work but it all looks kinda horrible
bool Control::order_in_path(int order){
    if(internal_elevator.get_dir() == DIR_UP){
        return internal_elevator.get_previous_floor() - button_to_floor(order) < 0;
    }
    else if(internal_elevator.get_dir() == DIR_DOWN){
        return internal_elevator.get_previous_floor() - button_to_floor(order) > 0;
    }
    else{
        return true;
    }
}


std::string Control::find_closest_elevator(int order){

    //Is inside order
    if (order >= N_OUTSIDE_BUTTONS){
        std::cout << PROMPT "Found an internal button press";
        //Elevator headed to order
        if(order_in_path(order)){
            return "Internal elevator";
        }
        else{
            return "";
        }
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



/*
void Control::send_order_to_closest_elevator(int order, std::string closest_elevator_ip){
    if(closest_elevator_ip.empty()){
        add_to_external_orders(order);
    }
    else if(closest_elevator_ip == "Internal elevator"){
        set_internal_elevator_order(order)
    }
    else{
        communication->send_order(order, closest_elevator_ip);
    }


}
*/





//this is ok
void Control::send_order_to_closest_elevator(int order){
    // responds to stop

    std::cout << PROMPT "sending order to closest elevator\n";

    std::string closest_elevator_ip = find_closest_elevator(order);
    std::cout << PROMPT "Closest elevator " << closest_elevator_ip << std::endl;
    //All elevators going in the wrong direction
    //send to pending list
    if (closest_elevator_ip.empty()){
        if (order == STOP_BUTTON) {
            std::cout << PROMPT << ERROR "Attempted to send STOP to closest elevator\n";
        }else {
            std::cout << PROMPT "---------------No elevators could handle order\n";
            pending_orders[order] = true;
            print_pending_orders();

            communication->send_pending_order(order, true); 
        }
    }
    
    //Internal elevator is closest or order came from inside
    else if (closest_elevator_ip == "Internal elevator"){
        if (order == STOP_BUTTON) {
            std::cout << PROMPT << ERROR "Attempted to send STOP to closest elevator\n";
        }else {
            std::cout << PROMPT "closest elevator is self, setting internal order: "
                << order << std::endl;


            set_internal_elevator_order(order, true);

            internal_elevator.print_current_orders();
        }
    }
//
//    //An external elevator is closest
    else{
        if (order == STOP_BUTTON) {
            std::cout << PROMPT << ERROR "Attempted to send STOP to closest elevator\n";
        }else {
            communication->send_order(order, closest_elevator_ip);
        }
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
    for(int order = 0; order < N_ORDER_BUTTONS; order++){
        //Take all pending orders headed in opposite direction
        if(pending_orders[order] && direction_of_order(order) != internal_elevator.get_dir() || (pending_orders[order] && order > N_OUTSIDE_BUTTONS)){
            std::cout << PROMPT "Found pending orders\n";
            pending_orders_empty = false;
            pending_orders[order] = false; //Clear order from pending list
            communication->send_pending_order(order, false); 
            set_internal_elevator_order(order, true);
            reverse_elevator_direction();
        }

    }
    print_pending_orders();
    internal_elevator.print_current_orders();
    //No more orders in pending list
    if(pending_orders_empty && internal_elevator.is_order_list_empty()){
        std::cout << PROMPT "No more orders what so ever\n";
        set_internal_elevator_direction(DIR_STOP);
    }
}





//Direction functions
//this is ok
void Control::reverse_elevator_direction(){
    std::cout << PROMPT "reversing direction\n";
    if(internal_elevator.get_dir() == DIR_UP){
        this->internal_elevator.set_dir(DIR_DOWN);
    }
    else{
        this->internal_elevator.set_dir(DIR_UP);
    }
}







//should be renamed
//this is ok
void Control::determine_button_lights_to_set(){
    bool lights_to_set[N_OUTSIDE_BUTTONS] = { 0 };
    std::map<std::string, boost::shared_ptr<Elevator>>::iterator it = external_elevators.begin();
    for (it; it != external_elevators.end(); it++){
        for(int i = 0; i < N_OUTSIDE_BUTTONS; i++){
            lights_to_set[i] |= it->second->get_order(i);
        }        
    }
    for (int i = 0; i < N_OUTSIDE_BUTTONS; i++){
        hardware->set_button_lamp(i, lights_to_set[i]);
    }
}












//Interthread communication functions//
//Hardware thread
//this is ok
void Control::deliver_button(int button, bool button_value){
    io.post( [=] { button_routine(button, button_value); });
}





//this is ok
void Control::deliver_floor_sensor_signal(floor_t floor){
    io.post( [=] { floor_sensor_routine(floor); });
}






//set_button_lights in this function
void Control::external_status_update_routine(status_msg_t message, std::string ip) {
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




//Communication thread    
//Update status of elevator
//this is ok
void Control::deliver_status(status_msg_t message, std::string ip){
    io.post( [=] { external_status_update_routine(message, ip); });
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










//this is ok
void Control::refresh_stranded_timer(){
    stranded_timer.expires_from_now(boost::posix_time::milliseconds(STRANDED_TIMEOUT));
    std::cout << PROMPT "starting stranded timer\n";
    stranded_timer.async_wait([&](const boost::system::error_code &e){
        elevator_stranded(e); });
}














//this is ok
void Control::elevator_stranded(const boost::system::error_code &e){


    if (e == boost::asio::error::operation_aborted) {return;}

    set_internal_elevator_direction(STRANDED);

    std::cout << PROMPT << ERROR "elevator is now STRANDED and cannot move\n";

    //Send all orders from outside to external elevators
//    for (int i = 0; i < N_OUTSIDE_BUTTONS; i++){
//        if (internal_elevator.get_order(i)){
//            send_order_to_closest_elevator(i);
//        }
//    }
}









//Set functions
//Send only if values change!!
//Might put the three recurring lines in it's own function

//this is ok
void Control::set_internal_elevator_direction(direction_t dir){
    std::cout << "Setting internal direction to " << dir << std::endl;
    internal_elevator.set_dir(dir);

    if (dir == DIR_STOP || dir == STRANDED){
        //stranded_timer.cancel();
    }

    //Send updated data to other threads
    hardware->set_motor_direction(dir);
    communication->update_internal_status(internal_elevator.get_status()); //Should be implemented sometime
}






//this is ok
void Control::set_internal_elevator_order(int order, bool value){

    internal_elevator.set_order(order, value);

    if(internal_elevator.get_dir() == DIR_STOP){
        head_to_order(order);
    }

    hardware->set_button_lamp(order, value);
    if(is_outside_order(order)){
        communication->update_internal_status(internal_elevator.get_status());
    }    
}





//this is ok
void Control::set_internal_elevator_floor(floor_t floor){
    internal_elevator.set_previous_floor(floor);

    hardware->set_floor_indicator(floor); //Might set this in hardware
    communication->update_internal_status(internal_elevator.get_status());
}






//this is ok but in a painful way
void Control::clear_orders_at_floor(floor_t floor){
    if (floor == FIRST){
        internal_elevator.set_order(FIRST_UP, false);
        hardware->set_button_lamp(FIRST_UP, false);
    }
    else if (floor == FOURTH){
        internal_elevator.set_order(FOURTH_DOWN, false);
        hardware->set_button_lamp(FOURTH_DOWN, false);
    }
    else if (internal_elevator.get_dir() == DIR_UP){
        internal_elevator.set_order(floor * 2, false); //up
        hardware->set_button_lamp(floor*2, false);
    }
    else if (internal_elevator.get_dir() == DIR_DOWN){
        internal_elevator.set_order(floor * 2 - 1, false);   //down
        hardware->set_button_lamp(floor*2 - 1, false);
    }
    set_internal_elevator_order(N_OUTSIDE_BUTTONS + floor, false); //internal
    hardware->set_button_lamp(N_OUTSIDE_BUTTONS + floor, false); //internal
}






void Control::head_to_order(int order){
    //order to the same floor
    if(internal_elevator.get_previous_floor() - button_to_floor(order) == 0){ //This case might result in slightly weird behaviour
        set_internal_elevator_direction(DIR_STOP);
        internal_elevator.set_order(order, false);
    }
    //order is below
    else if(internal_elevator.get_previous_floor() - button_to_floor(order) > 0){
        set_internal_elevator_direction(DIR_DOWN);
    }
    //order is above
    else{
        set_internal_elevator_direction(DIR_UP);
    }
}






void Control::print_pending_orders() {
    std::cout << PROMPT "------------------------pending orders---------------------------\n";
    for (int i = 0; i<N_ORDER_BUTTONS; i++) {
        std::cout << pending_orders[i] << "\t";
    }
    std::cout << "\n1U\t2D\t2U\t3D\t3U\t4D\tF1\tF2\tF3\tF4\n----------------------------------------------------------------------------\n" << std::endl;

}






//returns -1 if no orders in direction
bool Control::is_order_in_direction(floor_t current_floor, direction_t dir) {


    int iterator_change;
    int start_looking;
    int stop_looking;

    if (dir == DIR_UP) {
        if (internal_elevator.get_order(FOURTH_DOWN)) {
            return true;
        }

        iterator_change = -1;
        start_looking = FOURTH;
        stop_looking = current_floor + 1;

    } else if (dir == DIR_DOWN) {
        if (internal_elevator.get_order(FIRST_UP)) {
            return true;
        }

        iterator_change = 1;
        start_looking = FIRST;
        stop_looking = current_floor - 1;
    } else {
        std::cout << PROMPT 
            << ERROR "wrong arguments passed to is_order_in_direction\n";
        return -1;
    }


    for (int i = start_looking; i<stop_looking; i+= iterator_change) {
//        if (internal_elevator.) {
//            return true;
//        }
    }
    return false;
}



