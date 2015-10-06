/**
******************************************************************************
* @file    wmbus_userinterface.h
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  This file contains header for wmbus_userinterface.c
*
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software 
* distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WMBUS_USERINTERFACE_H
#define __WMBUS_USERINTERFACE_H
/* Includes ------------------------------------------------------------------*/
#include "wmbus_appli.h"

/**
 * @defgroup WMBUS_USERINTERFACE WMBus User Interface
 * @brief Configuration and management of WMBus User Interface layer.
 * @details See the file <i>@ref wmbus_userinterface.h</i> for more details.
 * @{
 */
/**
 * @defgroup WMBUS_USERINTERFACE WMBus User Interface Exported defines
 * @{
 */
#define DEFAULT_UPPER_TEMP_LIMIT                0x75
#define DEFAULT_LOWER_TEMP_LIMIT                0x00
#define DEFAULT_UPPER_TEMP_ALARM_THRES          0x40
#define DEFAULT_LOWER_TEMP_ALARM_THRES          0x00
/**
 *@}
 */
/**
 * @defgroup WMBUS_USERINTERFACE WMBus User Interface Exported variables
 * @{
 */
extern int16_t CurrentTemp;
extern int16_t MaxMeasTemp;
extern int16_t MinMeasTemp;
extern int16_t LowerAlarmThres;
extern int16_t UpperAlarmThres;
/**
 *@}
 */
/**
 * @brief  MBUS_USERINTERFACE WMBus User Interface enumeration
 * @{
 */
typedef enum 
{
  SPIRIT_EVAL=0x00,
  CONCENTRATOR_EVAL=0x10,
  METER_EVAL =0x11,
  OTHER_HW =0xFF
}SPIRIT_HWType_T;
/**
 *@}
 */

/** @defgroup WMBUS_USERINTERFACE WMBus User Interface Exported Functions
 * @{
 */
extern SpiritBool WMBus_UIResetBoard(uint8_t resetVar);
extern uint8_t WMBus_GetFWVersion(void);
extern uint8_t WMBus_GetHWVersion(void);
extern SPIRIT_HWType_T WMBus_GetHWType(void);
extern void WMBus_GetTempSensorData(uint8_t *buff);
extern void WMBus_SetTempSensorData(uint8_t *buff);


extern SpiritBool WMBus_SetWMBusDeviceType(WMBusDeviceType_T devtype);
extern WMBusMode_T WMBus_GetWMBusMode(void);
extern SpiritBool WMBus_SetWMBusMode(WMBusMode_T mode);
extern SpiritBool WMBus_SetWMBusMeterType(uint8_t MeterType);

extern SpiritBool WMBus_SetWMBusResponseDelay(uint16_t delay);
extern uint16_t WMBus_GetWMBusResponseDelay(void);
extern SpiritBool WMBus_SetDataTimeIntervals(uint32_t txInterval, uint32_t RxTimeout);
extern SpiritBool WMBus_GetDataTimeIntervals(uint32_t *txInterval, uint32_t *RxTimeout);
extern SpiritBool WMBus_SetPreamblePostambleLen(uint16_t preambleLen, uint8_t postambleLen);
extern SpiritBool WMBus_GetPreamblePostambleLen(uint16_t *preambleLen, uint8_t *postambleLen);
extern SpiritBool WMBus_GetRFPowerdBm(float *rfPower);
extern SpiritBool WMBus_SetRFPowerdBm(float rfPower);

extern void WMBus_GetEncryptionKey(uint8_t *buff, uint8_t *keytype, uint8_t *keyseqnum);
extern void WMBus_SetEncryptionKey(void *mfrID, uint8_t *buff, uint8_t keytype, uint8_t keyseqnum);
extern void WMBus_GetSerialPortParam(uint8_t *tempPtr);
extern void WMBus_SetSerialPortParam(uint8_t *tempPtr);
extern SpiritBool WMBus_SetWMBusChannel(WMBusChannel_T channel);
extern WMBusChannel_T WMBus_GetWMBusChannel(void);
extern void WMBus_GetWMBusConfig(void *vptr);
extern void WMBus_SetWMBusConfig(uint8_t *buff);
extern void WMBus_GetRSSILEDConfig(uint8_t *rssicntr);
extern void WMBus_SetRSSILEDConfig(uint8_t val);
extern void WMBus_ReadSpecificMeter(void *buff, uint64_t MeterID);
extern uint8_t WMBus_AddUpdateMeter(void *buff);
extern uint8_t WMBus_GetEventType(void);
extern SpiritBool WMBus_GetRSSIThresholddBm(int32_t *RSSIThreshold);
extern SpiritBool WMBus_SetRSSIThresholddBm(int32_t RSSIThreshold);
extern uint8_t* WMBus_AppliGetEEpromEntries(void);
/**
 *@}
 */
/**
 *@}
 */
#endif  /*__WMBUS_USERINTERFACE_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/