/**
 * @file    SDK_EVAL_DFU_Flash_If.h
 * @author  MSH RF/ART Team IMS-Systems Lab
 * @version V1.0.0
 * @date    03/07/2011
 * @brief   Header for SDK_EVAL_DFU_Flash_If.c file.
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
#ifndef __SDK_EVAL_DFU_FLASH_IF_H
#define __SDK_EVAL_DFU_FLASH_IF_H


/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SDK_EVAL_DFU            SDK EVAL DFU Project
 * @{
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface    SDK EVAL DFU Flash Interface
 * @{
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface_Exported_Types      SDK EVAL DFU Flash Interface Exported Types
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface_Exported_Constants    SDK EVAL DFU Flash Interface Exported Constants
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface_Exported_Macros       SDK EVAL DFU Flash Interface Exported Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface_Exported_Functions     SDK EVAL DFU Flash Interface Exported Functions
 * @{
 */

uint16_t SdkEvalDFUFlashIfInit(void);
uint16_t SdkEvalDFUFlashIfErase (uint32_t lSectorAddress);
uint16_t SdkEvalDFUFlashIfWrite (uint32_t lSectorAddress, uint32_t lDataLength);
uint8_t *SdkEvalDFUFlashIfRead (uint32_t lSectorAddress, uint32_t lDataLength);

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

#endif /* __SDK_EVAL_DFU_FLASH_IF_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
