/**
* @file    SDK_WMbusStd_B.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of reception of WMBUS std packets.
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
#define RX_BUFFER 100
#elif SDK
#include "SPIRIT_SDK_Util.h"
#include "SDK_Common.h"
#ifdef USE_VCOM
#include "SDK_EVAL_VC_General.h"
#endif
#define RX_BUFFER 350
#endif

//#define SUBMODE_N

/**
* @addtogroup SDK_Examples
* @{
*/

/**
* @addtogroup SDK_WMbusStd        SDK WMbus Std
* @{
*/

/**
* @addtogroup SDK_WMbusStd_B              SDK WMbus Std B
* @brief Device B configured as a receiver.
* @details This code explains how to configure a receiver with the
* WMBUS N protocol.
*
* @{
*/

/**
* @defgroup WMbusStd_B_Private_TypesDefinitions           WMbus Std B Private TypesDefinitions
* @{
*/

/**
*@}
*/


/**
* @defgroup WMbusStd_B_Private_Defines                    WMbus Std B Private Defines
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

#ifdef SUBMODE_N

/*  Packet configuration parameters */
#undef PREAMBLE_LENGTH
#define PREAMBLE_LENGTH             PKT_PREAMBLE_LENGTH_02BYTES
#undef SYNC_LENGTH
#define SYNC_LENGTH                 PKT_SYNC_LENGTH_2BYTES
#undef SYNC_WORD
#define SYNC_WORD                   0xF68D0000
#undef LENGTH_TYPE
#define LENGTH_TYPE                 PKT_LENGTH_FIX
#undef EN_WHITENING
#define EN_WHITENING                S_DISABLE
#undef CRC_MODE
#define CRC_MODE                    PKT_NO_CRC
#undef CONTROL_LENGTH
#define CONTROL_LENGTH              PKT_CONTROL_LENGTH_0BYTES
#undef EN_ADDRESS
#define EN_ADDRESS                  S_DISABLE
#undef EN_FEC
#define EN_FEC                      S_DISABLE

#else

/*  Packet configuration parameters  */
#define MBUS_SUBMODE                    MBUS_SUBMODE_S1_S2_LONG_HEADER
#undef PREAMBLE_LENGTH
#define PREAMBLE_LENGTH                 36
#define POSTAMBLE_LENGTH                16

#endif

#define CRC_POLYNOM 0x3D65

/**
*@}
*/


/**
* @defgroup WMbusStd_B_Private_Macros                             WMbus Std B Private Macros
* @{
*/


#define CEILING(X) (X-(uint16_t)(X) > 0 ? (uint16_t)(X+1) : (uint16_t)(X))

/**
*@}
*/

/**
* @defgroup WMbusStd_B_Private_Variables                          WMbus Std B Private Variables
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

#ifdef SUBMODE_N
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

#else


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
* @brief GPIO IRQ structure fitting
*/
SGpioInit xGpioIRQ={
  SPIRIT_GPIO_3,
  SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
  SPIRIT_GPIO_DIG_OUT_IRQ
};


/**
* @brief IRQ status struct declaration
*/
SpiritIrqs xIrqStatus;


/**
* @brief Rx buffer declaration: how to store the received data
*/
uint8_t vectcRxBuff[RX_BUFFER];

/**
* @brief A flag to notify the packet reception
*/
SpiritFlagStatus xRxDone=S_RESET;


/**
* @brief Packet length variable
*/
uint16_t nPcktLength;




/**
*@}
*/

/**
* @defgroup WMbusStd_B_Private_FunctionPrototypes                         WMbus Std B Private FunctionPrototypes
* @{
*/

/**
*@}
*/


/**
* @defgroup WMbusStd_B_Private_Functions                                  WMbus Std B Private Functions
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


/**
* @brief  Calculates the 16-bit CRC. 
* @param  Input Param:uint8  crcData  - Data to perform the CRC-16 operation on.
*                     uint16 crcReg   - Current or initial value of the CRC calculation
* @retval LINK_STATUS : LINK_STATUS_SUCCESS
*       LINK_STATUS_INVALID_SIZE   - The data-field buffer is too large
*                       
*/
SpiritBool CRCCheck(uint8_t *pStart, uint8_t *pStop)
{
  uint16_t  seed= 0x0000;
  
  while (pStart != pStop)
  {
    seed = crcCalc(seed, *pStart);
    pStart++;
  }
  seed =~seed;
  if((pStop[0]  == (uint8_t)(seed>>8))&&(pStop[1]  == (uint8_t)(seed)))
  {
    return S_TRUE;
  }
  return S_FALSE;
}


