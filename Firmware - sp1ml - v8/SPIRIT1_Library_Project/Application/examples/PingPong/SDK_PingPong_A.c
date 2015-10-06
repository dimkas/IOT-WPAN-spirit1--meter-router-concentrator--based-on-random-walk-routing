/**
* @file    SDK_PingPong_A.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of transmission and reception of SPIRIT STack packets.
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
* @addtogroup SDK_Examples
* @{
*/

/**
* @addtogroup SDK_PingPong                             SDK Ping Pong
* @{
*/

/**
* @addtogroup SDK_PingPong_A                           SDK Ping Pong A
* @brief Device A configured as the first ping-pong player.
* @details This code configures the device as ping-pong player making it switches from the Tx to the Rx state and
* vice-versa.
*
* The user can change the configuration parameters editing the defines at the beginning of the file.
* @{
*/

/**
* @defgroup PingPong_A_Private_TypesDefinitions                Ping Pong A Private TypesDefinitions
* @{
*/

/**
*@}
*/


/**
* @defgroup PingPong_A_Private_Defines                         Ping Pong A Private Defines
* @{
*/

/*  Addresses configuration parameters  */
#undef EN_FILT_MY_ADDRESS
#define EN_FILT_MY_ADDRESS          	S_ENABLE
#undef EN_FILT_BROADCAST_ADDRESS
#define EN_FILT_BROADCAST_ADDRESS       S_ENABLE
#undef MY_ADDRESS
#define MY_ADDRESS                  	0x44
#undef DESTINATION_ADDRESS
#define DESTINATION_ADDRESS        	0x34

/* LLP configuration parameters */
#define EN_AUTOACK                      S_DISABLE
#define EN_PIGGYBACKING             	S_DISABLE
#define MAX_RETRANSMISSIONS         	PKT_DISABLE_RETX

/**
*@}
*/


/**
* @defgroup PingPong_A_Private_Macros                          Ping Pong A Private Macros
* @{
*/

/**
*@}
*/

/**
* @defgroup PingPong_A_Private_Variables                       Ping Pong A Private Variables
* @{
*/


/**
* @brief Packet Stack structure fitting
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

/**
* @brief Address structure fitting
*/
PktStackAddressesInit xAddressInit={
 EN_FILT_MY_ADDRESS,
 MY_ADDRESS,
 EN_FILT_MULTICAST_ADDRESS,
 MULTICAST_ADDRESS,
 EN_FILT_BROADCAST_ADDRESS,
 BROADCAST_ADDRESS
};

/**
* @brief LLP structure fitting
*/
PktStackLlpInit xStackLLPInit ={
 EN_AUTOACK,
 EN_PIGGYBACKING,
 MAX_RETRANSMISSIONS
};

/**
* @brief GPIO structure fitting
*/
SGpioInit xGpioIRQ={
 SPIRIT_GPIO_2,
 SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
 SPIRIT_GPIO_DIG_OUT_IRQ
};

/**
* @brief Flags declarations
*/
FlagStatus xRxDoneFlag = RESET,
xTxDoneFlag=RESET;

/**
* @brief IRQ status struct declaration
*/
SpiritIrqs xIrqStatus;

/**
* @brief Ball buffer declaration: data to transmit
*/
uint8_t vectcBallBuff[20]={'A','A','A','D','A','V','A','A','A','A','A','A','A','A','A','A','A','A','A','A'} , cRxData;

/**
* @brief Tx counter declaration
*/
uint8_t cTxCounter=0;

/**
*@}
*/

/**
* @defgroup PingPong_A_Private_FunctionPrototypes                      Ping Pong A Private FunctionPrototypes
* @{
*/

/**
*@}
*/


/**
* @defgroup PingPong_A_Private_Functions                               PingPong A Private Functions
* @{
*/

/**
* @brief  This function handles External interrupt request (associated with Spirit GPIO 3).
* @param  None
* @retval None
*/
void M2S_GPIO_2_EXTI_IRQ_HANDLER(void)
{
 /* control the EXTI flag */
 if(EXTI_GetFlagStatus(M2S_GPIO_2_EXTI_LINE))
 {
   /* read the IRQ status bytes */
   SpiritIrqGetStatus(&xIrqStatus);
   
   /* set flags according to the raised IRQ*/
   if(xIrqStatus.IRQ_TX_DATA_SENT)
   {
     xTxDoneFlag = SET;
     SdkEvalLedToggle(LED1);
   }
   if(xIrqStatus.IRQ_RX_DATA_READY)
   {
     xRxDoneFlag = SET;
     SdkEvalLedToggle(LED2);
   }
   if(xIrqStatus.IRQ_RX_DATA_DISC)
   {
     SpiritCmdStrobeRx();
     SdkEvalLedToggle(LED3);
   }
   /* clear the EXTI flag */
   EXTI_ClearFlag(M2S_GPIO_2_EXTI_LINE);
 }
}


