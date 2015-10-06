/**
* @file    SDK_FifoHandler_B.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of reception of SPIRIT Basic packets with payload longer than 96 bytes.
* @details
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
*/

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"
#include "SPIRIT_Config.h"
#include "SDK_Configuration_Common.h"

#define USE_VCOM

#ifdef STM8L
#include "STM8L_Common.h"
#define XTAL_FREQUENCY          50000000
#define RANGE_TYPE              RANGE_EXT_NONE       //RANGE_EXT_SKYWORKS_169 or RANGE_EXT_SKYWORKS_868
#define FREQUENCY_OFFSET        0
/* max payload size */
#define PAYLOAD_LENGTH_FIX 100
#elif SDK
#include "SPIRIT_SDK_Util.h"
#include "SDK_Common.h"
#ifdef USE_VCOM
#include "SDK_EVAL_VC_General.h"
#endif
/* max payload size */
#define PAYLOAD_LENGTH_FIX 300
#endif

//#define COLERIDGE

/**
* @addtogroup SDK_Examples
* @{
*/

/**
* @addtogroup SDK_Fifo_Handler        SDK FIFO Handler
* @{
*/

/**
* @addtogroup SDK_Fifo_Handler_B              SDK FIFO Handler B
* @brief Device B configured as a receiver.
* @details This code explains how to configure a receiver for basic packets with payload longer than 96 bytes.
*
* The user can change the Basic packet configuration parameters editing the defines
* at the beginning of the file.
* @{
*/

/**
* @defgroup Fifo_Handler_B_Private_TypesDefinitions           FIFO Handler B Private TypesDefinitions
* @{
*/

/**
*@}
*/


/**
* @defgroup Fifo_Handler_B_Private_Defines                    FIFO Handler B Private Defines
* @{
*/

/*  Radio configuration parameters  */
//#undef DATARATE
//#define DATARATE                    1200
//#undef FREQ_DEVIATION
//#define FREQ_DEVIATION              20e3
//#undef BANDWIDTH
//#define BANDWIDTH                   50.5E3

#undef DATARATE
#define DATARATE                    38400
#undef FREQ_DEVIATION
#define FREQ_DEVIATION              20e3
#undef BANDWIDTH
#define BANDWIDTH                   100.5E3

//#undef DATARATE
//#define DATARATE                    100000
//#undef FREQ_DEVIATION
//#define FREQ_DEVIATION              50e3
//#undef BANDWIDTH
//#define BANDWIDTH                   325E3

//#undef DATARATE
//#define DATARATE                    250000
//#undef FREQ_DEVIATION
//#define FREQ_DEVIATION              127e3
//#undef BANDWIDTH
//#define BANDWIDTH                   540E3

//#undef DATARATE
//#define DATARATE                    500000
//#undef FREQ_DEVIATION
//#define FREQ_DEVIATION              150e3
//#undef BANDWIDTH
//#define BANDWIDTH                  750E3

/*  Packet configuration parameters  */
#undef LENGTH_TYPE
#define LENGTH_TYPE                 PKT_LENGTH_VAR
#undef LENGTH_WIDTH
#define LENGTH_WIDTH                12

/*  Addresses configuration parameters  */
#undef MY_ADDRESS         
#define MY_ADDRESS                  0x44
#undef DESTINATION_ADDRESS          
#define DESTINATION_ADDRESS         0x34



/**
*@}
*/


/**
* @defgroup Fifo_Handler_B_Private_Macros                             FIFO Handler B Private Macros
* @{
*/

/**
*@}
*/

/**
* @defgroup Fifo_Handler_B_Private_Variables                          FIFO Handler B Private Variables
* @{
*/

/**
* @brief Radio structure fitting
*/
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

/**
* @brief Packet Basic structure fitting
*/
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


/**
* @brief Address structure fitting
*/
PktBasicAddressesInit xAddressInit={
  EN_FILT_MY_ADDRESS,
  MY_ADDRESS,
  EN_FILT_MULTICAST_ADDRESS,
  MULTICAST_ADDRESS,
  EN_FILT_BROADCAST_ADDRESS,
  BROADCAST_ADDRESS
};


/**
* @brief GPIO IRQ structure fitting
*/
SGpioInit xGpioIRQ={
  SPIRIT_GPIO_3,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_IRQ
};

/**
* @brief Declare the Rx done flag
*/
FlagStatus xRxDoneFlag = RESET;


