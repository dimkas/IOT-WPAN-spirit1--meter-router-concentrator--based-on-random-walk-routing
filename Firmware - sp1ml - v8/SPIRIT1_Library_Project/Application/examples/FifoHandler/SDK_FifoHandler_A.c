/**
* @file    SDK_FifoHandler_A.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of transmission of SPIRIT Basic packets with payload longer than 96 bytes.
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
 * @addtogroup SDK_Examples SDK Examples
 * @{
 */

/**
 * @addtogroup SDK_Fifo_Handler                                SDK FIFO Handler
 * @{
 */

/**
 * @addtogroup SDK_Fifo_Handler_A                                      SDK FIFO Handler A
 * @brief Device A configured as a transmitter.
 * @details This code explains how to configure and manage with payload longer than 96 bytes.
 *
 * The user can change the Basic packet configuration parameters editing the defines at the beginning of the file.
 * @{
 */

/**
 * @defgroup Fifo_Handler_A_Private_TypesDefinitions                   FIFO Handler A Private TypesDefinitions
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Fifo_Handler_A_Private_Defines                            FIFO Handler A Private Defines
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



/**
 *@}
 */


/**
 * @defgroup Fifo_Handler_A_Private_Macros                                     FIFO Handler A Private Macros
 * @{
 */

/**
 *@}
 */

/**
 * @defgroup Fifo_Handler_A_Private_Variables                                  FIFO Handler A Private Variables
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

#ifndef COLERIDGE
uint8_t vectcTxBuff[PAYLOAD_LENGTH_FIX];
#else
uint8_t vectcTxBuff[]="\n\rIt is an ancient Mariner,\n\r\
And he stoppeth one of three.\n\r\
'By thy long beard and glittering eye,\n\r\
Now wherefore stopp'st thou me?\n\n\r\
The Bridegroom's doors are opened wide,\n\r\
And I am next of kin;\n\r\
The guests are met, the feast is set:\n\r\
May'st hear the merry din.'\n\n\r\
He holds him with his skinny hand,\n\r\
'There was a ship,' quoth he.\n\r\
'Hold off! unhand me, grey-beard loon!'\n\r\
Eftsoons his hand dropt he.";
#endif


/**
 * @brief Tx buffer index, residual and total bytes to be transmitted
 */
uint16_t nTxIndex, nResidualPcktLength, nPayloadLength=200;

/**
 * @brief Almost empty Tx FIFO threshold
 */
uint8_t cThreholdTxFifoAE = 48;


/**
 *@}
 */

/**
 * @defgroup Fifo_Handler_A_Private_FunctionPrototypes                                         FIFO Handler A Private FunctionPrototypes
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Fifo_Handler_A_Private_Functions                                                  FIFO Handler A Private Functions
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

    /* Clear the EXTI line flag */
    EXTI_ClearITPendingBit(M2S_GPIO_3_EXTI_LINE);

    /* Check the SPIRIT TX_DATA_SENT IRQ flag */
    if(xIrqStatus.IRQ_TX_DATA_SENT)
    {
      /* set the xTxDoneFlag to manage the event in the main() */
      xTxDoneFlag = SET;

      SdkEvalLedToggle(LED1);
    }
    else
      /* Check the SPIRIT TX_FIFO_ALMOST_EMPTY IRQ flag */
      if(xIrqStatus.IRQ_TX_FIFO_ALMOST_EMPTY)
      {
        /* read the number of elements in the Tx FIFO */
        uint8_t cNElemTxFifo = SpiritLinearFifoReadNumElementsTxFifo();

        /* check if the sum of the residual payload to be transmitted and the actual bytes in FIFO are higher than 96 */
        if(nResidualPcktLength+cNElemTxFifo>96)
        {
          /* .. if yes another almost full IRQ has to be managed */

          /* ..so fill the Tx FIFO */
          SpiritSpiWriteLinearFifo(96-cNElemTxFifo-1, &vectcTxBuff[nTxIndex]);

          /* update the number of bytes to be transmitted */
          nResidualPcktLength -= 96-cNElemTxFifo-1;

          /* update the number of bytes transmitted until now */
          nTxIndex += 96-cNElemTxFifo-1;
        }
        else
        {
          /* .. if not all the nResidualPcktLength bytes remaining can be written to the Tx FIFO */
          /* ..so disable the TX_FIFO_ALMOST_EMPTY IRQ */
          SpiritIrq(TX_FIFO_ALMOST_EMPTY , S_DISABLE);

          /* unarm the AE threshold mechanism */
          SpiritLinearFifoSetAlmostEmptyThresholdTx(96);

          /* fill the Tx fifo */
          SpiritSpiWriteLinearFifo(nResidualPcktLength, &vectcTxBuff[nTxIndex]);

          /* update the number of transmitted bytes */
          nTxIndex += nResidualPcktLength;

          /* update the number of bytes to be transmitted */
          nResidualPcktLength = 0;

        }

        /* re-read the number of elements in the Tx FIFO */
        cNElemTxFifo = SpiritLinearFifoReadNumElementsTxFifo();

      }

  }

}


