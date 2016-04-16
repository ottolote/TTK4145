#include "message_handler.hpp"
#include <iostream>


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





type_id_t Message_handler::read_message_type(encoded_msg_t encoded_message) {
    return extract_bits(encoded_message, TYPE_MASK, TYPE_INDEX);
}


//Helper functions
int extract_bits(encoded_msg_t message, int mask, int index){
    return (message & mask) >> index;
}

int set_bit(encoded_msg_t message, int bit_index) {
    return ( message | (1 << bit_index) );
}


int clear_bit(encoded_msg_t message, int bit_index) {
    return ( message & ~(1 << bit_index) );
}


bool Message_handler::read_bit(encoded_msg_t message, int bit_index) {
    return ( message >> bit_index) & 1;
}



bool Message_handler::is_ack_message(encoded_msg_t msg) {
    return extract_bits(msg, ACK_MASK, ACK_INDEX);;
}

encoded_msg_t Message_handler::add_ack_to_message(encoded_msg_t encoded_message) {
    return (set_bit(encoded_message, ACK_INDEX));
}

encoded_msg_t Message_handler::remove_ack_from_message(encoded_msg_t encoded_message) {
    return (clear_bit(encoded_message, ACK_INDEX));
}




unsigned int Message_handler::checksum(encoded_msg_t encoded_message) {
    unsigned int checksum = 0;
    for (unsigned int bitnum = 0; bitnum < STATUS_FIELD_WIDTH; bitnum++) {
        checksum += read_bit(encoded_message, bitnum)*bitnum*5 + 156; //value chosen arbitrarily
    }
    //std::cout << "checksum (checksum() ): " << checksum << std::endl;
    checksum = checksum %  32;
    //std::cout << "checksum % 32: " << checksum << std::endl;
    return checksum;
}






encoded_msg_t Message_handler::add_checksum(encoded_msg_t encoded_message) {

    if(read_checksum(encoded_message)) {
        std::cout << "Warning: add_checksum: message " << encoded_message 
            << " already has a checksum\n";
        // clear bits
        encoded_message = (encoded_message << (16-STATUS_FIELD_WIDTH));
        encoded_message = (encoded_message >> (16-STATUS_FIELD_WIDTH));
    }

    return (encoded_message 
            + (Message_handler::checksum(encoded_message) << (STATUS_FIELD_WIDTH)));
}





unsigned int Message_handler::read_checksum(encoded_msg_t encoded_message) {
    //std::cout << encoded_message << std::endl;
    unsigned int checksum = (encoded_message >> (STATUS_FIELD_WIDTH));
    //std::cout << encoded_message << std::endl;
    //std::cout << checksum << std::endl;
    return checksum;
}