/**
* @brief IRQ status struct declaration
*/
SpiritIrqs xIrqStatus;


/**
* @brief Rx buffer declaration: how to store the received data
*/
uint8_t vectcRxBuff[PAYLOAD_LENGTH_FIX];

/**
* @brief Rx buffer index, residual and total bytes to be received
*/
uint16_t nRxIndex, nPayloadLength = 200;

/**
* @brief Almost full Tx FIFO threshold  (measured from the tail)
*/
uint8_t cThreholdRxFifoAF = 78;


/**
* @brief Rx data in FIFO
*/
uint8_t cRxDataLen;

/**
*@}
*/

/**
* @defgroup Fifo_Handler_B_Private_FunctionPrototypes                         FIFO Handler B Private FunctionPrototypes
* @{
*/

/**
*@}
*/


/**
* @defgroup Fifo_Handler_B_Private_Functions                                  FIFO Handler B Private Functions
* @{
*/

/**
* @brief  This function handles External interrupt request. In this application it is used
*         to manage the Spirit IRQ configured to be notified on the Spirit GPIO_3.
* @param  None
* @retval None
*/
#ifdef STM8L
INTERRUPT_HANDLER(M2S_GPIO_3_EXTI_IRQ_HANDLER,12)
#elif SDK
void M2S_GPIO_3_EXTI_IRQ_HANDLER(void)
#endif
{
  static uint16_t rx_pl=0;
  
  /* Check the flag status of EXTI line */
  if(EXTI_GetITStatus(M2S_GPIO_3_EXTI_LINE))
  {
    /* Clear the EXTI line flag */
    EXTI_ClearITPendingBit(M2S_GPIO_3_EXTI_LINE);
    
    /* Get the IRQ status */
    SpiritIrqGetStatus(&xIrqStatus);
    
    /* Check the RX_FIFO_ALMOST_FULL IRQ flag */
    if(xIrqStatus.IRQ_RX_FIFO_ALMOST_FULL)
    {
      if(rx_pl==0)
      {
#if LENGTH_TYPE == PKT_LENGTH_VAR 
        rx_pl=SpiritPktBasicGetReceivedPktLength();
#else
        rx_pl=nPayloadLength;
#endif
      }
      
      cRxDataLen = SpiritLinearFifoReadNumElementsRxFifo();
      
      /* Read the RX FIFO */
      SpiritSpiReadLinearFifo(cRxDataLen, &vectcRxBuff[nRxIndex]);
      nRxIndex+=cRxDataLen;
      
      if(nRxIndex>=rx_pl)
      {
        SpiritCmdStrobeFlushRxFifo();
        
        SpiritIrqClearStatus();
          
        /* update the number of received bytes */
        nRxIndex = rx_pl;
      
        /* set the Rx done flag */
        xRxDoneFlag = SET;
        rx_pl=0;
      }


    }
    /* Check the RX_DATA_READY IRQ flag */
    if(xIrqStatus.IRQ_RX_DATA_READY)
    {      
      SdkEvalLedToggle(LED2);
      
      cRxDataLen = SpiritLinearFifoReadNumElementsRxFifo();
      
      if(cRxDataLen!=0)
        /* Read the RX FIFO */
        SpiritSpiReadLinearFifo(cRxDataLen, &vectcRxBuff[nRxIndex]);
      
      SpiritCmdStrobeFlushRxFifo();
      
      /* update the number of received bytes */
      nRxIndex += cRxDataLen;
    
      /* set the Rx done flag */
      xRxDoneFlag = SET;
      rx_pl=0;
    }
    
    /* Check the SPIRIT RX_DATA_DISC IRQ flag */
    if(xIrqStatus.IRQ_RX_DATA_DISC)
    {
      SdkEvalLedToggle(LED1);
      
      /* RX command - to ensure the device will be ready for the next reception */
      SpiritCmdStrobeRx();
      
      rx_pl=0;
    }
  }
}

/**
* @brief  Debug printing function.
* @param  None
* @retval None
*/
void printDbgData(void)
{
  SpiritBool correct=S_TRUE;
  
  /* print the received data */
  printf("B data received: \n\r");
  
  for(uint16_t i=0 ; i<nRxIndex ; i++)
  {
#ifndef COLERIDGE
    printf("%d ", vectcRxBuff[i]);
#else
    printf("%c", vectcRxBuff[i]);
#endif
    if(vectcRxBuff[i] != (uint8_t)i)
      correct=S_FALSE;
  }
  printf("\n\n\r");
  
  if(correct)
    SdkEvalLedToggle(LED2);

  printf("\n\rAF Thr=%d\n\rPayload length=%d\n\n\r", cThreholdRxFifoAF,nRxIndex);
}


