/**
* @file    SDK_WMbusStd_A.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of transmission of WMBUS std packets.
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
#define TX_BUFFER       100
#define PAYLOAD_LENGTH_FIX 100
#elif SDK
#include "SPIRIT_SDK_Util.h"
#include "SDK_Common.h"
#ifdef USE_VCOM
#include "SDK_EVAL_VC_General.h"
#endif
#define TX_BUFFER       350
#define PAYLOAD_LENGTH_FIX 256
#endif

//#define SUBMODE_N

/**
 * @addtogroup SDK_Examples SDK Examples
 * @{
 */

/**
 * @addtogroup SDK_WMbusStd                                SDK WMbus Std
 * @{
 */

/**
 * @addtogroup SDK_WMbusStd_A                                      SDK WMbus Std A
 * @brief Device A configured as a transmitter.
 * @details This code explains how to configure and manage
 * in the simpler way a transmitter with the WMBUS standard protocol.
 *
 * @{
 */

/**
 * @defgroup WMbusStd_A_Private_TypesDefinitions                   WMbus Std A Private TypesDefinitions
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup WMbusStd_A_Private_Defines                            WMbus Std A Private Defines
 * @{
 */

/*  Radio configuration parameters  */

#undef MODULATION_SELECT
#define MODULATION_SELECT           GFSK_BT05
#undef DATARATE
#define DATARATE                    32768
#undef FREQ_DEVIATION
#define FREQ_DEVIATION              50e3
#undef BANDWIDTH
#define BANDWIDTH                   125E3



#define CRC_POLYNOM 0x3D65

#ifndef SUBMODE_N

/*  Packet configuration parameters  */
#define MBUS_SUBMODE                    MBUS_SUBMODE_S1_S2_LONG_HEADER
#undef PREAMBLE_LENGTH
#define PREAMBLE_LENGTH                 36
#define POSTAMBLE_LENGTH                16

#endif

/**
 *@}
 */


/**
 * @defgroup WMbusStd_A_Private_Macros                                     WMbus Std A Private Macros
 * @{
 */

#define CEILING(X) (X-(uint16_t)(X) > 0 ? (uint16_t)(X+1) : (uint16_t)(X))

/**
 *@}
 */

/**
 * @defgroup WMbusStd_A_Private_Variables                                  WMbus Std A Private Variables
 * @{
 */

/**
 * @brief GPIO structure fitting
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

SGpioInit xGpioIRQ={
  SPIRIT_GPIO_3,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_IRQ
};

#ifndef SUBMODE_N

/**
 * @brief Packet Mbus structure fitting
 */
PktMbusInit xMbusInit={
  MBUS_SUBMODE,
  PREAMBLE_LENGTH,
  POSTAMBLE_LENGTH
};

#endif

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
uint8_t cLField=20;
uint8_t cThreholdTxFifoAE = 80;
uint8_t vectcTxBuff[TX_BUFFER];
uint16_t nDataToTx, nDataLength,nTxData, nResidualPcktLength, nPcktLength, nTxIndex;


/**
 *@}
 */

/**
 * @defgroup WMbusStd_A_Private_FunctionPrototypes                                         WMbus Std A Private FunctionPrototypes
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup WMbusStd_A_Private_Functions                                                  WMbus Std A Private Functions
 * @{
 */


/**
* @brief  Calculates the 16-bit CRC. The function requires that the CRC_POLYNOM is defined,
*           which gives the wanted CRC polynom. 
* @param  Input Param:uint8  crcData  - Data to perform the CRC-16 operation on.
*                     uint16 crcReg   - Current or initial value of the CRC calculation
* @retval LINK_STATUS : LINK_STATUS_SUCCESS
*       LINK_STATUS_INVALID_SIZE   - The data-field buffer is too large
*                       
*/
uint16_t crcCalc(uint16_t crcReg, uint8_t crcData) 
{
  uint8_t i;

  for (i = 0; i < 8; i++) 
  {
    // If upper most bit is 1
    if (((crcReg & 0x8000) >> 8) ^ (crcData & 0x80))
      crcReg = (crcReg << 1)  ^ CRC_POLYNOM;
    else
      crcReg = (crcReg << 1);
    
    crcData <<= 1;
  }
  return crcReg;
}


uint16_t computeCrc(uint8_t *pStart, uint8_t cNBytes)
{
  uint16_t  seed= 0x0000;

  for(uint8_t i=0; i<cNBytes ; i++)
  {
    seed = crcCalc(seed, pStart[i]);
  }
  seed =~seed;
  return seed;
}


