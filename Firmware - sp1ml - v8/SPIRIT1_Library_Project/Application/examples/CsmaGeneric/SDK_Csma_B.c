/**
* @file    SDK_Csma_B.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of configuration SPIRIT CSMA.
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

//#define USE_VCOM

#ifdef STM8L
#include "STM8L_Common.h"
#define XTAL_FREQUENCY          50000000
#define RANGE_TYPE              RANGE_EXT_NONE       //RANGE_EXT_SKYWORKS_169 or RANGE_EXT_SKYWORKS_868
#define FREQUENCY_OFFSET        0
#elif SDK
#include "SPIRIT_SDK_Util.h"
#include "SDK_Common.h"
#ifdef USE_VCOM
#include "SDK_EVAL_VC_General.h"
#endif
#endif


/**
* @addtogroup SDK_Csma_B                       SDK CSMA B
* @brief Device B configured as a transmitter with the CSMA feature active.
* @details This code explains how to configure a device in CSMA mode in order to avoid
* collisions when attempting to access the communication channel.
* The devices A and B have the same code in this example.
*
* The used is allowed to change the configuration parameters editing the defines at the beginning
* of the file.
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


/*
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


/*
* @brief GPIO structure fitting
*/
SGpioInit xGpioIRQ={
  SPIRIT_GPIO_2,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_IRQ
};


#ifdef SDK
void M2S_GPIO_2_EXTI_IRQ_HANDLER(void)
#endif
{
  /* Check the flag status of EXTI line */
  if(EXTI_GetITStatus(M2S_GPIO_2_EXTI_LINE)){
    
    /* Get the IRQ status */
    SpiritIrqGetStatus(&xIrqStatus);

    /* Check the SPIRIT RX_DATA_DISC IRQ flag */
    if(xIrqStatus.IRQ_RX_DATA_DISC)
    {
      SdkEvalLedToggle(LED1);
      
      /* RX command */
      SpiritCmdStrobeRx();
      
    }
    /* Check the SPIRIT RX_DATA_READY IRQ flag */
    if(xIrqStatus.IRQ_RX_DATA_READY)
    {      
      /* Get the RX FIFO size */
      cRxData=SpiritLinearFifoReadNumElementsRxFifo();
      
      /* Read the RX FIFO */
      SpiritSpiReadLinearFifo(cRxData, vectcRxBuff);
      
      /* Flush the RX FIFO */
      SpiritCmdStrobeFlushRxFifo();      
      
      SdkEvalLedToggle(LED2);
	    
//#ifdef USE_VCOM
//      printf("B data received: ..........");
//#endif
  
      /* RX command */
      SpiritCmdStrobeRx();
      
    }
    
    /* Clear the EXTI line flag */
    EXTI_ClearITPendingBit(M2S_GPIO_2_EXTI_LINE);
  }
  
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

  SdkEvalM2SGpioInit(M2S_GPIO_2,M2S_MODE_EXTI_IN);

  /* Spirit IRQ config */
  SpiritGpioInit(&xGpioIRQ);

#ifdef SDK
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_2,0x0A,0x0A,ENABLE);
#endif
  
  /* Spirit Radio config */
  SpiritRadioInit(&xRadioInit);
  
  /* Spirit Packet config */
  SpiritPktBasicInit(&xBasicInit);
  SpiritPktBasicAddressesInit(&xAddressInit);
  
  /* Spirit IRQs enable */
  SpiritIrqDeInit(&xIrqStatus);
  SpiritIrq(RX_DATA_DISC,S_ENABLE);
  SpiritIrq(RX_DATA_READY,S_ENABLE);
  
  /* payload length config */
  SpiritPktStackSetPayloadLength(20);
  
  /* enable SQI check */
  SpiritQiSetSqiThreshold(SQI_TH_0);
  SpiritQiSqiCheck(S_ENABLE);
  
  /* rx timeout config */
  SpiritTimerSetRxTimeoutMs(1000.0);
  SpiritTimerSetRxTimeoutStopCondition(SQI_ABOVE_THRESHOLD);
  
  /* IRQ registers blanking */
  SpiritIrqClearStatus();

  /* RX command */
  SpiritCmdStrobeRx();
  
  /* infinite loop */
  while (1){
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
