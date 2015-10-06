/**
* @file    SDK_BasicGeneric_A.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of transmission of SPIRIT Basic packets.
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
* @addtogroup SDK_Examples SDK Examples
* @{
*/


/**
* @addtogroup SDK_Basic_Generic        SDK Basic Generic
* @{
*/

/**
* @addtogroup SDK_Basic_Generic_A                                      SDK Basic Generic A
* @brief Device A configured as a transmitter.
* @details This code explains how to configure and manage
* in the simpler way a transmitter of basic packets.
*
* The user can change the Basic packet configuration parameters editing the defines at the beginning of the file.
* @{
*/

/**
* @defgroup Basic_Generic_A_Private_TypesDefinitions                   Basic Generic A Private TypesDefinitions
* @{
*/

/**
*@}
*/


/**
* @defgroup Basic_Generic_A_Private_Defines                            Basic Generic A Private Defines
* @{
*/

/**
*@}
*/


/**
* @defgroup Basic_Generic_A_Private_Macros                                     Basic Generic A Private Macros
* @{
*/

/**
*@}
*/

/**
* @defgroup Basic_Generic_A_Private_Variables                                  Basic Generic A Private Variables
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
* @brief GPIO structure fitting
*/
SGpioInit xGpioIRQ={
  SPIRIT_GPIO_3,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_IRQ
};


/**
* @brief Declare the Tx done flag
*/
FlagStatus xTxDoneFlag = RESET;


/**
* @brief IRQ status struct declaration
*/
SpiritIrqs xIrqStatus;


/**
* @brief Tx buffer declaration: data to transmit
*/
//uint8_t vectcTxBuff[20]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
uint8_t vectcTxBuff[20]={4,5,4,4,5,6,7,8,9,10,11,4,13,14,4,4,17,4,19,4};


/**
*@}
*/

/**
* @defgroup Basic_Generic_A_Private_FunctionPrototypes                                         Basic Generic A Private FunctionPrototypes
* @{
*/

/**
*@}
*/


/**
* @defgroup Basic_Generic_A_Private_Functions                                                  Basic Generic A Private Functions
* @{
*/

/**
* @brief  This function handles External interrupt request (associated with Spirit GPIO 3).
* @param  None
* @retval None
*/

#ifdef STM8L
INTERRUPT_HANDLER(M2S_GPIO_3_EXTI_IRQ_HANDLER,12)
#elif SDK
void M2S_GPIO_3_EXTI_IRQ_HANDLER(void)
#endif
{
  /* Check the flag status of EXTI line */
  if(EXTI_GetITStatus(M2S_GPIO_3_EXTI_LINE))
  {   
    /* Get the IRQ status */
    SpiritIrqGetStatus(&xIrqStatus);
    
    /* Check the SPIRIT TX_DATA_SENT IRQ flag */
    if(xIrqStatus.IRQ_TX_DATA_SENT)
    {
      /* set the tx_done_flag to manage the event in the main() */
      xTxDoneFlag = SET;
      
      /* toggle LED1 */
      SdkEvalLedToggle(LED1);
    }
    
    /* Clear the EXTI line flag */
    EXTI_ClearITPendingBit(M2S_GPIO_3_EXTI_LINE);
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
  
//#ifdef USE_VCOM
//#ifdef STM8L
//  SdkEvalComInit(115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
//#elif SDK
//  /* VC config */
//  SdkEvalVCInit();
//  while(bDeviceState != CONFIGURED);
//#endif
//#endif
  
  /* Spirit ON */
  SpiritEnterShutdown();
  SpiritExitShutdown();
  SpiritManagementWaExtraCurrent();
  
//#ifdef STM8L
//  /* Manually set the XTAL_FREQUENCY */
//  SpiritRadioSetXtalFrequency(XTAL_FREQUENCY);    
//  /* Initialize the frequency offset variable to compensate XTAL offset */
//  xRadioInit.lFrequencyBase = xRadioInit.lFrequencyBase + FREQUENCY_OFFSET;
//  /* Initialize the signals to drive the range extender application board */
//  RANGE_EXT_INIT(RANGE_TYPE);
//#elif SDK
  SpiritManagementIdentificationRFBoard();
  
  /* if the board has eeprom, we can compensate the offset calling SpiritManagementGetOffset
  (if eeprom is not present this fcn will return 0) */
  xRadioInit.lFrequencyBase = xRadioInit.lFrequencyBase + SpiritManagementGetOffset();
  
  /* Initialize the signals to drive the range extender application board */
  SpiritManagementRangeExtInit();  
//#endif

  /* uC IRQ config */
  SdkEvalM2SGpioInit(M2S_GPIO_3,M2S_MODE_EXTI_IN);
  
  /* Spirit IRQ config */
  SpiritGpioInit(&xGpioIRQ);  
  
  /* uC IRQ enable */
//#ifdef STM8L
//  enableInterrupts();
//#elif SDK
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0A,0x0A,ENABLE);
//#endif
  
  /* Spirit Radio config */
  SpiritRadioInit(&xRadioInit);
    
  /* Spirit Radio set power */
  SpiritRadioSetPALeveldBm(7,POWER_DBM);
  SpiritRadioSetPALevelMaxIndex(7);
  
  /* Spirit Packet config */
  SpiritPktBasicInit(&xBasicInit);
  SpiritPktBasicAddressesInit(&xAddressInit);
  
  /* Spirit IRQs enable */
  SpiritIrqDeInit(NULL);
  SpiritIrq(TX_DATA_SENT , S_ENABLE);
  
  /* payload length config */
  SpiritPktBasicSetPayloadLength(20);
  
  /* destination address.
  By default it is not used because address is disabled, see struct xAddressInit*/
  SpiritPktBasicSetDestinationAddress(0x35);

  /* IRQ registers blanking */
  SpiritIrqClearStatus();
  
  /* infinite loop */
  while (1){   
//#ifdef USE_VCOM
//    printf("A data to transmit: [");
//    
//    for(uint8_t i=0 ; i<20 ; i++)
//      printf("%d ", vectcTxBuff[i]);
//    printf("]\n\r");
//#endif
    
    /* fit the TX FIFO */
    SpiritCmdStrobeFlushTxFifo();
    SpiritSpiWriteLinearFifo(20, vectcTxBuff);
    
    /* send the TX command */
    SpiritCmdStrobeTx();
    
    /* wait for TX done */
    while(!xTxDoneFlag);
    xTxDoneFlag = RESET;

    /* pause between two transmissions */
    SdkDelayMs(500);
    
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