/**
 * @brief  Fits the Tx buffer with the first nPayloadLength integer numbers starting from 0
 * @param  None
 * @retval None
 */
void fitPayload(void)
{
  for(uint16_t i=0; i<nPayloadLength ; i++)
    vectcTxBuff[i]=i;
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
  
  /* Spirit Radio set power */
  SpiritRadioSetPALeveldBm(7,POWER_DBM);
  SpiritRadioSetPALevelMaxIndex(7);

  /* Spirit Packet config */
  SpiritPktBasicInit(&xBasicInit);
  SpiritPktBasicAddressesInit(&xAddressInit);

  /* Spirit IRQs enable */
  SpiritIrqDeInit(NULL);
  SpiritIrq(TX_DATA_SENT, S_ENABLE);

  /* IRQ registers blanking */
  SpiritIrqClearStatus();

  /* enable the GPIO 0 IRQ (MCU side) */
  EXTI_ClearITPendingBit(M2S_GPIO_3_EXTI_LINE);

#ifdef STM8L
  enableInterrupts();
#elif SDK
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0A,0x0A,ENABLE);
#endif
  
  /* destination address */
  SpiritPktBasicSetDestinationAddress(DESTINATION_ADDRESS);

  /* IRQ registers blanking */
  SpiritIrqClearStatus();

  /* flush the Tx FIFO */
  SpiritCmdStrobeFlushTxFifo();

#ifdef COLERIDGE
  nPayloadLength = strlen((char*)vectcTxBuff);
#endif
    
  /* infinite loop */
  while (1)
  {
    /* make a copy of the payload length (to be used to have the number of the residual bytes to transmit) */
    nResidualPcktLength = nPayloadLength;

    /* index TX buffer */
    nTxIndex = 0;

#ifndef COLERIDGE
    /* fit the packet payload with data */
    fitPayload();
#endif

    /* payload length config */
    SpiritPktBasicSetPayloadLength(nPayloadLength);

#ifdef USE_VCOM
    printf("A data to transmit: \n\r");
    for(uint16_t i=0 ; i<nPayloadLength ; i++)
    {
#ifndef COLERIDGE
      printf("%d ", vectcTxBuff[i]);
#else
      printf("%c", vectcTxBuff[i]);
#endif
    }
    printf("\n\n\r");
#endif

    /* flush the Tx FIFO */
    SpiritCmdStrobeFlushTxFifo();

    /* check if the payload length is higher than the FIFO length */
    if(nPayloadLength>96)
      {
        /* ... if yes transmit data using an AE IRQ and a FIFO reloading mechanism */
        /* set the almost empty threshold */
        SpiritLinearFifoSetAlmostEmptyThresholdTx(cThreholdTxFifoAE);
        SpiritIrq(TX_FIFO_ALMOST_EMPTY , S_ENABLE);

        /* write the linear fifo with the first 96 bytes of payload */
        SpiritSpiWriteLinearFifo(96, vectcTxBuff);

        /* store the number of transmitted bytes */
        nTxIndex = 96;

        /* update the residual number of bytes to be transmitted */
        nResidualPcktLength -= 96;
      }
    else
      {
        /* .. if not transmit data writing all of them to the Tx FIFO */
        SpiritSpiWriteLinearFifo(nPayloadLength, vectcTxBuff);
      }

    /* send the TX command */
    SpiritCmdStrobeTx();

    /* wait for TX done */
    while(!xTxDoneFlag);
    xTxDoneFlag = RESET;

#ifdef USE_VCOM
      printf("\n\rAE Thr=%d\n\rPayload length=%d\n\n\r ", cThreholdTxFifoAE,nPayloadLength);

#endif

    /* pause between two transmissions */
    SdkDelayMs(500);
    
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
