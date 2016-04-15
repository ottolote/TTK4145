#pragma once

#include "elevator_properties.h"
#include "message_format.h"

//Things that probably should be moved//
//************************************//


#define TYPE_INDEX 0
#define TYPE_MASK 1

//Order type message
//Index
#define ORDER_INDEX 1
#define ACK_INDEX 4

//Masks
#define ORDER_MASK 14
#define ACK_MASK 16

//Status type message
//Index
#define ORDER_LIST_INDEX 1 
#define DIR_INDEX 7
#define FLOOR_INDEX 9

//Masks
#define ORDER_LIST_MASK 1 
#define DIR_MASK 7
#define FLOOR_MASK 9


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
