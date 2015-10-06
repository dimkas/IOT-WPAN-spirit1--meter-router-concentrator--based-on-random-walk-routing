/**
* @file   wmbus_appli.h
* @author  MSH RF/IMS-Systems Lab
* @version V1.0.0
* @date    05/04/2011
* @brief   This file contains header for wmbus_appli.c
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
* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
*/
#ifndef __WMBUS_APPLI_H
#define __WMBUS_APPLI_H
/* Includes ------------------------------------------------------------------*/
#include "wmbus_linkServices.h"
#include <stdlib.h>
#include "SDK_EVAL_Config.h"
#include "main.h"

/** @defgroup
* @brief This file contains header for WMBus application.
* @{
*/
/** @defgroup WMBUS_APPLI_Exported_Defines
* @{
*/
#define DATA_PAYLOAD_LENGTH 16
/**
* @}
*/
/** @defgroup WMBUS_APPLI_Exported_Types
* @{
*/
typedef struct
{
  uint16_t VALIDITY:1;/*!<Validity bit*/
  uint16_t GROUP_ID:6;/*!<Group ID 6 bits*/
  uint16_t SERIAL_NUM:9;/*!<SERIAL Number 9 bits*/
}SerialNum_t;
//typedef struct 
//{
//  SerialNum_t SerialNum;/*!<Serial Number of Device*/
//  uint8_t DeviceType;/*!<Device Type: meter/other*/
//  uint8_t KeyType;/*!<database keytype*/
//  uint8_t Status;/*!<status of device*/
//  uint8_t TimeStamp[6];/*!<Time at which a packet is transmitted or received*/
//  uint8_t Payload[DATA_PAYLOAD_LENGTH];/*!<Data to be transmitted or received*/
//  uint8_t EncrypKey[16];/*!<Encryption Key*/
//  uint8_t MeterID[8];/*!<Meter ID*/  
//  float RSSI_field; /*RSSI of the last payload received*/
//}WMBusDatabaseFrame_t;
//struct DataBaseList
//{
//  WMBusDatabaseFrame_t MeterDataFrame;/*!<Database of meters*/
//  struct DataBaseList *next;
//};



typedef enum
{
  DATA_COMM_MODE=0x00,/*!<Data Communication mode*/
  INSTALL_MODE/*!<Install mode*/
}WMBusOperationMode_t;

//typedef struct
//{
//  uint8_t GroupID;/*!<Device Group ID*/
//  uint8_t MeterID[8];/*!<Meter ID*/
//}MeterDatabase_t;

//typedef struct{
//  uint8_t MeterMFR[5];/*!<Meter Manufacturer Identification*/
//  uint8_t A_Filed[13];/*!<A field of packet*/    
//  uint8_t DeviceType[3];/*!<Device Type*/
//  uint8_t TimeStamp[6];/*!<Time of packet reception/transmission*/
//  uint8_t Payload[DATA_PAYLOAD_LENGTH];/*!<Actual Data*/
//  uint8_t length;/*!<length of packet*/
//  
//  uint8_t installed;
//}DisplayFrame_t;

/*Some Application State Variables*/

/**
* @}
*/
/** @defgroup WMBUS_APPLI_Exported_Variables
* @{
*/
//extern DisplayFrame_t TFTDisplayFrame;
extern volatile SpiritBool bNewdatareceived;
extern uint8_t WMBusPayloadBuff[];
extern WMBusOperationMode_t WMBusOperationMode;
extern SpiritBool LinkAttrChanged;
/**
* @}
*/

/** @defgroup WMBUS_APPLI_Exported_FunctionsPrototype
* @{
*/
extern void WMBus_AppliInit(void);
extern void WMBus_AppliRxDataHandler(Frame *Frame);
extern WMBusOperationMode_t WMBus_AppliGetOperationMode(void);
extern void WMBus_AppliSetOperationMode(WMBusOperationMode_t currmode);
//extern void WMBus_AppliSetEncryptKey(void *mfrID,EncryptionKey_t keydata);
//extern void WMBus_AppliGetEncryptKey(EncryptionKey_t *keydata);
//extern uint8_t WMBus_AppliReadMeter(uint64_t meterID, uint8_t *buff);
//extern uint16_t WMBus_AppliGetMeterList(MeterDatabase_t *pDatabase);

//extern SpiritBool WMBus_AppliUpdateMeterAttr(WMBusDatabaseFrame_t databaseFrame);
//extern SpiritBool WMBus_AppliDeleteMeter(uint64_t meterid);
//extern SpiritBool WMBus_AppliAddMeter(WMBusDatabaseFrame_t databaseFrame);

//extern void WMBus_GetBoardMnfID(uint8_t *buff);
//extern SpiritBool WMBus_SetBoardMnfID(uint8_t *buff);

//extern uint16_t WMBus_AppliGetGroupAttr(uint8_t groupid);
//extern uint16_t WMBus_AppliGetGroupMembers(MeterDatabase_t *pDatabase, uint8_t groupid);

extern LINK_STATUS WMBus_AppliSendMeterData(uint8_t *buff, uint8_t length);
//extern void WMBus_AppliReadMeterAttr(uint64_t MeterIDTemp, uint8_t *tempBuff);

//extern LINK_STATUS WMBus_AppliPollRequests(void);
extern WMBusDeviceType_T WMBus_GetWMBusDeviceType(void);
extern LINK_STATUS WMBus_appliSendMeterInstallationRequest(void);

//extern SpiritBool WMBus_GetTime(uint32_t *date, uint32_t *time);
//extern SpiritBool WMBus_SetTime(uint32_t date, uint32_t time);

//extern FrameFormatType_t WMBus_AppliGetFrameFormat(void);
//extern LINK_STATUS WMBus_AppliSetFrameFormat(FrameFormatType_t frameFormat);

extern void Appli_RadioPowerON(void);
extern void Appli_RadioPowerOFF(void);
extern void Appli_RadioStandBy(void);

extern SpiritBool WMBus_SetWMBusMode(WMBusMode_T mode);
extern SpiritBool WMBus_SetWMBusDeviceType(WMBusDeviceType_T devtype);
extern SpiritBool WMBus_SetWMBusChannel(WMBusChannel_T channel);
extern SpiritBool WMBus_SetWMBusMeterType(uint8_t MeterType);

/**
* @}
*/
/**
* @}
*/
#endif
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
