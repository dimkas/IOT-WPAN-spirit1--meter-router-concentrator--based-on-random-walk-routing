/**
* @file    main.c
* @author  HEA & RF BU - IMS Systems Lab
* @version V1.0.6
* @date    April 02, 2014
* @brief   This file includes routines for main application.
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
#include <stdlib.h>
#include "main.h"

#include "SDK_EVAL_VC_General.h"
#include "SPIRIT_SDK_Util.h"


#ifndef DEVICE_TYPE
#define DEVICE_TYPE METER/*meter-> METER, concentrator-> OTHER*/
#endif

#ifndef DEVICE_METER_TYPE
#define DEVICE_METER_TYPE 0x02/* Electricity meter=0x02, Gas meter=0x03... see pg. no. 19 of prEN 13757-3:2010 */
#endif

#ifndef DEVICE_BAND
#define DEVICE_BAND 868 /*169 or 868*/
#endif


#define TX_OUTPUT_POWER (3.0) // Between [-3.0 , +27.0] if the RF board has an external PA. Otherwise between [-30.0 , +13.0].
#define RX_TIMEOUT 1000

#if (DEVICE_BAND == 169)
#define DEVICE_WMBUS_MODE N1_MODE
#define DEVICE_WMBUS_CHANNEL CHANNEL_1a
#define WMBUS_FRAME_FORMAT FRAME_FORMAT_A
#elif (DEVICE_BAND == 868)
#define DEVICE_WMBUS_MODE S1_MODE
#define DEVICE_WMBUS_CHANNEL CHANNEL_1
#define WMBUS_FRAME_FORMAT FRAME_FORMAT_A
#else
#error DEVICE_BAND undefined or unsupported
#endif



uint8_t WMBusPayloadBuff[255] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x02,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x03,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x04,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x05,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x06,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x07,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x08,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x09,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x10,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x11,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x12,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x13,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x14,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x15,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x16,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff,\
                                  0x17,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0e,0x0f,0xff};

#define MSG_PAYLOAD_LENGTH          6

uint8_t dispString1[17];
uint8_t dispString2[17];
uint8_t dispString3[17];
uint8_t dispString4[17];

static __IO uint32_t DOWNStatus = 0x00, UPStatus = 0x00, SELStatus = 0x00,RIGHTStatus=0x00, LEFTStatus=0x00;
uint32_t VolumeInL =0x1234;
uint64_t accNum = 0x1E;
WMBusDeviceType_T WMBusDeviceType;
SpiritBool bNewUserDataAvailable = S_FALSE;
SpiritBool bNewAlertDataAvailable = S_FALSE;

