/**
* @file    SDK_Sniff_B.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of reception of SPIRIT Basic packets in sniff mode.
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
#define FREQUENCY_OFFSET        (0)
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
* @addtogroup SDK_Sniff        SDK Sniff
* @{
*/

/**
* @addtogroup SDK_Sniff_B              SDK Sniff B
* @brief Device B configured as a receiver.
* @details This code explains how to configure a receiver for
* basic packets.
*
* The user can change the Basic packet configuration parameters editing the defines
* at the beginning of the file.
* @{
*/

/**
* @defgroup Sniff_B_Private_TypesDefinitions           Sniff B Private TypesDefinitions
* @{
*/

/**
*@}
*/


/**
* @defgroup Sniff_B_Private_Defines                    Sniff B Private Defines
* @{
*/

#undef DATARATE
#define DATARATE                    2400

#undef FREQ_DEVIATION
#undef BANDWIDTH
#define FREQ_DEVIATION              4800
#define BANDWIDTH                   16000
#define SPIRIT1_RX_TIMEOUT_DURATION     1

/**
*@}
*/


/**
* @defgroup Sniff_B_Private_Macros                             Sniff B Private Macros
* @{
*/

/**
*@}
*/

/**
* @defgroup Sniff_B_Private_Variables                          Sniff B Private Variables
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
* @brief IRQ status struct declaration
*/
SpiritIrqs xIrqStatus;

/**
* @brief Rx buffer declaration: how to store the received data
*/
uint8_t vectcRxBuff[96], cRxData;

/**
*@}
*/

/**
* @defgroup Sniff_B_Private_FunctionPrototypes                         Sniff B Private FunctionPrototypes
* @{
*/

/**
*@}
*/


/**
* @defgroup Sniff_B_Private_Functions                                  Sniff B Private Functions
* @{
*/

/* preamble length from typedef to number of bits */
#define PREAMBLE_LENGTH_BITS                            (((PREAMBLE_LENGTH>>3)+1)*8)

/* sync length from typedef to number of bits */
#define SYNC_LENGTH_BITS                                (((SYNC_LENGTH>>1)+1)*8)

/* LDC timer duration (ms) */
#define SPIRIT1_LDC_TIMER_DURATION                      ((1.0E3*(PREAMBLE_LENGTH_BITS - 16))/DATARATE)

/* SYNC timeout duration (ms) - add a time of two bytes more */
#define SYNC_TIMEOUT_DURATION                           ((1.0E3*(PREAMBLE_LENGTH_BITS + SYNC_LENGTH_BITS+10))/DATARATE)




