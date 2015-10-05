#ifndef CONFIG
#define CONFIG

#if !defined(METER) && !defined(ROUTER) && !defined(CONCENTRATOR)
#error "You should choose node type !!!"
#endif


/*************************************** METER CONFIG *****************************************************/
#ifdef METER


/* network filter */
//#define ENABLE_NETWORK_FILTER

/*  Addresses configuration parameters  */
#undef EN_FILT_MY_ADDRESS
#define EN_FILT_MY_ADDRESS          S_ENABLE
#undef MY_ADDRESS
#define MY_ADDRESS                  	0x30  //20
#undef DESTINATION_ADDRESS
#define DESTINATION_ADDRESS        	0x10

#define PEREFFERED_ADRESS     0xff

#define NEED_ACK1 // NEED_ACK1 or NO_ACK1
#define MAX_ACK_RETRY 5  
#define ACK_TIME_OUT 200  // 1 == 100 ms
#define PERIOD_DELAY 150  //   1 == 100 ms  //200


#endif
/*************************************** CONCENTRATOR CONFIG *****************************************************/
#ifdef CONCENTRATOR

/* network filter */
//#define ENABLE_NETWORK_FILTER

/*  Addresses configuration parameters  */
#undef EN_FILT_MY_ADDRESS
#define EN_FILT_MY_ADDRESS          S_ENABLE
#undef MY_ADDRESS
#define MY_ADDRESS                  	0x00


#endif
/*************************************** ROUTER CONFIG *****************************************************/
#ifdef  ROUTER

/* network filter */
//#define ENABLE_NETWORK_FILTER

/*  Addresses configuration parameters  */
#undef EN_FILT_MY_ADDRESS
#define EN_FILT_MY_ADDRESS          S_DISABLE
#undef MY_ADDRESS
#define MY_ADDRESS                  	0x44
#define CONCENTRATOR_ADDRESS  0x00
#define SEUIL 95 // if the random number  > threshold then we forget the msg else we send it 


#endif


#endif
