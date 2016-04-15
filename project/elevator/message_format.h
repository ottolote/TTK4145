#pragma once

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
   t	o   h   f
   y    r   e   l
   p    d	a   o
   e    e	d	o
		r	i	r
		l   n
		i   g
		s
		t
******************************************/
//Encoded message type
typedef int encoded_msg_t;

//Decoded message types
struct order_msg_t { outside_buttons_t order; int ack; };
struct status_msg_t { bool order_list[N_OUTSIDE_BUTTONS]; direction_t dir; floor_t floor; };

