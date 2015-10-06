/**
 * @file    SDK_EVAL_Led.h
 * @author  High End Analog & RF BU - AMS
 * @version V3.0.0
 * @date    August 7, 2012
 * @brief   This file provides all the low level API to manage the LEDs.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_LED_H
#define __SDK_EVAL_LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @addtogroup SDK_EVAL_STM8L
 * @{
 */
 
/** @addtogroup SDK_EVAL_LED		SDK EVAL Led
 * @brief Management of STM8L discovery Leds.
 * @details See the file <i>@ref SDK_EVAL_Led.h</i> for more details.
 * @{
 */

/**
 * @brief  Enumeration of SDK EVAL LEDs
 */
typedef enum
{
  LED1 = 0,
  LED2 = 1,
  
} SdkEvalLed;

#define LEDn                             2

#define LED1_PIN                         GPIO_Pin_7
#define LED1_GPIO_PORT                   GPIOE

#define LED2_PIN                         GPIO_Pin_7
#define LED2_GPIO_PORT                   GPIOC


void SdkEvalLedInit(SdkEvalLed xLed);
void SdkEvalLedOn(SdkEvalLed xLed);
void SdkEvalLedOff(SdkEvalLed xLed);
void SdkEvalLedToggle(SdkEvalLed xLed);
FlagStatus SdkEvalLedGetState(SdkEvalLed xLed);

/**
 * @}
 */

 /**
 *@}
 */
 
#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
