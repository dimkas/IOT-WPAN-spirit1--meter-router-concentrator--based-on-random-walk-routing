/**
* @file    SDK_LDCR_A.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of transmission of SPIRIT Basic packets in LDCR mode.
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

/**
* @addtogroup SDK_Examples
* @{
*/

/**
* @addtogroup SDK_LDCR                         SDK LDCR
* @{
*/

/**
* @addtogroup SDK_LDCR_A                               SDK LDCR A
* @brief Device A configured as a transmitter.
* @details This code shows a synchronization method between two devices in order to obtain a low
* duty cycle communication and, as a matter of fact, save power. This code runs on the transmitter side.
*
* The user is allowed to change the algorithm parameters editing the defines at the beginning of the file.
* @{
*/

/**
* @defgroup LDCR_A_Private_TypesDefinitions            LDCR A Private TypesDefinitions
* @{
*/

/**
*@}
*/


/**
* @defgroup LDCR_A_Private_Defines                     LDCR A Private Defines
* @{
*/

/**
*@}
*/


/**
* @defgroup LDCR_A_Private_Macros                      LDCR A Private Macros
* @{
*/



/**
*@}
*/

/**
* @defgroup LDCR_A_Private_Variables                   LDCR A Private Variables
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
* @brief GPIO for IRQ raised notification structure fitting
*/
SGpioInit xGpioIRQ={
  SPIRIT_GPIO_3,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_IRQ
};

/**
* @brief GPIO for Tx test signal structure fitting
*/
SGpioInit xGpio1Tx=
{
  SPIRIT_GPIO_1,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_TX_STATE
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
* @brief Flags declarations
*/
FlagStatus xTxDoneFlag = RESET;

/**
* @brief IRQ status struct declaration
*/
SpiritIrqs xIrqStatus;


/**
* @brief Tx buffer declaration: data to transmit
*/
uint8_t vectcTxBuff[20]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};


/**
*@}
*/

/**
* @defgroup LDCR_A_Private_FunctionPrototypes                          LDCR A Private FunctionPrototypes
* @{
*/

/**
*@}
*/


/**
* @defgroup LDCR_A_Private_Functions                                   LDCR A Private Functions
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
      /* Set the tx_done_flag to manage the event in the main() */
      xTxDoneFlag = SET;
      
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
  
  /* Configure some output signals (to be probed with a scope to verify) */
  SpiritGpioInit(&xGpio1Tx);
  SpiritGpioInit(&xGpio0Sleep);

  /* Spirit Radio config */
  SpiritRadioInit(&xRadioInit);
  SpiritCalibrationRco(S_ENABLE);

  /* Spirit Radio set power */
  SpiritRadioSetPALevelMaxIndex(7);
  SpiritRadioSetPALeveldBm(7,POWER_DBM);
  
  /* Spirit Packet config */
  SpiritPktBasicInit(&xBasicInit);
  
  /* IRQ registers blanking */
  SpiritIrqClearStatus();
 
#ifdef STM8L
  enableInterrupts();
#elif SDK
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0A,0x0A,ENABLE);
#endif
  
  /* Spirit IRQs enable */
  SpiritIrqDeInit(&xIrqStatus);
  SpiritIrq(TX_DATA_SENT, S_ENABLE);
  
  /* payload length config */
  SpiritPktBasicSetPayloadLength(20);
  
  /* Set the wake Up event every WAKEUP_TIMER ms */
  SpiritTimerSetWakeUpTimerMs(WAKEUP_TIMER);
  
  SpiritCmdStrobeFlushTxFifo();
  
  /* Enable the LDCR mode */
  SpiritTimerLdcrMode(S_ENABLE);
  
  /* Send a Tx command */
  SpiritCmdStrobeTx();
  
  /* infinite loop */
  while (1)
  {
#ifdef USE_VCOM
    printf("A data to transmit: [");
    
    for(uint8_t i=0 ; i<20 ; i++)
      printf("%d ", vectcTxBuff[i]);
    printf("]\n\r");
#endif
    
    /* Load the Tx FIFO with 20 bytes of data */
    SpiritSpiWriteLinearFifo(20,vectcTxBuff);
    
    SpiritRefreshStatus();
    
    /* Wait for the Tx done */
    while(!xTxDoneFlag);
    xTxDoneFlag = RESET;
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