#define SM_RX_NEW_PACKET 0
#define SM_RX_INTERM 1
#define SM_RX_LAST 2
static void packetReceptionSM(void)
{
  static uint8_t rx_state=SM_RX_NEW_PACKET;
  static uint16_t nResidualPcktLength,nRxIndex;
  static uint8_t cRxDataLen;
  uint8_t cThreholdRxFifoAF;
  
  switch(rx_state)
  {
  
  case SM_RX_NEW_PACKET:
    {      
      
      SpiritSpiReadLinearFifo(12, &vectcRxBuff[0]);
      nRxIndex=12;
      uint8_t cLField=vectcRxBuff[0];
      
      /* L should be at least 10 (C-field+M-field+A-field+CI-field), otherwise give an error */
      if(cLField<10)
      {
        SpiritIrqClearStatus();
        SpiritCmdStrobeSabort();
        SpiritCmdStrobeFlushRxFifo();
        SpiritCmdStrobeRx();
        /* The SM should not continue! */
        return;
      }
      
   
      nPcktLength = 1+cLField+2+2*(CEILING(((float)cLField-9.0)/16.0));
      
      
      nResidualPcktLength=nPcktLength-nRxIndex;
        
      if(nResidualPcktLength>94)
      {      
        cThreholdRxFifoAF = 48;  
        rx_state = SM_RX_INTERM;
      }
      else
      {
        cThreholdRxFifoAF = nResidualPcktLength;
        rx_state = SM_RX_LAST;
      }
      SpiritLinearFifoSetAlmostFullThresholdRx(96-cThreholdRxFifoAF);

      
    }
    break;
  case SM_RX_LAST:
    {
      /* stop the reception*/
      SpiritCmdStrobeSabort();
      
      /* unarm the almost full threshold */
      SpiritIrq(RX_FIFO_ALMOST_FULL,S_DISABLE);
      
      /* Read the RX FIFO */
      SpiritSpiReadLinearFifo(nResidualPcktLength, &vectcRxBuff[nRxIndex]);
      /* update the number of received bytes */
      nResidualPcktLength=nPcktLength-nRxIndex-nResidualPcktLength;
      
      SpiritCmdStrobeFlushRxFifo();

      SpiritLinearFifoSetAlmostFullThresholdRx(96-12);
    
      
      rx_state=SM_RX_NEW_PACKET;
      xRxDone=S_SET;
               
    }
    break;
  case SM_RX_INTERM:
    {      

      /* we read always the same quantity of bytes */
      cRxDataLen = 47;      
      
      SpiritSpiReadLinearFifo(cRxDataLen, &vectcRxBuff[nRxIndex]);
      
      nRxIndex+=cRxDataLen; 
      nResidualPcktLength=nPcktLength-nRxIndex;
      
      
      if(nResidualPcktLength<90)
      {
        SpiritLinearFifoSetAlmostFullThresholdRx(96-nResidualPcktLength);
        rx_state = SM_RX_LAST;
      }
   
      /* workaround... because sometimes the IRQ line is found low because AF IRQ */
      SpiritIrqs irqStatus;SpiritIrqGetStatus(&irqStatus);
      
    }
    
    break;
    
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
    
    /* Get the IRQ status */
    SpiritIrqGetStatus(&xIrqStatus);
    
    /* Clear the EXTI line flag */
    EXTI_ClearITPendingBit(M2S_GPIO_3_EXTI_LINE);
    
    if(xIrqStatus.IRQ_RX_FIFO_ALMOST_FULL)
    {
      packetReceptionSM();
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
  SpiritBool xCrcResult = S_TRUE;
  
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
  
  SpiritRadioCsBlanking(S_ENABLE);
  
#ifdef SUBMODE_N
  /* Spirit Packet config */
  SpiritPktBasicInit(&xBasicInit);
  SpiritPktBasicAddressesInit(&xAddressInit);
#else
  /* Spirit Packet config */
  SpiritPktMbusInit(&xMbusInit);
#endif
  
  /* RX timeout config */
  SET_INFINITE_RX_TIMEOUT();
  
  /* Spirit IRQs enable */
  SpiritIrqDeInit(&xIrqStatus);
  
#ifdef SUBMODE_N
  /* payload length config */
  SpiritPktBasicSetPayloadLength(0x0FFF);
#else
  /* payload length config */
  SpiritPktMbusSetPayloadLength(0x0FFF);
#endif
  
  /* enable SQI check */
  SpiritQiSetSqiThreshold(SQI_TH_0);
  SpiritQiSqiCheck(S_ENABLE);
  
  SpiritQiSetRssiThresholddBm(-120);
  
  SpiritTimerSetRxTimeoutStopCondition(RSSI_AND_SQI_ABOVE_THRESHOLD);
  
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
            
    SpiritLinearFifoSetAlmostFullThresholdRx(96-12);
    SpiritIrq(RX_FIFO_ALMOST_FULL,S_ENABLE);
    
    /* RX command */
    SpiritCmdStrobeRx();
    
    while(!xRxDone);
    
    xCrcResult = CRCCheck(&vectcRxBuff[0],&vectcRxBuff[10]);
    
    for(uint16_t i=0 ; i<(nPcktLength-12)/18 ; i++)
    {
      xCrcResult &= CRCCheck(&vectcRxBuff[12+18*i],&vectcRxBuff[12+18*(i+1)-2]);
    }
    
    if((nPcktLength-12)%18!=0)
      xCrcResult &= CRCCheck(&vectcRxBuff[nPcktLength-((nPcktLength-12)%18)],&vectcRxBuff[nPcktLength-2]);
    
    SpiritCmdStrobeFlushRxFifo();
    
    if(xCrcResult)
    {
      SdkEvalLedToggle(LED2);
      
#ifdef USE_VCOM
      /* print the received data */
      printf("\n\rB data received: [");
      
      for(uint16_t i=0 ; i<nPcktLength ; i++)
      {
        printf("%d ", vectcRxBuff[i]);
      }
      printf("]\r\n");
      
      printf("RSSI = %.2f\r\npacket length= %d\r\n", SpiritQiGetRssidBm(), nPcktLength);

#endif
    }
    else
    {
      SdkEvalLedToggle(LED1);
    }
    
    xCrcResult=S_TRUE;
    xRxDone=S_RESET;

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
