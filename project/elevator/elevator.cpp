#include "elevator.h"
#include <cmath>

direction_t direction_of_order(int order);
int button_to_floor(int button);

void Elevator::timeout(){ 
	/*
	this->_dir = STRANDED; 
	// don't do anything if the timeout was pushed 
	//      (aborted and refreshed by refresh_timeout_timer() )
	if (e == asio::error::operation_aborted) { return; }

	// Handle the timeout
	std::cout << PROMPT "generic_thread timed out, we should probably deal with this and tell main something is wrong\n";
	return;
	*/
}

void Elevator::refresh_timer(){
	/*
	timeout_timer.cancel();
	timeout_timer.expires_from_now(posix_time::milliseconds(TIMEOUT_TIMER_DURATION_MS));
	timeout_timer.async_wait([&](const boost::system::error_code& e){
		// function to be run at timeout
		timeout(e); });
	return;
	*/
}

int Elevator::distance_from_order(int order){
	//elevator is stopped or headed in right direction
	if (direction_of_order(order) == this->_dir || this->_dir == DIR_STOP){
		return abs(this->_previous_floor - button_to_floor(order));
	}

	//Elevator is stranded or headed in the wrong direction
	else{
		return -1;
	}

}

//Orders going up are odd numbers while downward orders are even.
direction_t direction_of_order(int order){
	return static_cast<direction_t>(order % 2);
}

int button_to_floor(int button){
	return (button + 1) / 2;
}