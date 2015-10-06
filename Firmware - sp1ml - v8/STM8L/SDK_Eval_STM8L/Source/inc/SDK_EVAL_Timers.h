/**
 * @file    SDK_EVAL_Timers.c
 * @author  High End Analog & RF BU - AMS
 * @version V2.0.2
 * @date    Febrary 7, 2012
 * @brief   Delay function.
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
#ifndef __SDK_EVAL_TIMERS_H
#define __SDK_EVAL_TIMERS_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_clk.h"


#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @addtogroup SDK_EVAL_STM8L
 * @{
 */
 
/**
 * @addtogroup SDK_EVAL_TIMERS		SDK EVAL Timers
 * @brief Management of STM8L timers.
 * @details See the file <i>@ref SDK_EVAL_Timers.h</i> for more details.
 * @{
 */
   

/**
 * @brief MCU XO frequency(in KHz) definition
 */
#define CLOCK_FREQUENCY         16000

   
/**
 * @brief  Configures the specified TIMER to raise an interrupt every TIME ms.
 * @param  TIMER: timer to be set.
 *          This parameter can be a pointer to @ref TIM_TypeDef
 * @param  TIME: timer duration in ms.
 *          This parameter is a float.
 * @retval None
 */
#define SdkEvalTimersTimConfig_ms(TIMER , TIME)      {\
                                                        uint32_t n = (uint32_t)(TIME*CLOCK_FREQUENCY);\
                                                        uint16_t a,b;\
                                                        SdkEvalTimersFindFactors(n,&a,&b);\
                                                        SdkEvalTimersTimConfig(TIMER,a-1,b-1);\
                                                      }

void SdkDelayMs(uint32_t lTimeMs);
void SdkEvalTimersFindFactors(uint32_t lCycles, uint16_t *pnPrescaler, uint16_t *pnCounter);

/**
 *@}
 */
 
 /**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
