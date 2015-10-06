/**
 * @file    SDK_EVAL_DFU_Desc.h
 * @author  MSH RF/ART Team IMS-Systems Lab
 * @version V1.0.0
 * @date    03/07/2011
 * @brief   Descriptor Header for Device Firmware Upgrade (DFU).
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_DFU_DESC_H
#define __SDK_EVAL_DFU_DESC_H


/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SDK_EVAL_DFU            SDK EVAL DFU Project
 * @{
 */


/** @defgroup SDK_EVAL_DFU_Descriptor     SDK EVAL DFU Descriptor
 * @{
 */


/** @defgroup SDK_EVAL_DFU_Descriptor_Exported_Types      SDK EVAL DFU Descriptor Exported Types
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Descriptor_Exported_Constants    SDK EVAL DFU Descriptor Exported Constants
 * @{
 */
#define DFU_SIZ_DEVICE_DESC             18

#define DFU_SIZ_CONFIG_DESC             27


#define DFU_SIZ_STRING_LANGID           4
#define DFU_SIZ_STRING_VENDOR           38
#define DFU_SIZ_STRING_PRODUCT          20
#define DFU_SIZ_STRING_SERIAL           26
#define DFU_SIZ_STRING_INTERFACE0       96    /*!< Flash Bank 0 */



extern  uint8_t g_vectcDFUDeviceDescriptor[DFU_SIZ_DEVICE_DESC];
extern  uint8_t g_vectcDFUConfigDescriptor[DFU_SIZ_CONFIG_DESC];
extern  uint8_t g_vectcDFUStringLangId     [DFU_SIZ_STRING_LANGID];
extern  uint8_t g_vectcDFUStringVendor     [DFU_SIZ_STRING_VENDOR];
extern  uint8_t g_vectcDFUStringProduct    [DFU_SIZ_STRING_PRODUCT];
extern  uint8_t g_vectcDFUStringSerial     [DFU_SIZ_STRING_SERIAL];
extern  uint8_t g_vectcDFUStringInterface0 [DFU_SIZ_STRING_INTERFACE0];

#define bMaxPacketSize0             0x40     /*!< bMaxPacketSize0 = 64 bytes   */
#define wTransferSize               0x0400   /*!< wTransferSize   = 1024 bytes */
/* bMaxPacketSize0 <= wTransferSize <= 32kbytes */
#define wTransferSizeB0             0x00
#define wTransferSizeB1             0x04

/**
 * @}
 */ 


/** @defgroup SDK_EVAL_DFU_Descriptor_Exported_Macros       SDK EVAL DFU Descriptor Exported Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Descriptor_Exported_Functions     SDK EVAL DFU Descriptor Exported Functions
 * @{
 */


/**
 * @}
 */ 


/**
 * @}
 */


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* __SDK_EVAL_DFU_DESC_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
