#ifndef _MESSAGE_FORMAT_H
#define _MESSAGE_FORMAT_H

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
   t	o   d   f
   y    r   i   l
   p    d   r   o
   e    e       o
        r       r
		l
		i
		s
		t
******************************************/

//Things that probably should be moved//
typedef enum order_t { FIRST_UP, SECOND_DOWN, SECOND_UP, THIRD_DOWN, THIRD_UP, FOURTH_DOWN };
#define NUMBER_OF_BROADCASTED_ORDERS 6
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

#endif
