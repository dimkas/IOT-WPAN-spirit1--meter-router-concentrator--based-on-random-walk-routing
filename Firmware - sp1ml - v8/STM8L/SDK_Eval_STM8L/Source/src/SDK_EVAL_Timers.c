/**
 * @file    SDK_EVAL_Timers.c
 * @author  High End Analog & RF BU - AMS
 * @version V3.0.0
 * @date    August 7, 2012
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
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Timers.h"

/**
 * @addtogroup SDK_EVAL_STM8L
 * @{
 */
 
/**
 * @addtogroup SDK_EVAL_TIMERS
 * @{
 */

/**
  * @brief  delay for some time in ms unit
  * @caller auto_test
  * @param  n_ms is how many ms of time to delay
  * @retval None
  */
void SdkDelayMs(uint32_t lTimeMs)
{
/* Init TIMER 4 */
  uint16_t n_ms = (uint16_t)lTimeMs;
  
/* Init TIMER 4 */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

/* Init TIMER 4 prescaler: / (2^6) = /64 */
  TIM4->PSCR = 6;

/* HSI div by 1 --> Auto-Reload value: 16M / 64 = 1/4M, 1/4M / 1k = 250*/
  TIM4->ARR = 250;
  
/* Counter value: 2, to compensate the initialization of TIMER*/
  TIM4->CNTR = 2;

/* clear update flag */
  TIM4->SR1 &= ~TIM4_SR1_UIF;

/* Enable Counter */
  TIM4->CR1 |= TIM4_CR1_CEN;

  while(n_ms--)
  {
    while((TIM4->SR1 & TIM4_SR1_UIF) == 0) ;
    TIM4->SR1 &= ~TIM4_SR1_UIF;
  }

/* Disable Counter */
  TIM4->CR1 &= ~TIM4_CR1_CEN;
}



#define S_ABS(a) ((a)>0?(a):-(a))

/**
 * @brief  Computes two integer value prescaler and period such that Cycles = prescaler * period.
 * @param  lCycles the specified cycles for the desired timer value.
 * @param  pnPrescaler prescaler factor.
 * @param  pnCounter period factor.
 * @retval None.
*/
void SdkEvalTimersFindFactors(uint32_t lCycles, uint16_t *pnPrescaler, uint16_t *pnCounter)
{
  uint16_t b0;
  uint16_t a0;
  long err, err_min=lCycles;

  *pnPrescaler = a0 = ((lCycles-1)/0xffff) + 1;
  *pnCounter = b0 = lCycles / *pnPrescaler;

  for (; *pnPrescaler < 0xffff-1; (*pnPrescaler)++)
  {
    *pnCounter = lCycles / *pnPrescaler;
    err = (long)*pnPrescaler * (long)*pnCounter - (long)lCycles;
    if (S_ABS(err) > (*pnPrescaler / 2))
    {
      (*pnCounter)++;
      err = (long)*pnPrescaler * (long)*pnCounter - (long)lCycles;
    }
    if (S_ABS(err) < S_ABS(err_min))
    {
      err_min = err;
      a0 = *pnPrescaler;
      b0 = *pnCounter;
      if (err == 0) break;
    }
  }

  *pnPrescaler = a0;
  *pnCounter = b0;

}


/**
 * @}
 */

/**
 *@}
 */


/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
