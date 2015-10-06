/**
* @file    SDK_Encryption_A.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of encryption and decryption with SPIRIT Basic packets.
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
* @addtogroup SDK_Encryption                   SDK Encryption
* @{
*/

/**
* @addtogroup SDK_Encryption_A                 SDK Encryption A
* @brief Device A configured as a ping-pong player.
* @details This code performs operation using the AES on datas excanged by the two applications A
* and B.
*
* The user can modify the AES key and the other parameters editing the defines at the beginning of the file.
* @{
*/

/**
* @defgroup Encryption_A_Private_TypesDefinitions              Encryption A Private TypesDefinitions
* @{
*/

/**
*@}
*/


/**
* @defgroup Encryption_A_Private_Defines                       Encryption A Private Defines
* @{
*/

/* AES configuration parameters */
#define AES_KEY                         0xA1,0xB2,0xC3,0xD4,0xE5,0xF6,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA
#define N_BYTES                         16

/**
*@}
*/


/**
* @defgroup Encryption_A_Private_Macros                                Encryption A Private Macros
* @{
*/

/**
*@}
*/

/**
* @defgroup Encryption_A_Private_Variables                             Encryption A Private Variables
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
* @brief Flags declarations
*/
FlagStatus xRxDoneFlag = RESET,
xTxDoneFlag=RESET,
xAesEndFlag=RESET;

/**
* @brief IRQ status struct declaration
*/
SpiritIrqs xIrqStatus;


/**
* @brief Buffer declaration: data to transmit
*/
uint8_t vectcBuff[N_BYTES]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}, vectcEncBuff[N_BYTES],
cRxData;


/**
* @brief Tx counter declaration
*/
uint8_t cTxCounter=0;


/**
* @brief Variables used for encryption declaration
*/
uint8_t vectcAesEncKey[16]={AES_KEY};

/**
*@}
*/

/**
* @defgroup Encryption_A_Private_FunctionPrototypes                            Encryption A Private FunctionPrototypes
* @{
*/

/**
*@}
*/


/**
* @defgroup Encryption_A_Private_Functions                                     Encryption A Private Functions
* @{
*/

/**
* @brief  This function handles External lines 15 to 10 interrupt request.
* @param  None
* @retval None
*/
void M2S_GPIO_3_EXTI_IRQ_HANDLER(void)
{
  /* control the EXTI flag */
  if(EXTI_GetFlagStatus(M2S_GPIO_3_EXTI_LINE))
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
    if(xIrqStatus.IRQ_AES_END)
    {
      xAesEndFlag=SET;
    }
    
    /* clear the EXTI flag */
    EXTI_ClearFlag(M2S_GPIO_3_EXTI_LINE);
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
  
#ifdef STM8L
  enableInterrupts();
#elif SDK
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0A,0x0A,ENABLE);
#endif
  
  /* Spirit IRQ config */
  SpiritGpioInit(&xGpioIRQ);
  
  /* Spirit Radio config */
  SpiritRadioInit(&xRadioInit);
  
  /* Spirit Radio set power */
  SpiritRadioSetPALeveldBm(7,POWER_DBM);
  SpiritRadioSetPALevelMaxIndex(7);
  
  /* Spirit Packet config */
  SpiritPktBasicInit(&xBasicInit);
  SpiritPktBasicAddressesInit(&xAddressInit);
  
  /* Spirit AES mode on */
  SpiritAesMode(S_ENABLE);
  
  /* Spirit IRQs enable */
  SpiritIrqDeInit(NULL);
  SpiritIrq(TX_DATA_SENT, S_ENABLE);
  SpiritIrq(RX_DATA_READY, S_ENABLE);
  SpiritIrq(AES_END, S_ENABLE);
  
  /* payload length config */
  SpiritPktBasicSetPayloadLength(N_BYTES);
  
  /* rx timeout config */
  SET_INFINITE_RX_TIMEOUT();
  SpiritTimerSetRxTimeoutStopCondition(ANY_ABOVE_THRESHOLD);
  
  /* destination address */
  SpiritPktBasicSetDestinationAddress(DESTINATION_ADDRESS);
  uint8_t addr = SpiritPktStackGetTransmittedDestAddress();
  
  /* IRQ registers blanking */
  SpiritIrqClearStatus();
  
  /* write the encryption key */
  SpiritAesWriteKey(vectcAesEncKey);
  
  /* infinite loop */
  while (1)
  {
    /* write data to be encrypted */
    SpiritAesWriteDataIn(vectcBuff, N_BYTES);
    
    /* execute AES encryption */
    SpiritAesExecuteEncryption();
    
    /* wait for the encryption */
    while(!xAesEndFlag);
    xAesEndFlag=RESET;
    
    /* read encrypted data to be sent */
    SpiritAesReadDataOut(vectcEncBuff, N_BYTES);
    
    /* fit the TX FIFO */
    SpiritCmdStrobeFlushTxFifo();
    SpiritSpiWriteLinearFifo(N_BYTES, vectcEncBuff);
    
#ifdef USE_VCOM
    /* send messages via COM */
    printf("A data to Tx\n\rdecrypted: [");
    
    for(uint8_t i=0 ; i<N_BYTES ; i++)
      printf("%d ", vectcBuff[i]);
    printf("]\n\r");
    
    printf("encrypted: [");
    for(uint8_t i=0 ; i<N_BYTES ; i++)
      printf("%d ", vectcEncBuff[i]);
    printf("]\n\r");
    
#endif
    
    /* Spirit Radio config */
    SpiritRadioInit(&xRadioInit);
    
    /* send the TX command */
    SpiritCmdStrobeTx();
    
    /* wait for TX done */
    while(!xTxDoneFlag);
    xTxDoneFlag = RESET;
    
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

