/**
******************************************************************************
* @file    wmbus_crc.h
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  This file contains header for wmbus_crc.c
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
#ifndef __WMBUS_CRC_H
#define __WMBUS_CRC_H
/* Includes ------------------------------------------------------------------*/
#include "SPIRIT_Config.h"

/**
 * @defgroup SPIRIT_WMBus_Link_CRC WMBus link CRC 
* @brief Configuration and management of WMBus Link layer CRC.
 * @details See the file <i>@ref wmbus_crc.h</i> for more details.
 * @{
 */
/**
 * @defgroup SPIRIT_WMBus_Link_CRC WMBus Link Layer CRC Exported defines
 * @{
 */
/**
 * @}
 */
/** @defgroup WMBus_Link_CRC_Exported_Functions          WMBus Link CRC Exported Functions
 * @{
 */
extern void AppendCRC(uint8_t *pStart, uint8_t *pStop);
extern SpiritBool CRCCheck(uint8_t *pStart, uint8_t *pStop);
/**
 * @}
 */

/**
 * @}
 */
#endif  /*__WMBUS_CRC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
