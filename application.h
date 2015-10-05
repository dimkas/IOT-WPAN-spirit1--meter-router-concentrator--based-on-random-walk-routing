#ifndef APPLICATION
#define APPLICATION

#include "stm32l1xx.h"
#include "network.h"

/* device type */
#if !defined(METER) && !defined(ROUTER) && !defined(CONCENTRATOR)
#error "You should choose node type !!!"
#endif

#ifdef METER

typedef enum {PRIODIC , APRIODIC   } meter_type ;
typedef enum {BUILD_MSG , SEND_DATA , WAIT_ACK  } meter_state ;

typedef struct 
{
meter_type type ;
spirit_msg msg; // used to save meters data  
Network network ;
uint8_t ack_retry ;
uint16_t ack_time_out ; //  time out value
__IO uint16_t ack_time; // timer for ACK 
uint16_t period_delay ; // period delay between two msg 
__IO uint16_t period_time; // timer for the delay between two msg 
meter_state   state ; 
} Meter ;

void init_application(Meter * meter );
void routine_application(Meter * meter );
#endif
#ifdef CONCENTRATOR


typedef enum {LISTENING , SEND_DATA_TO_SERVER , SEND_ACK  } concentrator_state ;

typedef struct 
{
Network network ;
concentrator_state state ;
} Concentrator ;

void init_application(Concentrator * concentrator );
void routine_application(Concentrator * concentrator );

#endif
#ifdef  ROUTER

typedef enum {LISTENING1 , GET_RANDOM , WAIT , SEND_MSG  } router_state ;

typedef struct 
{
uint8_t random ;
uint8_t seuil ;
__IO uint32_t time_to_wait; // timer   
Network network ;
router_state state ;
} Router  ;

void init_application(Router * router );
void routine_application(Router * router );

#endif

#endif