/**
* @brief  System main function.
* @param  None
* @retval None
*/
void main (void)
{
  SDK_SYSTEM_CONFIG();
  
#ifdef USE_VCOM
#ifdef STM8L
  SdkEvalComInit(115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
#elif SDK
  /* VC config */
  SdkEvalVCInit();
  while(bDeviceState != CONFIGURED);
#endif
#endif
  
  /* Spirit ON */
  SpiritEnterShutdown();
  SpiritExitShutdown();
  SpiritManagementWaExtraCurrent();
  
#ifdef STM8L
  /* Manually set the XTAL_FREQUENCY */
  SpiritRadioSetXtalFrequency(XTAL_FREQUENCY);    
  /* Initialize the frequency offset variable to compensate XTAL offset */
  xRadioInit.lFrequencyBase = xRadioInit.lFrequencyBase + FREQUENCY_OFFSET;
  /* Initialize the signals to drive the range extender application board */
  RANGE_EXT_INIT(RANGE_TYPE);
#elif SDK
  SpiritManagementIdentificationRFBoard();

  /* if the board has eeprom, we can compensate the offset calling SpiritManagementGetOffset
  (if eeprom is not present this fcn will return 0) */
  xRadioInit.lFrequencyBase = xRadioInit.lFrequencyBase + SpiritManagementGetOffset();
  
  /* Initialize the signals to drive the range extender application board */
  SpiritManagementRangeExtInit();  
#endif
  
  SdkEvalM2SGpioInit(M2S_GPIO_3,M2S_MODE_EXTI_IN);

  /* Spirit IRQ config */
  SpiritGpioInit(&xGpioIRQ);
  
  /* Spirit Radio config */
  SpiritRadioInit(&xRadioInit);
  
  /* Spirit Packet config */
  SpiritPktBasicInit(&xBasicInit);
  SpiritPktBasicFilterOnCrc(S_DISABLE);
  SpiritPktBasicAddressesInit(&xAddressInit);
  
  /* Spirit IRQs enable */
  SpiritIrqDeInit(&xIrqStatus);
  SpiritIrq(RX_DATA_DISC,S_ENABLE);
  SpiritIrq(RX_DATA_READY,S_ENABLE);
  
  /* enable SQI check */
  SpiritQiSetSqiThreshold(SQI_TH_0);
  SpiritQiSqiCheck(S_ENABLE);
  
  /* RX timeout config */
  SpiritTimerSetRxTimeoutMs(1500.0);
  SpiritTimerSetRxTimeoutStopCondition(SQI_ABOVE_THRESHOLD);
  
  /* IRQ registers blanking */
  SpiritIrqClearStatus();

  /* enable the GPIO 0 IRQ (MCU side) */
  EXTI_ClearITPendingBit(M2S_GPIO_3_EXTI_LINE);

#ifdef STM8L
  enableInterrupts();
#elif SDK
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0A,0x0A,ENABLE);
#endif
  
#ifdef COLERIDGE
  nPayloadLength = 402;
#endif
  
  /* infinite loop */
  while (1)
  {    
    /* reset the Rx buffer index (number of bytes received until now) */
    nRxIndex = 0;

    /* payload length config */
    SpiritPktBasicSetPayloadLength(nPayloadLength);
    
    /* .. set the almost full threshold and configure the associated IRQ */
    SpiritLinearFifoSetAlmostFullThresholdRx(96-cThreholdRxFifoAF);
    SpiritIrq(RX_FIFO_ALMOST_FULL,S_ENABLE);
    
    /* RX command */
    SpiritCmdStrobeRx();
    
    /* wait for RX done */
    while(!xRxDoneFlag);
    
    xRxDoneFlag = RESET;

#ifdef USE_VCOM
    printDbgData();
#endif
    
#ifndef COLERIDGE
    /* increment the payload length (overflowing it at PAYLOAD_LENGTH_FIX) */
    nPayloadLength = (nPayloadLength+1)%PAYLOAD_LENGTH_FIX;
#endif
  }
  
}



#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/**
*@}
*/

/**
*@}
*/

/**
*@}
*/

/**
*@}
*/


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
