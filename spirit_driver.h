#ifndef SPIRIT_DRIVER
#define  SPIRIT_DRIVER

#include "stm32l1xx.h"
#include "SDK_EVAL_Config.h"
#include "SPIRIT_Config.h"
#include "SDK_Configuration_Common.h"
#include "SPIRIT_SDK_Util.h"
#include "SDK_Common.h"
#include "config.h"


/* CSMA configuration parameters */
#define PERSISTENT_MODE_EN              S_DISABLE
#define CS_PERIOD                       TBIT_TIME_64
#define CS_TIMEOUT                      TCCA_TIME_3
#define MAX_NB                          5
#define BU_COUNTER_SEED                 0xFA21
#define CU_PRESCALER                    32
#define RSSI_THR -60

/*   Payload Length   */
#define PAYLOAG_LENGTH  30


typedef enum {IDLE , RX , TX , WAIT_TX_FLAG , RECEIVED_DATA_READY } spirit_stat ;

typedef struct 
{
  uint8_t  vectcTxBuff[96] ; // Tx buffer declaration: data to transmit
uint8_t cTxdata ; // size of data to send 
uint8_t  vectcRxBuff[96] ; // Rx buffer declaration: where to store the received data
uint8_t cRxData; // size of received data 
FlagStatus xTxDoneFlag , xRxDoneFlag ; // Tx RX done flag declaration

CsmaInit xCsmaInit ; // CSMA structure fitting
SGpioInit xGpioIRQ ;  //  GPIO structure fitting 
SpiritIrqs xIrqStatus; // IRQ status struct declaration
PktBasicAddressesInit xAddressInit ; // Address structure fitting
PktBasicInit xBasicInit   ;  // Packet Basic structure fitting
SRadioInit xRadioInit;  // Radio structure fitting

spirit_stat state ;
}
spirit_driver ;

void routine_spirit_driver (spirit_driver* spirit);
void Init_spirit_driver (spirit_driver* spirit);

void set_destination_adresse(uint8_t cAddress);
void set_my_adresse(uint8_t cAddress);

#endif