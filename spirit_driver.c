#include "spirit_driver.h"

/* Radio structure fitting */
SRadioInit xRadioInit = {
  XTAL_OFFSET_PPM,
  BASE_FREQUENCY,
  CHANNEL_SPACE,
  CHANNEL_NUMBER,
  MODULATION_SELECT,
  DATARATE,
  FREQ_DEVIATION,
  BANDWIDTH
};

/* Packet Basic structure fitting */
PktBasicInit xBasicInit={
  PREAMBLE_LENGTH,
  SYNC_LENGTH,
  SYNC_WORD,
  LENGTH_TYPE,
  LENGTH_WIDTH,
  CRC_MODE,
  CONTROL_LENGTH,
  EN_ADDRESS,
  EN_FEC,
  EN_WHITENING
};

/* Address structure fitting */
PktBasicAddressesInit xAddressInit={
  EN_FILT_MY_ADDRESS,
  MY_ADDRESS,
  EN_FILT_MULTICAST_ADDRESS,
  MULTICAST_ADDRESS,
  EN_FILT_BROADCAST_ADDRESS,
  BROADCAST_ADDRESS
};

/* GPIO structure fitting */
SGpioInit xGpioIRQ={
  SPIRIT_GPIO_2,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_IRQ
};


/* CSMA structure fitting */
CsmaInit xCsmaInit={
  PERSISTENT_MODE_EN,
  CS_PERIOD,
  CS_TIMEOUT,
  MAX_NB,
  BU_COUNTER_SEED,
  CU_PRESCALER
};

/* set destination adresse */
void set_destination_adresse(uint8_t cAddress)
{
SpiritPktCommonSetDestinationAddress(cAddress);
}


/* spirit driver init  */
void Init_spirit_driver (spirit_driver* spirit)
{
  spirit->state=RX;
  spirit->cTxdata=PAYLOAG_LENGTH;
  spirit->xRxDoneFlag=RESET;
  spirit->xTxDoneFlag=RESET;
  spirit->xRadioInit=xRadioInit;
  spirit->xBasicInit=xBasicInit;
  spirit->xAddressInit=xAddressInit;
  spirit->xGpioIRQ=xGpioIRQ;
  spirit->xCsmaInit=xCsmaInit;

 SDK_SYSTEM_CONFIG();

   
  /* Spirit ON */
  SpiritEnterShutdown();
  SpiritExitShutdown();
  SpiritManagementWaExtraCurrent();
  SpiritManagementIdentificationRFBoard();
  
  /* if the board has eeprom, we can compensate the offset calling SpiritManagementGetOffset
  (if eeprom is not present this fcn will return 0) */
  xRadioInit.lFrequencyBase = xRadioInit.lFrequencyBase + SpiritManagementGetOffset();
  
  /* Initialize the signals to drive the range extender application board */
  SpiritManagementRangeExtInit();  

  SdkEvalM2SGpioInit(M2S_GPIO_2,M2S_MODE_EXTI_IN);
  
  /* Spirit IRQ config */
  SpiritGpioInit(&spirit->xGpioIRQ);
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_2,0x0A,0x0A,ENABLE);

  /* Spirit Radio config */
  SpiritRadioInit(&spirit->xRadioInit);
  
  /* Spirit Radio set power */
  SpiritRadioSetPALeveldBm(7,POWER_DBM);
  SpiritRadioSetPALevelMaxIndex(7);
  
  /* Spirit Packet config */
  SpiritPktBasicInit(&spirit->xBasicInit);
  SpiritPktBasicAddressesInit(&spirit->xAddressInit);
  
  /* Spirit CSMA config */
  SpiritCsmaInit(&spirit->xCsmaInit);
  SpiritCsma(S_ENABLE);
  SpiritQiSetRssiThresholddBm(RSSI_THR);
  
  /* Spirit IRQs enable */
  SpiritIrqDeInit(NULL);
  SpiritIrq(TX_DATA_SENT , S_ENABLE);
  SpiritIrq(MAX_BO_CCA_REACH , S_ENABLE);
  SpiritIrq(RX_DATA_DISC,S_ENABLE);
  SpiritIrq(RX_DATA_READY,S_ENABLE);
  
  /* payload length config */
  SpiritPktStackSetPayloadLength(spirit->cTxdata);
  
    ////////////////////////////////////////////////////
 
    /* enable SQI check */
  SpiritQiSetSqiThreshold(SQI_TH_0);
  SpiritQiSqiCheck(S_ENABLE);
  
    /* rx timeout config */
  SpiritTimerSetRxTimeoutMs(1000.0);
  SpiritTimerSetRxTimeoutStopCondition(SQI_ABOVE_THRESHOLD);
 ////////////////////////////////////////////////////////////// 
  
  /* destination address */
  SpiritPktStackSetDestinationAddress(DESTINATION_ADDRESS);
  
  /* IRQ registers blanking */
  SpiritIrqClearStatus();  
    
}

void routine_spirit_driver (spirit_driver* spirit)
{
      switch (spirit->state)
      {
      case TX :
                spirit->xTxDoneFlag=RESET;
                
                SpiritCsma(S_ENABLE); 
                  SpiritCmdStrobeSabort();
                 /* fit the TX FIFO */
                SpiritCmdStrobeFlushTxFifo();
                SpiritSpiWriteLinearFifo(spirit->cTxdata, spirit->vectcTxBuff);
                /* send the TX command */
                SpiritCmdStrobeTx();
                spirit->state=WAIT_TX_FLAG;
      break;
      case WAIT_TX_FLAG :
                /* wait for TX done */
                if (spirit->xTxDoneFlag)
                {  
                  spirit->state=RX; 
                SpiritCsma(S_DISABLE);
                }
      break;
      case RX : 
                SpiritCsma(S_DISABLE);
                
                  SpiritCmdStrobeSabort();
                SpiritCmdStrobeRx();
                spirit->state=IDLE; 

                /*CSMA is an optional feature that can be enabled when performing transmission. Please
                note that CSMA must not be enabled when the transceiver is in receive mode. CSMA
                cannot be used in conjuction with link layer protocol (see Section 9.7.5) features such as     
                automatic acknowledgment and automatic retransmission. */
        
      break;
      case RECEIVED_DATA_READY : // no thing to do here  : wait for superior layer  to get msg from the RX buffer
      break;
      case IDLE :
        
      break;
      }

}

