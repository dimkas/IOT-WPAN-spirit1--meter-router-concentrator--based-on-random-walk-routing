/**
 * @file    SDK_EVAL_DFU_General.h
 * @author  MSH RF/ART Team IMS-Systems Lab
 * @version V1.0.0
 * @date    03/07/2011
 * @brief   SDK USB Hardware Configuration & Setup for DFU.
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
#ifndef __SDK_EVAL_DFU_GENERAL_H
#define __SDK_EVAL_DFU_GENERAL_H


/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SDK_EVAL_DFU            SDK EVAL DFU Project
 * @{
 */


/** @defgroup SDK_EVAL_DFU_General      SDK EVAL DFU General
 * @{
 */


/** @defgroup SDK_EVAL_DFU_General_Exported_Types      SDK EVAL DFU General Exported Types
 * @{
 */

/**
 * @brief For STM32L15xx devices it is possible to use the internal USB pullup
 *        controlled by register SYSCFG_PMC (refer to RM0038 reference manual for
 *        more details).
 *        It is also possible to use external pullup (and disable the internal pullup)
 *        by setting the define USB_USE_EXTERNAL_PULLUP in file SDK_EVAL_Virtual_Com.h
 *        and configuring the right pin to be used for the external pull up configuration.
 *        Uncomment the following define to use an external pull up instead of the
 *        integrated STM32L15xx internal pull up. In this case make sure to set up
 *        correctly the external required hardware and the GPIO defines below.
 */

//#define USB_USE_EXTERNAL_PULLUP

#if !defined(USB_USE_EXTERNAL_PULLUP)
#define STM32L15_USB_CONNECT                SYSCFG_USBPuCmd(ENABLE)
#define STM32L15_USB_DISCONNECT             SYSCFG_USBPuCmd(DISABLE)

#elif defined(USB_USE_EXTERNAL_PULLUP)

/* PA10 is chosen just as illustrating example, you should modify the defines
below according to your hardware configuration. */
#define USB_DISCONNECT                      GPIOA
#define USB_DISCONNECT_PIN                  GPIO_Pin_10
#define RCC_AHBPeriph_GPIO_DISCONNECT       RCC_AHBPeriph_GPIOA
#define STM32L15_USB_CONNECT                GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN)
#define STM32L15_USB_DISCONNECT             GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN)
#endif /* USB_USE_EXTERNAL_PULLUP */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_General_Exported_Constants    SDK EVAL DFU General Exported Constants
 * @{
 */

/**
 * @brief   Flash memory address from where user application will be loaded
 */
#define ApplicationAddress        0x08003000

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_General_Exported_Macros       SDK EVAL DFU General Exported Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_General_Exported_Functions     SDK EVAL DFU General Exported Functions
 * @{
 */

void SdkEvalDFUInit(void);
void SdkEvalDFUGetSerialNum(void);
void SdkEvalDFUCableConfig (FunctionalState xNewState);
void SdkEvalDFUEnterLowPowerMode(void);
void SdkEvalDFULeaveLowPowerMode(void);
void SdkEvalDFUResetDevice(void);

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

#endif /* __SDK_EVAL_DFU_GENERAL_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