#if (DEVICE_TYPE == OTHER) 
/*Concentrator database in dataeeprom with meter-id and corresponding encryption key*/
uint8_t ConcDataBaseBuff[10][26]={0x01,0x01,0x4E,0x8D,0x12,0x45,0x63,0x12,0x10,\
                                  0x02,0xc2,0x86,0x69,0x6d,0x88,0x7c,0x9a,0xa0,\
                                       0x61,0x1b,0xbb,0x3e,0x20,0x25,0xa4,0x5a,\
                                  0x02,0x01,0x4E,0x8D,0x11,0x45,0x63,0x17,0x10,\
                                  0x03,0x03,0x02,0x01,0x00,0x07,0x06,0x05,0x04,\
                                       0x0b,0x0a,0x09,0x08,0x0f,0x0e,0x0d,0x0c,\
                                  0x03,0x01,0x4E,0x8D,0x12,0x45,0x63,0x17,0x10,\
                                  0x04,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,\
                                      0x12,0x13,0x014,0x15,0x16,0x17,0x18,0x19,\
                                  0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
#endif

#if(DEVICE_METER_TYPE==0x02)/*ElectricityMeter*/
/*Encryption Key for meter and meter id*/
uint8_t manufIdBuff[8] = {0x4E,0x8D,0x12,0x45,0x63,0x12,0x10,0x02};
uint8_t MTR_EnKey_TO_EEPROM[16]={0xc2,0x86,0x69,0x6d,0x88,0x7c,0x9a,0xa0,0x61,\
  0x1b,0xbb,0x3e,0x20,0x25,0xa4,0x5a};
#elif(DEVICE_METER_TYPE==0x03)/*GasMeter*/
/*Encryption Key for meter and meter id*/
uint8_t manufIdBuff[8] = {0x4E,0x8d,0x11,0x45,0x63,0x17,0x10,0x03};
uint8_t MTR_EnKey_TO_EEPROM[16]={0x03,0x02,0x01,0x00,0x07,0x06,0x05,0x04,0x0b,\
  0x0a,0x09,0x08,0x0f,0x0e,0x0d,0x0c};
#elif(DEVICE_METER_TYPE==0x04)/*Heatmeter*/
/*Encryption Key for meter and meter id for Heat meter*/
uint8_t manufIdBuff[8] = {0x4E,0x8D,0x12,0x45,0x63,0x17,0x10,0x04};
uint8_t MTR_EnKey_TO_EEPROM[16]={0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,\
  0x13,0x014,0x15,0x16,0x17,0x18,0x19};
#elif(DEVICE_METER_TYPE==0x31)
uint8_t manufIdBuff[8] = {0x4E,0x8D,0x12,0x45,0x63,0x17,0x10,0x31};
uint8_t MTR_EnKey_TO_EEPROM[16]={0x00};
#endif
/**
* @}
*/
/** @defgroup MAIN_Private_Functions
* @{
*/


/**
* @brief  This the the main application routine.
* @param  None.
* @retval None.
*/
void main (void)
{
  float foffset=0;
  
  /* Offset to do not overwrite the DFU */
  extern uint32_t __vector_table;
  if((NVIC_VectTab_FLASH & (uint32_t)(&__vector_table)) && ((uint32_t)(&__vector_table))>=0x8003000)
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE, ENABLE );
    
  /*writing meter database to EEPROM*/
  Write_Meter_DB_EEPROM();
  
  SdkEvalIdentification();
  
  /* Initialize the Virtual Com */
//  SdkEvalVCInit(); 
//  while(bDeviceState != CONFIGURED);
//  
  /* Leds Initialization */
  SdkEvalLedInit(LED1);  SdkEvalLedOn(LED1);
  SdkEvalLedInit(LED2);
  SdkEvalLedInit(LED3);
  SdkEvalLedInit(LED4);
  SdkEvalLedInit(LED5);
  
  /* Start of the System Clock */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  SdkStartSysTick();
  
  /* Initialize the SDN pin micro side */
  SdkEvalM2SGpioInit(M2S_GPIO_SDN,M2S_MODE_GPIO_OUT);
  
  /* Regulate the Vcc_RF */
  SdkEvalPmRfSwitchInit();
  SdkEvalPmI2CInit();
  SdkEvalPmADCInit();
  SdkEvalPmRegulateVRfI(3.3);
  SdkEvalPmRfSwitchToVRf();
  
  EepromSpiInitialization();  
  SpiritSpiInit();
  
    uint8_t tmpreg;
  /* Board management */   
  SdkEvalEnterShutdown();
  do{
    SdkEvalSpiReadRegisters(0xC1, 1, &tmpreg);
    tmpreg = tmpreg>>1;
  }while(tmpreg!=0 && tmpreg!=0x7F);
   
  SdkEvalExitShutdown(); 
  do{
    /* Reads the MC_STATUS register */
    SdkEvalSpiReadRegisters(0xC1, 1, &tmpreg);
    tmpreg = tmpreg>>1;
  }while(tmpreg!=MC_STATE_READY);

  /* Enable the external interrupt on the buttons lines */
  SdkEvalPushButtonInit(BUTTON_RIGHT, BUTTON_MODE_EXTI);
  SdkEvalPushButtonInit(BUTTON_DOWN, BUTTON_MODE_EXTI);
  if(SdkEvalGetVersion()==SDK_EVAL_VERSION_2_1)
    SdkEvalPushButtonInit(BUTTON_KEY, BUTTON_MODE_EXTI);
  else
    SdkEvalPushButtonInit(BUTTON_SCM_PS, BUTTON_MODE_EXTI);
  
  
  /* Micro EXTI config */      
  SdkEvalM2SGpioInit(M2S_GPIO_3,M2S_MODE_EXTI_IN);
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0F,0x0F,DISABLE); 
  
  SpiritManagementIdentificationRFBoard();
  if(SdkEvalGetHasEeprom())
  {
    uint8_t tempVect[4];
    EepromRead(0x0021,4,tempVect);
    for(uint8_t i=0;i<4;i++)
    {
      ((uint8_t*)&foffset)[i]=tempVect[3-i];
    }
  }
  else
  {
#if (DEVICE_BAND == 169)
    SpiritManagementSetRangeExtender(RANGE_EXT_SKYWORKS_169);
#elif (DEVICE_BAND == 868)
    SpiritManagementSetRangeExtender(RANGE_EXT_SKYWORKS_868);
#endif
  }
  
  SpiritEnterShutdown();  
  
  SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0F,0x0F,ENABLE);
  
  
  
  /* WMBUS configuration */
  WMBus_LinkSetAttribute(LINK_ATTR_ID_METER_TYPE,DEVICE_METER_TYPE);
  WMBus_LinkSetAttribute(LINK_ATTR_ID_DEV_TYPE,DEVICE_TYPE);                  // Device type configuration
  WMBus_LinkSetAttribute(LINK_ATTR_ID_MODE, DEVICE_WMBUS_MODE);               // WMBUS mode configuration
  WMBus_LinkSetAttribute(LINK_ATTR_ID_CHANNEL, DEVICE_WMBUS_CHANNEL);         // Channeel configuration
  WMBus_LinkSetAttribute(LINK_ATTR_ID_RX_TIMEOUT, RX_TIMEOUT);                // Define the time of the RX window
  WMBus_LinkSetAttribute(LINK_ATTR_ID_RF_POWER, (uint32_t) TX_OUTPUT_POWER);  // Define the output power of the transmitter
  WMBus_LinkSetAttribute(LINK_ATTR_ID_FRAME_FORMAT, WMBUS_FRAME_FORMAT);  // Define the wmbus frame format
  WMBus_LinkSetAttribute(LINK_ATTR_ID_OFFSET, (int32_t)foffset);
  
  WMBus_SetBoardMnfID(manufIdBuff);

  /* Initialization of the Application Layer */
  WMBus_AppliInit();  
  
  /* Get the device type */
  WMBusDeviceType = WMBus_GetWMBusDeviceType();
  
  /* Define the Install mode as start operation mode */
  WMBusOperationMode = INSTALL_MODE;
  
  /* Start the loop */
  while (1)
  {
    /* If the device is a concentrator */ 
    if(OTHER == WMBusDeviceType)    
    {
      /* If new data has been received */
      if(bNewdatareceived == S_TRUE)
      {
        /* Copy the data */
        strcpy((char*)dispString1, (const char*)"MFR ID-");
        strcat((char*)dispString1, (const char*)TFTDisplayFrame.MeterMFR);
        strcpy((char*)dispString2, (const char*)"A-");
        strcat((char*)dispString2, (const char*)TFTDisplayFrame.A_Filed);
        strcpy((char*)dispString3, (const char*)"Meter Type-");
        strcat((char*)dispString3, (const char*)TFTDisplayFrame.DeviceType);
        strcpy((char*)dispString4, (const char*)"Data-");
        strcat((char*)dispString4, (const char*)TFTDisplayFrame.Payload);
        bNewdatareceived = S_FALSE;
        
        /* Print the data on terminal */
        printf("-------------------------------------------------------------\r\n");
        printf("%s\r\n",dispString1);
        printf("%s\r\n",dispString2);
        printf("%s\r\n",dispString3);
        printf("%s\r\n",dispString4);
        printf("%s\r\n",TFTDisplayFrame.installed ? "Installed" : "Not installed");
        printf("-------------------------------------------------------------\r\n");
        SdkEvalLedToggle(LED2);
      }
      /* Send a reception command */
      WMBus_AppliPollRequests();
    }
    
    /* If the device is a meter*/ 
    if(METER == WMBusDeviceType)
    {
      /* If the joystick has been push on right */
      if(RIGHTStatus)
      {
        
        RIGHTStatus =0;
        SdkEvalLedOn(LED2);    
        /* Send Meter Data over RF*/
        
        WMBus_AppliSendMeterData(WMBusPayloadBuff, MSG_PAYLOAD_LENGTH);  
          
        
        /*increase access number after each packet transmission*/
        accNum++;
        SdkEvalLedToggle(LED2);
          
        
      }
      /* If the joystick has been push down */
      if(DOWNStatus)
      {
        DOWNStatus = 0x00;
        /* Send the installation request */ 
        WMBus_appliSendMeterInstallationRequest();
     }
      /* If the joystick has been push up */
      if(UPStatus)
      {        
        UPStatus =0x00;
        /* Increase the data to send */
        VolumeInL++;
      }
    }
  }
}
/**
* @brief  This routine updates the respective status for key press.
* @param  None
* @retval None
*/
void Set_KeyStatus(void)
{
  if(DATA_COMM_MODE == WMBusOperationMode)
  {
    SdkEvalLedOff(LED3);
    WMBusOperationMode = INSTALL_MODE;
  }
  else
  {
    WMBusDeviceType = WMBus_GetWMBusDeviceType();
    SdkEvalLedOn(LED3);
    WMBusOperationMode = DATA_COMM_MODE;
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
* @}
*/
/**
* @}
*/
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/