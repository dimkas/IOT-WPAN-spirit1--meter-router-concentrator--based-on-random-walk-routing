/**
* @file    SDK_LDCR_B.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of reception of SPIRIT Basic packets in LDCR mode.
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
#define LED3    LED2 /* the STM8L discovery doesn't have the LED3*/
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
* @addtogroup SDK_LDCR                         SDK LDCR
* @{
*/

/**
* @addtogroup SDK_LDCR_B                       SDK LDCR B
* @brief Device B configured as a receiver.
* @details This code shows a synchronization method between two devices in order to obtain a low
* duty cycle communication and, as a matter of fact, save power. This code runs on the receiver side.
*
* The user is allowed to change the algorithm parameters editing the defines at the beginning of the file.
* @{
*/

/**
* @defgroup LDCR_B_Private_TypesDefinitions            LDCR B Private TypesDefinitions
* @{
*/

/**
*@}
*/


/**
* @defgroup LDCR_B_Private_Defines                     LDCR B Private Defines
* @{
*/

/*  Addresses configuration parameters  */
#undef MY_ADDRESS                  
#define MY_ADDRESS                  0x44
#undef DESTINATION_ADDRESS         
#define DESTINATION_ADDRESS         0x34

/**
*@}
*/


/**
* @defgroup LDCR_B_Private_Macros                              LDCR B Private Macros
* @{
*/


/**
*@}
*/

/**
* @defgroup LDCR_B_Private_Variables                           LDCR B Private Variables
* @{
*/


/**
* @brief Packet Basic structure fitting
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
* @brief GPIO for Rx test signal structure fitting
*/
SGpioInit xGpio1Rx=
{
  SPIRIT_GPIO_1,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_RX_STATE
};

/**
* @brief GPIO for Sleep test signal structure fitting
*/
SGpioInit xGpio0Sleep=
{
  SPIRIT_GPIO_0,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_SLEEP_OR_STANDBY
};

/**
* @brief Rx buffer declaration: how to store the received data
*/
uint8_t vectcRxBuff[96], cRxData;



/**
* @brief Flags declarations
*/
FlagStatus xRxDataReadyFlag=RESET,
xFirstReception=RESET;


/**
* @brief IRQ status struct declaration
*/
SpiritIrqs xIrqStatus;

/**
*@}
*/

/**
* @defgroup LDCR_B_Private_FunctionPrototypes                          LDCR B Private FunctionPrototypes
* @{
*/

/**
*@}
*/


/**
* @defgroup LDCR_B_Private_Functions                                   LDCR B Private Functions
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
  if(EXTI_GetITStatus(M2S_GPIO_3_EXTI_LINE)){
    
    /* Get the IRQ status */
    SpiritIrqGetStatus(&xIrqStatus);
    
    /* Check the SPIRIT RX_DATA_READY IRQ flag */
    if(xIrqStatus.IRQ_RX_DATA_READY)
    {
      /* Get the RX FIFO size */
      cRxData = SpiritLinearFifoReadNumElementsRxFifo();
      
      /* Read the RX FIFO */
      SpiritSpiReadLinearFifo(cRxData, vectcRxBuff);
      
#ifdef USE_VCOM
      /* print the received data */
      printf("B data received: [");
      for(uint8_t i=0 ; i<cRxData ; i++)
        printf("%d ", vectcRxBuff[i]);
      printf("]\r\n");
#endif
      /* Set the rx data ready flag */
      xRxDataReadyFlag=SET;
      
      SdkEvalLedToggle(LED2);
    }
    if(xIrqStatus.IRQ_RX_DATA_DISC)
    {
      printf("B data discarded\n\r");
      SdkEvalLedToggle(LED3);
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

  /* Reset Spirit registers to default */
  SpiritCmdStrobeSres();
  
  SdkEvalM2SGpioInit(M2S_GPIO_3,M2S_MODE_EXTI_IN);

  /* Spirit IRQ config */
  SpiritGpioInit(&xGpioIRQ);
  
  /* Configure some output signals (to be probed with a scope to verify) */
  SpiritGpioInit(&xGpio1Rx);
  SpiritGpioInit(&xGpio0Sleep);
  
  /* Spirit Radio config */
  SpiritRadioInit(&xRadioInit);
  SpiritCalibrationRco(S_ENABLE);

  /* Spirit Packet config */
  SpiritPktBasicInit(&xBasicInit);
  
  /* IRQ registers blanking */
  SpiritIrqClearStatus();
  
#ifdef STM8L
  enableInterrupts();
#elif SDK
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0A,0x0A,ENABLE);
#endif
  
  SpiritQiSetSqiThreshold(SQI_TH_0);
  SpiritQiSqiCheck(S_ENABLE);
  
  /* Spirit IRQs enable */
  SpiritIrqDeInit(&xIrqStatus);
  SpiritIrq(RX_DATA_READY,S_ENABLE);
  SpiritIrq(RX_DATA_DISC,S_ENABLE);
  
  /* Payload length config */
  SpiritPktBasicSetPayloadLength(20);
  
  /* RX timeout config */
  SET_INFINITE_RX_TIMEOUT();
  SpiritTimerSetRxTimeoutStopCondition(SQI_ABOVE_THRESHOLD);
  
  float fWuTimer;
  uint8_t cWuPrescaler,cWuCounter,cWuReloadCounter;
  
  /* Set the wake Up event every WAKEUP_TIMER ms */
  SpiritTimerSetWakeUpTimerMs(WAKEUP_TIMER);
  
  /* Get the counter and the prescaler of the WU timer */
  SpiritTimerGetWakeUpTimer(&fWuTimer,&cWuCounter,&cWuPrescaler);
  
  /*
  Compute the rephasing timer to be reloaded on sync.
  This value will be equal to: WAKE_UP_TIMER - (PREAMB_TIME + SYNC_TIME) - GUARD_TIME,
  where:
  - (PREAMB_TIME + SYNC_TIME) is the time needed to transmit preamble and sync.
  - GUARD_TIME is a security bound to make the Rx awake before the Tx.
  */
  float fReloadMs = WAKEUP_TIMER-1000.0*(float)(SpiritPktBasicGetPreambleLength()+SpiritPktBasicGetSyncLength())*8.0/DATARATE-10.0;
  
  /* In order to have not lack of accuracy it is recommended that the reload timer
  has the same prescaler of the WakeUp timer.
  So compute the counter using the WU prescaler previously computed.
  */
  cWuReloadCounter=(uint8_t)(fReloadMs*(float)SpiritTimerGetRcoFrequency()/1000/(cWuPrescaler+1))-1;
  
  /* Set the Wake up reload timer */
  SpiritTimerSetWakeUpTimerReload(cWuReloadCounter,cWuPrescaler);
  
  /* Enable the auto reload on sync */
  SpiritTimerLdcrAutoReload(S_ENABLE);
  
  /* Enable the LDCR mode */
  SpiritTimerLdcrMode(S_ENABLE);
  
  SpiritCmdStrobeFlushRxFifo();
  
  /* Set Spirit in Rx state */
  SpiritCmdStrobeRx();
  
  /* infinite loop */
  while (1) {
    
    while(!xRxDataReadyFlag);
    xRxDataReadyFlag=RESET;
      
    if(xFirstReception==RESET)
    {
      SpiritTimerSetRxTimeoutMs(80.0);
      
      xFirstReception=SET;
      SdkEvalLedToggle(LED1);
    }
    
    
    
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