/**
* @brief  This function handles External interrupt request (associated with the KEY_BUTTON).
* @param  None
* @retval None
*/
//void KEY_BUTTON_EXTI_IRQ_HANDLER(void)
//{
//  /* Use this ISR to corrupt the data flow in order to stop the test */
//  if(EXTI_GetFlagStatus(KEY_BUTTON_EXTI_LINE))
//  {
//    vectcBallBuff[14]=3;
//    SdkEvalLedToggle(LED3);
//    EXTI_ClearFlag(KEY_BUTTON_EXTI_LINE);
//  }
//}


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
//  SdkEvalComInit(115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_Tx);
//#elif SDK
//  /* VC config */
//  SdkEvalVCInit();
//  while(bDeviceState != CONFIGURED);
//#endif
//#endif

 /* Micro EXTI config */
 //SdkEvalPushButtonInit(BUTTON_KEY, BUTTON_MODE_EXTI);
 
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

 /* Initialize the signals to drive the range extender application board */
 //SpiritManagementRangeExtInit();  
 
 SdkEvalM2SGpioInit(M2S_GPIO_2,M2S_MODE_EXTI_IN);
 SdkEvalM2SGpioInterruptCmd(M2S_GPIO_2,0x0A,0x0A,ENABLE);
 
 /* Spirit IRQ config */
 SpiritGpioInit(&xGpioIRQ);
 
 /* Spirit Radio config */
 SpiritRadioInit(&xRadioInit);
 
 /* Spirit Radio set power */
 SpiritRadioSetPALevelMaxIndex(7);
 SpiritRadioSetPALeveldBm(7,POWER_DBM);
 
 /* Spirit Packet config */
 SpiritPktStackInit(&xStackInit);
 SpiritPktStackAddressesInit(&xAddressInit);
 SpiritPktStackLlpInit(&xStackLLPInit);
 
 /* Spirit IRQs enable */
 SpiritIrqDeInit(NULL);
 SpiritIrq(TX_DATA_SENT, S_ENABLE);
 SpiritIrq(RX_DATA_READY, S_ENABLE);
 SpiritIrq(RX_DATA_DISC, S_ENABLE);
 
 /* payload length config */
 SpiritPktStackSetPayloadLength(20);
 
 /* destination address */
 SpiritPktStackSetDestinationAddress(DESTINATION_ADDRESS);
 
 /* rx timeout config */
 SET_INFINITE_RX_TIMEOUT();
 SpiritTimerSetRxTimeoutStopCondition(SQI_ABOVE_THRESHOLD);
 
 
 /* IRQ registers blanking */
 SpiritIrqClearStatus();
 
 /* infinite loop */
 while (1){
   
   /* fit the TX FIFO */
   SpiritCmdStrobeFlushTxFifo();
   SpiritSpiWriteLinearFifo(20, vectcBallBuff);
       
   /* send the TX command */
   SpiritCmdStrobeTx();
   
   /* wait for TX done */
   while(!xTxDoneFlag);
   xTxDoneFlag = RESET;
   //SdkDelayMs(100);
 

   
   /* RX command */
//   SpiritCmdStrobeRx();
//   
//   /* wait for data received */
//   while(!xRxDoneFlag);
//   xRxDoneFlag=RESET;
//   
//   /* when rx data ready read the number of received bytes */
//   cRxData=SpiritLinearFifoReadNumElementsRxFifo();
//   
//   /* read the RX FIFO */
//   SpiritSpiReadLinearFifo(cRxData, vectcBallBuff);
//   SpiritCmdStrobeFlushRxFifo();
//   
//   /*  A simple way to control if the received data sequence is correct.
//   Use a local block.
//   */
//   {
//     SpiritBool correct=S_TRUE;
     
     /* control all the received bytes and verify that they are sequential
     numbers from 1 to 20. If one of them is not an expected value then
     stop the communication.
     */
     for(uint8_t i=0 ; i<cRxData ; i++)
       if(vectcBallBuff[i] != i+1)
         //correct=S_FALSE;
     
//#ifdef USE_VCOM
//      /* send messages via COM */
//      if(correct)
//      {
//        printf("A Rx data: [");
//        for(uint8_t i=0 ; i<cRxData ; i++)
//          printf("%d ", vectcBallBuff[i]);
//        printf("]\r\n");
//      }
//      else
//      {
//        printf("data error! ping pong stopped\n\r");
//      }
//#endif
     
     /* stop communication if an incorrect sequence has been received */
//      if(!correct)
//      {
//        //SdkEvalLedOn(LED4);
//        
//        while(1);
//      }
 //  }
   
   /* pause between RX and TX */
   //
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
