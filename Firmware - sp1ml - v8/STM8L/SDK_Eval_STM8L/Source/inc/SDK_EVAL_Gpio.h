/**
 * @file    SDK_EVAL_Spirit_Gpio.h
 * @author  High End Analog & RF BU - AMS
 * @version V3.0.0
 * @date    August 7, 2012
 * @brief   GPIO Configuration used to drive SPIRIT GPIOs.
 * @details
 *
 * This module exports API to manage the Spirit GPIO from the micro
 * side. It's also provided with functions to put the Spirit in
 * shutdown or to turn it on.
 * The following example shows how to configure the shutdown pin
 * and the GPIO 3 as an EXTI input.
 *
 * <b>Example:</b>
 * @code
 *
 *   ...
 *
 *   SdkEvalM2SGpioInit(M2S_GPIO_SDN,M2S_MODE_GPIO_OUT);
 *
 *   SdkEvalM2SGpioInit(M2S_GPIO_3,M2S_MODE_EXTI_IN);
 *   SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3,0x0F,0x0F,ENABLE);
 *
 *   ...
 *
 *   SdkEvalSpiritExitShutdown();
 *
 *   ...
 *
 * @endcode
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
#ifndef __SDK_EVAL_SPIRIT_GPIO_H
#define __SDK_EVAL_SPIRIT_GPIO_H


  /* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup SDK_EVAL_STM8L
 * @{
 */
 
/** @addtogroup SDK_EVAL_GPIO			SDK EVAL Gpio
 * @brief GPIO Configuration used in the STM8L discovery to drive GPIOs.
 * @details See the file <i>@ref SDK_EVAL_Gpio.h</i> for more details.
 * @{
 */



/**
 * @brief  Number of MCU GPIO pins used for SPIRIT GPIO.
 */
#define M2S_GPIO_NUMBER    5


/**
 * @brief  MCU GPIO pin enumeration for SPIRIT GPIO
 */
typedef enum
{
  M2S_GPIO_0     = 0x00, /*!< GPIO_0 selected */
  M2S_GPIO_1     = 0x01, /*!< GPIO_1 selected */
  M2S_GPIO_2     = 0x02, /*!< GPIO_2 selected */
  M2S_GPIO_3     = 0x03, /*!< GPIO_3 selected */
  M2S_GPIO_SDN   = 0x04  /*!< GPIO_SDN selected */

}M2SGpioPin;

#define IS_M2S_GPIO_PIN(PIN) (((PIN) == M2S_GPIO_0) || \
                              ((PIN) == M2S_GPIO_1) || \
                              ((PIN) == M2S_GPIO_2) || \
                              ((PIN) == M2S_GPIO_3) || \
                              ((PIN) == M2S_GPIO_SDN))

/**
 * @brief  MCU GPIO pin working mode for SPIRIT GPIO
 */
typedef enum
{
  M2S_MODE_GPIO_IN  = 0x00,   /*!< Work as GPIO input */
  M2S_MODE_EXTI_IN,           /*!< Work as EXTI */
  M2S_MODE_GPIO_OUT,          /*!< Work as GPIO output */

}M2SGpioMode;

#define IS_M2S_GPIO_MODE(MODE) (((MODE) == M2S_MODE_GPIO_IN) || \
                                ((MODE) == M2S_MODE_EXTI_IN) || \
                                ((MODE) == M2S_MODE_GPIO_OUT))


#define M2S_GPIO_0_PORT                        GPIOE
#define M2S_GPIO_0_PIN                         GPIO_Pin_7
#define M2S_GPIO_0_EXTI_PIN_SOURCE             EXTI_Pin_7
#define M2S_GPIO_0_EXTI_LINE                   EXTI_IT_Pin7
#define M2S_GPIO_0_EXTI_IRQ_HANDLER            EXTI7_IRQHandler


#define M2S_GPIO_1_PORT                        GPIOE
#define M2S_GPIO_1_PIN                         GPIO_Pin_6
#define M2S_GPIO_1_EXTI_PIN_SOURCE             EXTI_Pin_6
#define M2S_GPIO_1_EXTI_LINE                   EXTI_IT_Pin6
#define M2S_GPIO_1_EXTI_IRQ_HANDLER            EXTI6_IRQHandler


#define M2S_GPIO_2_PORT                        GPIOC
#define M2S_GPIO_2_PIN                         GPIO_Pin_7
#define M2S_GPIO_2_EXTI_PIN_SOURCE             EXTI_Pin_7
#define M2S_GPIO_2_EXTI_LINE                   EXTI_IT_Pin7
#define M2S_GPIO_2_EXTI_IRQ_HANDLER            EXTI7_IRQHandler


#define M2S_GPIO_3_PORT                        GPIOC
#define M2S_GPIO_3_PIN                         GPIO_Pin_4
#define M2S_GPIO_3_EXTI_PIN_SOURCE             EXTI_Pin_4
#define M2S_GPIO_3_EXTI_LINE                   EXTI_IT_Pin4
#define M2S_GPIO_3_EXTI_IRQ_HANDLER            EXTI4_IRQHandler


#define M2S_GPIO_SDN_PORT                      GPIOC
#define M2S_GPIO_SDN_PIN                       GPIO_Pin_1


void SdkEvalM2SGpioInit(M2SGpioPin xGpio, M2SGpioMode xGpioMode);
void SdkEvalM2SGpioInterruptCmd(M2SGpioPin xGpio, uint8_t nPreemption, uint8_t nSubpriority, FunctionalState xNewState);
FlagStatus SdkEvalGpioGetLevel(M2SGpioPin xGpio);
void SdkEvalGpioSetLevel(M2SGpioPin xGpio, FlagStatus xLevel);



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