/**
* @brief  This function handles Timer3 interrupt request. In this application it is used
*         to manage the MCU Timer expiration 
SYNC_TIMEOUT_DURATION purpose
stop Rx and go back in LDC mode
* @param  None
* @retval None
*/
#ifdef STM8L
INTERRUPT_HANDLER(TIM3_IRQHandler,21)
{
  if (TIM3_GetITStatus(TIM3_IT_Update))
  {
    TIM3_ClearITPendingBit(TIM3_IT_Update);
#elif SDK 
void TIM6_IRQHandler(void) 
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update))
  {
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
#endif
    SpiritIrq(RX_DATA_DISC,S_DISABLE);
    
    /* no SYNC IRQ raised before SYNC_TIMEOUT_DURATION => Stop Rx and stop TIM3 */ 
    SpiritCmdStrobeSabort();
    SpiritCmdStrobeFlushRxFifo();
    
    /* IRQ registers blanking */
    SpiritIrqClearStatus();
    
    /* enable LDC mode and start Rx */
    SpiritTimerLdcrMode(S_ENABLE);
    SpiritCmdStrobeRx();
    
    /* enable RSSI_ABOVE_TH_IRQ */
    SpiritIrq(RSSI_ABOVE_TH,S_ENABLE);    
    
#ifdef STM8L
    TIM3_ITConfig(TIM3_IT_Update, DISABLE);
    TIM3_Cmd(DISABLE);
    TIM3_ClearITPendingBit(TIM3_IT_Update);
#elif SDK       
    SdkEvalTimersState(TIM6, DISABLE);
#endif
  }
}


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
  /* Check the flag status of EXTI line */
  if(EXTI_GetITStatus(M2S_GPIO_3_EXTI_LINE)){
    /* Clear the EXTI line flag */
    EXTI_ClearITPendingBit(M2S_GPIO_3_EXTI_LINE);
    
    /* Get the IRQ status */
    SpiritIrqGetStatus(&xIrqStatus);
    
    /* Check the Spirit1 RSSI above threshold IRQ flag */
    if(xIrqStatus.IRQ_RSSI_ABOVE_TH)
    {     
      SpiritIrq(RSSI_ABOVE_TH, S_DISABLE);
      /* disable LDC mode to avoid LDC timer falling during block Reception*/
      SpiritTimerLdcrMode(S_DISABLE);
      
      /* start MCU Timer to detect SYNC : SYNC_TIMEOUT_DURATION */
#ifdef STM8L
      TIM3_SetCounter(0);
      TIM3_ITConfig(TIM3_IT_Update, ENABLE);
      TIM3_Cmd(ENABLE);
      TIM3_ClearITPendingBit(TIM3_IT_Update);
#elif SDK       
      SdkEvalTimersResetCounter(TIM6);
      SdkEvalTimersState(TIM6, ENABLE);
#endif
      
    }
    
    /* Check the SPIRIT IRQ_VALID_SYNC IRQ flag */
    if(xIrqStatus.IRQ_VALID_SYNC)
    {
      /* SYNC IRQ received => disable SYNC_TIMEOUT_DURATION timeout */
#ifdef STM8L
      TIM3_ITConfig(TIM3_IT_Update, DISABLE);
      TIM3_Cmd(DISABLE);
      TIM3_ClearITPendingBit(TIM3_IT_Update);
#elif SDK
      SdkEvalTimersState(TIM6, DISABLE);
      SdkEvalLedToggle(LED4);
#endif
      SpiritIrq(RX_DATA_DISC,S_ENABLE);
    }
    
    /* Check the SPIRIT RX_DATA_READY IRQ flag */
    if(xIrqStatus.IRQ_RX_DATA_READY)
    {

      /* Get the RX FIFO data */
      cRxData=SpiritLinearFifoReadNumElementsRxFifo();
      SpiritSpiReadLinearFifo(cRxData, vectcRxBuff);
      
#ifdef USE_VCOM
      float rssi = SpiritQiGetRssidBm();
      printf("rssi = %.1f\r\n",rssi);      

      printf("received %d data [",cRxData);
      for(uint32_t i=0;i<cRxData;i++)
        printf("%d, ",vectcRxBuff[i]);
      printf("]\r\n");
#endif      
      
      /* Flush the RX FIFO */
      SpiritCmdStrobeFlushRxFifo();
      
      /* toggle LED2 */
      SdkEvalLedToggle(LED2);      
      
      /* disable MCU Timer = large Rx timeout as Rx terminated */
#ifdef STM8L
      TIM3_ITConfig(TIM3_IT_Update, DISABLE);
      TIM3_Cmd(DISABLE);
      TIM3_ClearITPendingBit(TIM3_IT_Update);
#elif SDK       
      SdkEvalTimersState(TIM6, DISABLE);
#endif
      
      /* enable LDC mode and start Rx */
      SpiritIrqClearStatus();
      SpiritIrq(RSSI_ABOVE_TH,S_ENABLE);
      SpiritTimerLdcrMode(S_ENABLE);
      SpiritCmdStrobeRx();      
    }
    else if(xIrqStatus.IRQ_RX_DATA_DISC && !xIrqStatus.IRQ_RX_TIMEOUT)
    {
        
#ifdef USE_VCOM
      printf("pckt discarded!\n\r");
#endif
      
#ifdef STM8L
      SdkEvalLedToggle(LED2);
      TIM3_ITConfig(TIM3_IT_Update, DISABLE);
      TIM3_Cmd(DISABLE);
      TIM3_ClearITPendingBit(TIM3_IT_Update); 
#elif SDK
      SdkEvalLedToggle(LED3);
      SdkEvalTimersState(TIM6, DISABLE);
#endif
      /* IRQ registers blanking */
      SpiritIrqClearStatus();
      
      /* enable LDC mode and start Rx */
      SpiritTimerLdcrMode(S_ENABLE);
      SpiritCmdStrobeRx();
      
      /* enable RSSI_ABOVE_TH_IRQ */
      SpiritIrq(RSSI_ABOVE_TH,S_ENABLE);   
      SpiritIrq(RX_DATA_DISC,S_DISABLE);
    }
    
    
    
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
  
  /* initialize GPIO3 IRQ configuration */
  SdkEvalM2SGpioInit(M2S_GPIO_3,M2S_MODE_EXTI_IN);
  SdkEvalM2SGpioInit(M2S_GPIO_2,M2S_MODE_GPIO_IN);
  SdkEvalM2SGpioInit(M2S_GPIO_1,M2S_MODE_GPIO_IN);
  
  /* Spirit gpio config */
  SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_3, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP, SPIRIT_GPIO_DIG_OUT_IRQ});
  SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_2, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP, SPIRIT_GPIO_DIG_OUT_RX_STATE});
  SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_1, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP, SPIRIT_GPIO_DIG_OUT_SLEEP_OR_STANDBY});
  
  /* Spirit Radio config */
  SpiritRadioInit(&xRadioInit);
  