void fitTxBuffer()
{
  uint16_t nCrc, j=0, maxIndex, nFittedData;
  
#ifdef SUBMODE_N 
  vectcTxBuff[j++] = 0x55;
  vectcTxBuff[j++] = 0x55;
  vectcTxBuff[j++] = 0xF6;
  vectcTxBuff[j++] = 0x8D;
  nPcktLength+=4;
#endif
  
  vectcTxBuff[j++] = cLField;   // L  field
  
  for(uint8_t i=0 ; i<9 ; i++)
  {
    vectcTxBuff[i+j] = i+1;
  }
  
  nCrc = computeCrc(&vectcTxBuff[j-1],10);
  j+=9;

  vectcTxBuff[j++] = (uint8_t)(nCrc>>8);
  vectcTxBuff[j++] = (uint8_t) nCrc;
  nFittedData=9;
  
  while(nFittedData<cLField)
  {
    if((cLField-nFittedData)/16 != 0)
      maxIndex = 16;
    else
      maxIndex = (cLField-nFittedData)%16;
      
    for(uint8_t i=0 ; i<maxIndex ; i++)
    {
      vectcTxBuff[j++] = i+1;
      nFittedData++;
    }
    
    nCrc = computeCrc(&vectcTxBuff[j-maxIndex],maxIndex);
    vectcTxBuff[j++] = (uint8_t)(nCrc>>8);
    vectcTxBuff[j++] = (uint8_t) nCrc;
  }
}


/**
 * @brief  This function handles External interrupt request (associated with Spirit GPIO 0).
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

          /* fill the Tx fifo */
          SpiritSpiWriteLinearFifo(nResidualPcktLength, &vectcTxBuff[nTxIndex]);
          
          /* unarm the AE threshold mechanism */
          SpiritLinearFifoSetAlmostEmptyThresholdTx(96);
          
          /* update the number of bytes to be transmitted */
          nResidualPcktLength = 0;
          
          /* update the number of transmitted bytes */
          nTxIndex += nResidualPcktLength;
        }
        
        /* re-read the number of elements in the Tx FIFO */
        cNElemTxFifo = SpiritLinearFifoReadNumElementsTxFifo();

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

  SdkEvalM2SGpioInit(M2S_GPIO_3,M2S_MODE_EXTI_IN);

  /* Spirit IRQ config */
  SpiritGpioInit(&xGpioIRQ);

  /* Spirit Radio config */
  SpiritRadioInit(&xRadioInit);
  
  /* Spirit Radio set power */
  SpiritRadioSetPALeveldBm(7,POWER_DBM);
  SpiritRadioSetPALevelMaxIndex(7);

#ifdef SUBMODE_N
  /* Spirit Direct Tx config */
  SpiritDirectRfSetTxMode(DIRECT_TX_FIFO_MODE);
#else
  /* Spirit Packet config */
  SpiritPktMbusInit(&xMbusInit);
#endif
  
  /* Spirit IRQs enable */
  SpiritIrqDeInit(NULL);
  SpiritIrq(TX_DATA_SENT , S_ENABLE);
  SpiritIrq(TX_FIFO_ALMOST_EMPTY , S_ENABLE);

  /* IRQ registers blanking */
  SpiritIrqClearStatus();

  EXTI_ClearITPendingBit(M2S_GPIO_3_EXTI_LINE);
  
#ifdef STM8L
  enableInterrupts();
#elif SDK
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0A,0x0A,ENABLE);
#endif
  
  
  /* infinite loop */
  while (1){    
    if(cLField<20)
      cLField=20;
    
    nPcktLength = 1+cLField+2+2*(CEILING(((float)cLField-9.0)/16.0));
  
    /* Index TX buffer */
    nTxIndex = 0;

    /* fit the packet payload with data */
    fitTxBuffer();

    /* payload length config */
    SpiritPktMbusSetPayloadLength(nPcktLength);
    
    /* make a copy of the payload length (to be used to have the number of the residual bytes to transmit) */
    nResidualPcktLength = nPcktLength;
    
#ifdef USE_VCOM
    printf("\r\nA data to transmit: [");
    for(uint16_t i=
#ifdef SUBMODE_N
        4 
#else
        0
#endif
          ;i<nPcktLength ;i++)
    {
      printf("%d ", vectcTxBuff[i]);
    }
    printf("]\r\n");
    printf("packet length= %d\r\n", nPcktLength
#ifdef SUBMODE_N
           -4
#endif
             );
#endif

    /* flush the Tx FIFO */
    SpiritCmdStrobeFlushTxFifo();

    /* check if the payload length is higher than the FIFO length */
    if(nPcktLength>96)
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
        SpiritSpiWriteLinearFifo(nPcktLength, vectcTxBuff);
      }

    /* send the TX command */
    SpiritCmdStrobeTx();

    /* wait for TX done */
    while(!xTxDoneFlag);
    xTxDoneFlag = RESET;
    
    /* pause between two transmissions */
    SdkDelayMs(1000);
    
    /* increment the payload length (overflowing it at PAYLOAD_LENGTH_FIX) */
    cLField++;
    if(cLField<20 || cLField>PAYLOAD_LENGTH_FIX)
      cLField=20;
    
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
