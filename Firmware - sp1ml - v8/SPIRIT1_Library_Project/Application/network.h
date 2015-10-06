#ifndef NETWORK
#define NETWORK


#include "stm32l1xx.h"
#include "spirit_msg.h"



typedef struct 
{
spirit_msg rx_fifo[MAX_SPIRIT_MSG] ;
spirit_msg tx_fifo[MAX_SPIRIT_MSG] ;
} Network ;


uint8_t fifo_is_full(spirit_msg* msg);
void send_spirit_msg(spirit_msg msg , Network* network);

#endif 