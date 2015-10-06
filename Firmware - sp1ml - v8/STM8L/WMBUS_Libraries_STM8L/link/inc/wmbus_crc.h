/**
 * @file    wmbus_crc.h
 * @author  HEA & RF BU - IMS Systems Lab
 * @version V1.0.3
 * @date    05/04/2012
 * @brief   This file contains header for wmbus_crc.c
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
