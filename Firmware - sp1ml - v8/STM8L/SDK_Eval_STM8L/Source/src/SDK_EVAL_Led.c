/**
 * @file    SDK_EVAL_Led.c
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


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Led.h"

/**
 * @addtogroup SDK_EVAL_STM8L
 * @{
 */
 
/** @addtogroup SDK_EVAL_LED
 * @{
 */


/** @defgroup SDK_EVAL_Led_Private_Variables                    SDK EVAL Led Private Variables
 * @{
 */
GPIO_TypeDef* vectpxGpioPort[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT};
static const uint16_t s_vectnGpioPin[LEDn] = {LED1_PIN, LED2_PIN};


/**
 * @brief  Configures LED GPIO.
 * @param  xLed Specifies the Led to be configured.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 * @retval None.
 */
void SdkEvalLedInit(SdkEvalLed xLed)
{
  /* Configure the GPIO_LED pin */
  GPIO_Init(vectpxGpioPort[xLed], s_vectnGpioPin[xLed], GPIO_Mode_Out_PP_High_Fast);
}

/**
 * @brief  Turns selected LED On.
 * @param  xLed Specifies the Led to be set on.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 * @retval None.
 */
void SdkEvalLedOn(SdkEvalLed xLed)
{
  vectpxGpioPort[xLed]->ODR |= s_vectnGpioPin[xLed];
}

/**
 * @brief  Turns selected LED Off.
 * @param  xLed Specifies the Led to be set off.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 * @retval None.
 */
void SdkEvalLedOff(SdkEvalLed xLed)
{
  vectpxGpioPort[xLed]->ODR &= ~s_vectnGpioPin[xLed];
}

/**
 * @brief  Toggles the selected LED.
 * @param  xLed Specifies the Led to be toggled.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 * @retval None.
 */
void SdkEvalLedToggle(SdkEvalLed xLed)
{
  vectpxGpioPort[xLed]->ODR ^= s_vectnGpioPin[xLed];
}

/**
 * @brief  Returns the status of a specified led.
 * @param  xLed Specifies the Led to be read.
 *         This parameter can be one of following parameters:
 *         @arg LED1
 *         @arg LED2
 * @retval FlagStatus return the status of the LED. This parameter can be:
 *         SET or RESET.
 */
FlagStatus SdkEvalLedGetState(SdkEvalLed xLed)
{
  if(vectpxGpioPort[xLed]->IDR & s_vectnGpioPin[xLed])
    return RESET;
  else
    return SET;
}

/**
 * @}
 */

/**
 * @}
 */



/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/