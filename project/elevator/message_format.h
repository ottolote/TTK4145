#ifndef _MESSAGE_FORMAT_H
#define _MESSAGE_FORMAT_H

#include <cstdint>
#include <boost/shared_ptr.hpp>
#include "elevator_properties.h"

/*
 *
 * TODO
 *
 * - Add bit for clear/set pending
 *
 *
 */

/************Message format***************
Order
0b|x|xxx|x|x|x|
   t  o  a c a
   y  r  c l d
   p  d  k e d
   e  e    a p
      r    r n

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

#define ORDER_MESSAGE_TYPE 1
#define STATUS_MESSAGE_TYPE 0

//Encoded message type
typedef uint16_t encoded_msg_t;

//Decoded message types
struct order_msg_t { outside_buttons_t order; int ack; int clear_pending; int add_to_pending; };

class status_msg_t 
{ 
    public:
        status_msg_t(): order_list( new bool[N_ORDER_BUTTONS]() ) {};
        boost::shared_ptr<bool[N_OUTSIDE_BUTTONS]> order_list;
        direction_t dir;
        floor_t floor; 
};


#endif
