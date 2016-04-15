#ifndef _MESSAGE_FORMAT_H
#define _MESSAGE_FORMAT_H

#include <cstdint>
#include "elevator_properties.h"

/************Message format***************
Order
0b|x|xxx|x|
   t  o  a
   y  r  c
   p  d  k
   e  e
      r

Status
0b|x|xxxxxx|xx|xx|
   t    o   d   f
   y    r   i   l
   p    d   r   o
   e    e       o
        r       r
        l
        i
        s
        t
******************************************/
//Encoded message type
typedef uint16_t encoded_msg_t;

//Decoded message types
struct order_msg_t { outside_buttons_t order; int ack; };
struct status_msg_t { outside_buttons_t order_list[N_OUTSIDE_BUTTONS]; direction_t dir; floor_t floor; };

#endif
