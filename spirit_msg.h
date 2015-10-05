#ifndef SPIRIT_MSG
#define SPIRIT_MSG

#include "stm32l1xx.h"
#include "spirit_driver.h"

#define DATA_LENGTH 20 
#define MAX_SPIRIT_MSG 5
#define MAX_ROUTER 5

typedef enum {ACK , NO_ACK , NEED_ACK , COMMAND } Type ;
typedef enum {EMPTY , RX_MSG  , READY_TO_SEND , SENDING  } msg_state ;  // ps : after sending msg state switch to empty
                                                            // SENDING  : send msg until xTxDoneFlag 
typedef struct 
{
uint8_t src ;
uint8_t dst ;
uint8_t preffered_adresse ; // enable/disable routing
uint8_t seq ; //sequence number
Type type  ;
uint8_t data[DATA_LENGTH];
uint8_t routeurs[MAX_ROUTER] ; // to describe the route 
msg_state state ;
} spirit_msg ;

void send_spirit_msg_to_buffer(spirit_msg * msg); 
void get_spirit_msg_from_buffer(spirit_msg * msg); 
void routine_network (spirit_msg * msg , spirit_msg * rx_msg);
void init_network (spirit_msg * msg , spirit_msg * rx_msg);

#endif


