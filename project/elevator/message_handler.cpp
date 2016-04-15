#include "message_handler.h"

int extract_bits(encoded_msg_t message, int mask, int index);

status_msg_t Message_handler::decode_status_message(encoded_msg_t encoded_message){
    status_msg_t status_msg;
    status_msg.dir = static_cast<direction_t>(extract_bits(encoded_message, DIR_MASK, DIR_INDEX));
    status_msg.floor = static_cast<floor_t>(extract_bits(encoded_message, FLOOR_MASK, FLOOR_INDEX));

    int order_bits = extract_bits(encoded_message, ORDER_LIST_MASK, ORDER_LIST_INDEX);

    //iterate through order list
    for (int i = 0; i < N_OUTSIDE_BUTTONS; i++){
        status_msg.order_list[i] = static_cast<outside_buttons_t>(extract_bits(order_bits, 1 << i, i));
    }
    return status_msg;
}

order_msg_t Message_handler::decode_order_message(encoded_msg_t encoded_message){
    order_msg_t order_msg;
    order_msg.ack = static_cast<outside_buttons_t>(extract_bits(encoded_message, ACK_MASK, ACK_INDEX));
    order_msg.order = static_cast<outside_buttons_t>(extract_bits(encoded_message, ORDER_MASK, ORDER_INDEX));

    return order_msg;
}

encoded_msg_t Message_handler::encode_status_message(status_msg_t status_msg){
    int order_bits = 0;

    //make order bits
    for (int i = 0; i < N_OUTSIDE_BUTTONS; i++){
            order_bits |= status_msg.order_list[i] << i;
    }

    return (STATUS_MESSAGE_TYPE << TYPE_INDEX | status_msg.floor << FLOOR_INDEX) | (status_msg.dir << DIR_INDEX) | (order_bits << ORDER_LIST_INDEX);
}

encoded_msg_t Message_handler::encode_order_message(order_msg_t order_msg){
    return  (ORDER_MESSAGE_TYPE << TYPE_INDEX) | (order_msg.order << ORDER_INDEX);
}



type_id_t read_message_type(encoded_msg_t encoded_message) {
    return extract_bits(encoded_message, TYPE_MASK, TYPE_INDEX);
}



//Helper functions
int extract_bits(encoded_msg_t message, int mask, int index){
    return (message & mask) >> index;
}


