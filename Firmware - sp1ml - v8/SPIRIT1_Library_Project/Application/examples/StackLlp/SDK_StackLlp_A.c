/**
* @file    SDK_StackLlp_A.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of transmission of SPIRIT STack packets with auto Ack and auto ReTx features.
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
#elif SDK
#include "SPIRIT_SDK_Util.h"
#include "SDK_Common.h"
#ifdef USE_VCOM
#include "SDK_EVAL_VC_General.h"
#endif
#endif

//#define     PIGGYBACKING

/**
 * @addtogroup SDK_Examples
 * @{
 */

/**
 * @addtogroup SDK_StackLlp                     SDK STack LLP
 * @{
 */

/**
 * @addtogroup SDK_StackLlp_A                   SDK STack LLP A
 * @{
 */

/**
 * @defgroup StackLlp_A_Private_TypesDefinitions        STack LLP A Private TypesDefinitions
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup StackLlp_A_Private_Defines                 STack LLP A Private Defines
 * @{
 */

/*  Packet configuration parameters  */
#undef LENGTH_TYPE
#define LENGTH_TYPE                 	PKT_LENGTH_VAR
#undef LENGTH_WIDTH
#define LENGTH_WIDTH                	8

/*  Addresses configuration parameters  */
#undef MY_ADDRESS
#define MY_ADDRESS                  	0x44
#undef DESTINATION_ADDRESS
#define DESTINATION_ADDRESS        	0x34

/* LLP configuration parameters */
#define EN_AUTOACK                      S_DISABLE
#define EN_PIGGYBACKING             	S_DISABLE
#define MAX_RETRANSMISSIONS         	PKT_N_RETX_2


/**
 *@}
 */


/**
 * @defgroup StackLlp_A_Private_Macros                  STack LLP A Private Macros
 * @{
 */

/**
 *@}
 */

/**
 * @defgroup StackLlp_A_Private_Variables               STack LLP A Private Variables
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

/* Packet Stack structure fitting */
PktStackInit xStackInit={
  PREAMBLE_LENGTH,
  SYNC_LENGTH,
  SYNC_WORD,
  LENGTH_TYPE,
  LENGTH_WIDTH,
  CRC_MODE,
  CONTROL_LENGTH,
  EN_FEC,
  EN_WHITENING
};

/* Address structure fitting */
PktStackAddressesInit xAddressInit={
  EN_FILT_MY_ADDRESS,
  MY_ADDRESS,
  EN_FILT_MULTICAST_ADDRESS,
  MULTICAST_ADDRESS,
  EN_FILT_BROADCAST_ADDRESS,
  BROADCAST_ADDRESS
};

/* LLP structure fitting */
PktStackLlpInit xStackLLPInit ={
  EN_AUTOACK,
  EN_PIGGYBACKING,
  MAX_RETRANSMISSIONS
};

/* GPIO structure fitting */
SGpioInit xGpioIRQ={
  SPIRIT_GPIO_3,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_IRQ
};


FlagStatus xTxDoneFlag = RESET;
SpiritIrqs xIrqStatus;

#ifdef PIGGYBACKING
uint8_t vectcRxBuff[96], cRxData;
#endif

uint8_t vectcTxBuff[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
/**
 *@}
 */

/**
 * @defgroup StackLlp_A_Private_FunctionPrototypes              STack LLP A Private Function Prototypes
 * @{
 */

/**
 *@}
 */

/**
 * @defgroup StackLlp_A_Private_Functions                       STack LLP A Private Functions
 * @{
 */


/**
 * @brief  This function handles External lines 15 to 10 interrupt request.
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
    SpiritIrqGetStatus(&xIrqStatus);

    /* Check the SPIRIT TX_DATA_SENT IRQ flag */
    if(xIrqStatus.IRQ_TX_DATA_SENT || xIrqStatus.IRQ_MAX_RE_TX_REACH)
    {
      /* set the tx_done_flag to manage the event in the main() */
      xTxDoneFlag = SET;

      if(xIrqStatus.IRQ_TX_DATA_SENT)
        SdkEvalLedToggle(LED1);

      if(xIrqStatus.IRQ_MAX_RE_TX_REACH)
        SdkEvalLedToggle(LED2);
      
#ifdef USE_VCOM
      printf("N.Elem.TX.FIFO: %d\n\r", SpiritLinearFifoReadNumElementsTxFifo());
      printf("SEQ.NUM.: %d\n\r", SpiritPktCommonGetTransmittedSeqNumber());
      printf("NRETX: %d\n\n\r", SpiritPktStackGetNReTx());
#endif
      
#ifdef PIGGYBACKING
      /* Get the RX FIFO size */
      cRxData=SpiritLinearFifoReadNumElementsRxFifo();

      /* Read the RX FIFO */
      SpiritSpiReadLinearFifo(cRxData, vectcRxBuff);

#ifdef USE_VCOM
      /* print the received data */
      printf("A piggybacking data received: [");
      for(uint8_t i=0 ; i<cRxData ; i++)
        printf("%d ", vectcRxBuff[i]);
      printf("]\n\r");
#endif
#endif
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

  /* Spirit IRQ config */
  SpiritGpioInit(&xGpioIRQ);
  
  /* Spirit Radio config */
  SpiritRadioInit(&xRadioInit);
  
  /* Spirit Radio set power */
  SpiritRadioSetPALeveldBm(7,POWER_DBM);
  SpiritRadioSetPALevelMaxIndex(7);
  
  /* Spirit Packet config */
  SpiritPktStackInit(&xStackInit);
  SpiritPktStackLlpInit(&xStackLLPInit);

  /* Spirit IRQs enable */
  SpiritIrqDeInit(NULL);
  SpiritIrq(TX_DATA_SENT , S_ENABLE);
  SpiritIrq(RX_DATA_READY, S_ENABLE);
  SpiritIrq(MAX_RE_TX_REACH, S_ENABLE);

  EXTI_ClearITPendingBit(M2S_GPIO_3_EXTI_LINE);
  SdkEvalM2SGpioInit(M2S_GPIO_3,M2S_MODE_EXTI_IN);  

  /* payload length config */
  SpiritPktStackSetPayloadLength(20);

   /* enable SQI check */
  SpiritQiSetSqiThreshold(SQI_TH_0);
  SpiritQiSqiCheck(S_ENABLE);

  /* rx timeout config */
  SpiritTimerSetRxTimeoutMs(200.0);
  SpiritTimerSetRxTimeoutStopCondition(SQI_ABOVE_THRESHOLD);

  /* require ack from the receiver */
  SpiritPktStackRequireAck(S_ENABLE);

  /* IRQ registers blanking */
  SpiritIrqClearStatus();
  
#ifdef STM8L
  enableInterrupts();
#elif SDK
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0A,0x0A,ENABLE);
#endif
    
  /* infinite loop */
  while (1){

#ifdef USE_VCOM
    printf("A data to transmit: [");

    for(uint8_t i=0 ; i<20 ; i++)
      printf("%d ", vectcTxBuff[i]);
    printf("]\r\n");
#endif

    /* fit the TX FIFO */
    SpiritCmdStrobeFlushTxFifo();
    SpiritSpiWriteLinearFifo(20, vectcTxBuff);

    /* send the TX command */
    SpiritCmdStrobeTx();

    /* wait for TX done */
    while(!xTxDoneFlag);
    xTxDoneFlag = RESET;

    /* Delay after reset */
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
