/**
 * @file    main.c
 * @author  HEA & RF BU - IMS Systems Lab
 * @version V1.0.3
 * @date    April 02, 2012
 * @brief   Main application.
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
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
*/


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"
#include "SPIRIT_Config.h"
#include "SDK_Configuration_Common.h"
#include "main.h"
#include "STM8L_Common.h"

#define FREQ_OFFSET             (0)                     // set here the freq offset for your radio

#ifndef DEVICE_TYPE
#define DEVICE_TYPE METER/*meter-> METER, concentrator-> OTHER*/
#endif

#ifndef DEVICE_METER_TYPE
#define DEVICE_METER_TYPE 0x03/*Electricity meter=0x02, \
Gas meter=0x03... see pg. no. 19 of prEN 13757-3:2010.... This value must be \
defined in preprocessor*/
#endif

#ifndef DEVICE_BAND
#define DEVICE_BAND 169 /*169 or 868*/
#endif

/* Wireless Meter Bus Specific Configuration----------------------------------*/
#define TX_OUTPUT_POWER (3.0) // Between [-3.0 , +27.0] if the RF board has an external PA. Otherwise between [-30.0 , +13.0].
#define RX_TIMEOUT 1000

#if (DEVICE_BAND == 169)
#define XTAL_FREQUENCY          25000000
#define DEVICE_WMBUS_MODE N1_MODE
#define DEVICE_WMBUS_CHANNEL CHANNEL_1a 
#define WMBUS_FRAME_FORMAT FRAME_FORMAT_A

#elif (DEVICE_BAND == 868)
#define XTAL_FREQUENCY          50000000
#define DEVICE_WMBUS_MODE S1_MODE
#define DEVICE_WMBUS_CHANNEL CHANNEL_1 
#define WMBUS_FRAME_FORMAT FRAME_FORMAT_A
#else
#error DEVICE_BAND undefined or unsupported
#endif


static volatile uint8_t DOWNStatus = 0x00, UPStatus = 0x00, RIGHTStatus=0x00,
LEFTStatus=0x00, SELStatus = 0x00, DATAStatus=0x00;
uint32_t VolumeInL =0x1234;
WMBusDeviceType_T WMBusDeviceType;
uint8_t WMBusPayloadBuff[128] = {0x00};
static __IO uint8_t Counter = 0x00; 
uint8_t accNum = 0x1E;


/**
* @defgroup Basic_Generic_A_Private_Functions                                                  
* @{
*/



/**
* @brief  System main function.
* @param  None
* @retval None
*/
void main (void)
{
  SDK_SYSTEM_CONFIG();
  
  SdkEvalComInit(115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
  USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
    
  SpiritRadioSetXtalFrequency(XTAL_FREQUENCY);        
 
    /* Board management */   
  SpiritExitShutdown();  
  
  SpiritEnterShutdown(); 

  /*Setting the link layer attributes*/
  WMBus_LinkSetAttribute(LINK_ATTR_ID_METER_TYPE,DEVICE_METER_TYPE);                       // Meter type configuration, 0x03 :electricity meter
  WMBus_LinkSetAttribute(LINK_ATTR_ID_DEV_TYPE,DEVICE_TYPE);                        // Device type configuration
  WMBus_LinkSetAttribute(LINK_ATTR_ID_MODE, DEVICE_WMBUS_MODE);               // WMBUS mode configuration
  WMBus_LinkSetAttribute(LINK_ATTR_ID_CHANNEL, DEVICE_WMBUS_CHANNEL);         // Channeel configuration
  WMBus_LinkSetAttribute(LINK_ATTR_ID_RX_TIMEOUT, RX_TIMEOUT);                // Define the time of the RX window
  WMBus_LinkSetAttribute(LINK_ATTR_ID_RF_POWER, (uint32_t) TX_OUTPUT_POWER);  // Define the output power of the transmitter
  WMBus_LinkSetAttribute(LINK_ATTR_ID_FRAME_FORMAT, WMBUS_FRAME_FORMAT);  // Define the wmbus frame format
  WMBus_LinkSetAttribute(LINK_ATTR_ID_OFFSET, (int32_t)FREQ_OFFSET);
  
  
  WMBus_AppliInit();
 
  /* Micro EXTI config */      
  SdkEvalM2SGpioInit(M2S_GPIO_3,M2S_MODE_EXTI_IN);
  SdkEvalM2SGpioInit(M2S_GPIO_2,M2S_MODE_GPIO_IN);
  SdkEvalM2SGpioInit(M2S_GPIO_1,M2S_MODE_GPIO_IN);
  enableInterrupts();

  /* the following are used to manage the PA extenders */
#if (DEVICE_BAND == 169)
    /* CSD control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_0, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_TX_RX_MODE});
    
    /* CTX/BYP control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_1, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_TX_STATE});
    
    /* Vcont control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_2, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_RX_STATE});
#elif (DEVICE_BAND == 868) 
    /* CSD control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_0, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_TX_RX_MODE});
    
    /* CTX/BYP control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_1, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_RX_STATE});
    
    /* Vcont control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_2, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_TX_STATE});
#endif  
  
  /* infinite loop */
  while (1)
  {
    
    if(DATAStatus==1)
    {
      DATAStatus=0x00;
      WMBus_appliSendMeterInstallationRequest(); 
      SdkEvalLedOn(LED2);
    }
    if(RIGHTStatus==1)
    {
      RIGHTStatus = 0x00;
      SdkEvalLedToggle(LED2);
      /* For testing purpose */

      /* Demo meter Payload of 6 bytes */
      WMBusPayloadBuff[0] = 0x04;/* 32 Bit Integer */
      WMBusPayloadBuff[1] = 0x13;/* VIF volume in litres */
      
      WMBusPayloadBuff[2] = (uint8_t)(VolumeInL>>24);
      WMBusPayloadBuff[3] = (uint8_t)(VolumeInL>>16);
      WMBusPayloadBuff[4] = (uint8_t)(VolumeInL>>8);
      WMBusPayloadBuff[5] = (uint8_t)(VolumeInL);
            
      WMBus_AppliSendMeterData(WMBusPayloadBuff, 0x06);

      SdkEvalLedToggle(LED2);
     }
    else if((UPStatus))
    {        
      UPStatus =0x00;
      VolumeInL++;
    }
    
  } 
  
}

/**
* @brief  This routine updates the respective status for SEL key press.
* @param  None
* @retval None
*/
void Set_SELStatus(void)
{
  SELStatus = 1;
}
/**
* @brief  This routine updates the respective status for Down key press.
* @param  None
* @retval None
*/
void Set_DOWNStatus()
{
  DOWNStatus=1;
}
/**
* @brief  This routine updates the respective status for Up key press.
* @param  None
* @retval None
*/
void Set_UPStatus(void)
{
  UPStatus=1;
}


/**
* @brief  This routine updates the respective status for Right key press.
* @param  None
* @retval None
*/
void Set_RightStatus(void)
{
  RIGHTStatus=1;
}
/**
* @brief  This routine updates the respective status for Left key press.
* @param  None
* @retval None
*/
void SetLeftKeyStatus(void)
{
  LEFTStatus=1;
}
/**
* @brief  This routine updates the respective status for user key press.
* @param  None
* @retval None
*/
void Set_DataStatus(void)
{
  DATAStatus=1;
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


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