#ifdef STM8L
  enableInterrupts();
#elif SDK
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0A,0x0A,ENABLE);
#endif
  
  /* Spirit Packet config */
  SpiritPktBasicInit(&xBasicInit);
  SpiritPktBasicSetPayloadLength(20);
  
  /* Spirit IRQs enable */
  SpiritIrqDeInit(&xIrqStatus);
  SpiritIrq(RX_DATA_READY,S_ENABLE);
  SpiritIrq(RSSI_ABOVE_TH,S_ENABLE);
  SpiritIrq(VALID_SYNC,S_ENABLE);
  //SpiritIrq(RX_DATA_DISC,S_ENABLE);
  
  /* enable SQI check */
  SpiritQiSetSqiThreshold(SQI_TH_0);
  SpiritQiSqiCheck(S_ENABLE);
  
  /* RX timeout config => CS evaluation */
  SpiritTimerSetRxTimeoutMs(SPIRIT1_RX_TIMEOUT_DURATION);
  SpiritQiSetRssiThresholddBm(-50);
  SpiritTimerSetRxTimeoutStopCondition(RSSI_ABOVE_THRESHOLD);
  
  /* IRQ registers blanking */
  SpiritIrqClearStatus();
  
#ifdef STM8L 
  uint32_t n;
  n = (uint32_t)(SYNC_TIMEOUT_DURATION*CLOCK_FREQUENCY);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  TIM3_TimeBaseInit(TIM3_Prescaler_128, TIM3_CounterMode_Up, n/128);
  TIM3_UpdateRequestConfig(TIM3_UpdateSource_Global);
  TIM3_ITConfig(TIM3_IT_Update, ENABLE);
  TIM3_Cmd(ENABLE);
#elif SDK
  /* TIMER 6 config : SYNC_TIMEOUT_DURATION */
  SdkEvalTimersTimConfig_ms(TIM6, SYNC_TIMEOUT_DURATION);
#endif
  
  /* clear FIFO if needed */
  SpiritCmdStrobeFlushRxFifo();
  
  /* setup and start LDC mode */
  SpiritTimerSetWakeUpTimerMs(SPIRIT1_LDC_TIMER_DURATION);
  
  /* enable LDC mode and start Rx */
  SpiritTimerLdcrMode(S_ENABLE);
  SpiritCmdStrobeRx();
  
  /* infinite loop */
  for(;;){   
    
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
