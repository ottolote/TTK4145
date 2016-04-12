#ifndef _MESSAGE_DECODE_H
#define _MESSAGE_DECODE_H

//#include "elevator.h"
//#include "message_format.h"

//Encoded message type
typedef int encoded_msg_t;

/*
//Decoded message types
struct order_msg_t{ order_t order; int ack; };
struct status_msg_t{ order_t order_list[NUMBER_OF_BROADCASTED_ORDERS]; direction_t dir; floor_t floor; };

class Message_handler{
private:
	

public:
	//decoding
	static order_msg_t decode_order_message(encoded_msg_t encoded_message);
	static status_msg_t decode_status_message(encoded_msg_t encoded_message);
	
	//encoding
	static encoded_msg_t encode_order_message(order_msg_t order_msg);
	static encoded_msg_t encode_status_message(status_msg_t status_msg);

};
*/

#endif
