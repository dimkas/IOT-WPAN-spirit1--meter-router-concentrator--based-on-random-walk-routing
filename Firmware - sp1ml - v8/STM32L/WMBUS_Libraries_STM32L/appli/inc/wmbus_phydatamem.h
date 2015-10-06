/**
******************************************************************************
* @file    wmbus_phydatamem.h
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  This file contains header for wmbus_phydatamem.c
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
#ifndef __WMBUS_PHYDATAMEM_H
#define __WMBUS_PHYDATAMEM_H
/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include <string.h>
#include "SPIRIT_Config.h"
/**
 * @defgroup EEPROM_WMBUS_PARAM EEPROM WMBUS Parameter R/W
 * @brief Configuration and management of WMBUS Parameter in EEPROM
 * @details See the file <i>@ref wmbus_phydatamem.h</i> for more details.
 * @{
 */
/**
 * @defgroup EEPROM_WMBUS_PARAM EEPROM WMBUS Parameter  defines
 * @{
 */
#define EEPROM_NVM_BASE                0x08080064
#define EEPROM_METER1_DATABASE_START   0x08080100
#define EEPROM_METER1_DATABASE_END     0x08080120
   
#define EEPROM_METER2_DATABASE_START   0x08080122
#define EEPROM_METER2_DATABASE_END     0x08080142
   
#define EEPROM_METER3_DATABASE_START   0x08080144
#define EEPROM_METER3_DATABASE_END     0x08080164
   
#define EEPROM_METER4_DATABASE_START   0x08080166
#define EEPROM_METER4_DATABASE_END     0x08080186
   
#define EEPROM_METER5_DATABASE_START   0x08080188
#define EEPROM_METER5_DATABASE_END     0x080801A8

#define EEPROM_METER6_DATABASE_START   0x080801AA
#define EEPROM_METER6_DATABASE_END     0x080801CA

#define EEPROM_METER7_DATABASE_START   0x080801CC
#define EEPROM_METER7_DATABASE_END     0x080801EC

#define EEPROM_METER8_DATABASE_START   0x080801EE
#define EEPROM_METER8_DATABASE_END     0x0808020E
  
#define EEPROM_METER9_DATABASE_START   0x08080210
#define EEPROM_METER9_DATABASE_END     0x08080230
 
#define EEPROM_METER_10_DATABASE_START   0x08080233
#define EEPROM_METER_10_DATABASE_END     0x08080253 

#define EEPROM_MTR_EnKey_SADDRESS        0x08080300
#define EEPROM_MTR_EnKey_EADDRESS        0x08080320   

#define EEPROM_METER_ID_SADDRESS         0x08080322  
#define EEPROM_METER_ID_EADDRESS         0x08080332  


#define DEFAULT_CONFIG             (uint8_t)0x01
#define UPDATED_CONFIG             (uint8_t)0x02
#define TOBE_UPDATED_CONFIG        (uint8_t)0x03
/**
 *@}
 */

/** @defgroup EEPROM_WMBUS_PARAM EEPROM WMBUS Parameter Exported Functions
 * @{
 */
extern void WMBus_PhyReadNVMParam(uint8_t *NVMparam, uint8_t length);
extern void WMBus_PhyWriteNVMParam(uint8_t *NVMparam, uint8_t length);
extern void WMBus_PhyEraseNVMParam(uint32_t *NVMparam, uint8_t Wordlength);
extern uint8_t* WMBus_PhyReadMeterDatabase(uint32_t Saddress, uint16_t length, uint32_t Eaddress);
extern void WMBus_PhyWriteMeterDatabase(uint8_t *buff, uint16_t length, uint32_t Saddress);
extern void WMBus_WriteMeterDatabaseToEEPROM(uint8_t *buff, uint16_t length, uint32_t Saddress);
extern uint8_t* WMBus_ReadMeterEnkeyFromEEPROM(uint32_t Saddress, uint16_t length, uint32_t Eaddress);
extern void WMBus_EraseMeterDatabaseFromEEPROM(uint8_t *buff, uint32_t Saddress, uint8_t Wordlength);
extern void WMBus_EraseMeterDatabaseFromEEPROM1(uint32_t Saddress, uint8_t Wordlength);
extern void Read_EEPROM_Mtr_DataBase(uint64_t buff);

/**
 * @}
 */

/**
 * @}
 */
#endif  /*__WMBUS_PHYDATAMEM_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
