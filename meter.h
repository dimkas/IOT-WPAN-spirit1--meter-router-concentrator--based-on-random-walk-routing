#ifndef METER
#define METER

#include "stm32l1xx.h"
#include "network.h"

#define TIME_OUT 10000 

typedef enum {DATA_SENDING , WAIT_ACK , IDLE} meter_state ;

typedef struct 
{
spirit_msg  data ;
uint32_t time_out ;
meter_state   state ; 
} meter ;


#endif
