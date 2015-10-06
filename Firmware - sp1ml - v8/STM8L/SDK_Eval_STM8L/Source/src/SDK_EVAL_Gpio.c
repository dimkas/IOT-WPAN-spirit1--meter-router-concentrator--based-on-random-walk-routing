/**
 * @file     SDK_EVAL_Spirit_Gpio.c
 * @author  High End Analog & RF BU - AMS
 * @version V3.0.0
 * @date    August 7, 2012
 * @brief   GPIO Configuration used to drive SPIRIT GPIOs.
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
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Gpio.h"

/**
 * @addtogroup SDK_EVAL_STM8L
 * @{
 */
 
/** @addtogroup SDK_EVAL_GPIO
 * @{
 */

/**
 * @brief  M2S GPio Port array
 */

GPIO_TypeDef* vectpxM2SGpioPort[M2S_GPIO_NUMBER] = {
        M2S_GPIO_0_PORT,
        M2S_GPIO_1_PORT,
        M2S_GPIO_2_PORT,
        M2S_GPIO_3_PORT,
        M2S_GPIO_SDN_PORT
};

/**
 * @brief  M2S GPio Pin array
 */
static const GPIO_Pin_TypeDef s_vectnM2SGpioPin[M2S_GPIO_NUMBER] = {
        M2S_GPIO_0_PIN,
        M2S_GPIO_1_PIN,
        M2S_GPIO_2_PIN,
        M2S_GPIO_3_PIN,
        M2S_GPIO_SDN_PIN
};

/**
 * @brief  M2S GPio Pin array
 */
static const EXTI_Pin_TypeDef s_vectnM2SGpioLine[M2S_GPIO_NUMBER-1] = {
        M2S_GPIO_0_EXTI_PIN_SOURCE,
        M2S_GPIO_1_EXTI_PIN_SOURCE,
        M2S_GPIO_2_EXTI_PIN_SOURCE,
        M2S_GPIO_3_EXTI_PIN_SOURCE
};


/**
 * @brief  Configures MCU GPIO and EXTI Line for SPIRIT GPIOs.
 * @param  xGpio Specifies the GPIO to be configured.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: SPIRIT GPIO_0
 *         @arg M2S_GPIO_1: SPIRIT GPIO_1
 *         @arg M2S_GPIO_2: SPIRIT GPIO_2
 *         @arg M2S_GPIO_3: SPIRIT GPIO_3
 *         @arg M2S_GPIO_SDN: SPIRIT GPIO_SDN
 * @param  xGpioMode Specifies GPIO mode.
 *         This parameter can be one of following parameters:
 *         @arg M2S_MODE_GPIO_IN: MCU GPIO will be used as simple input.
 *         @arg M2S_MODE_EXTI_IN: MCU GPIO will be connected to EXTI line with interrupt
 *         generation capability.
 *         @arg M2S_MODE_GPIO_OUT: MCU GPIO will be used as simple output.
 * @retval None.
 */
void SdkEvalM2SGpioInit(M2SGpioPin xGpio, M2SGpioMode xGpioMode)
{
  GPIO_TypeDef GPIO_Type;
  GPIO_Mode_TypeDef GPIO_Mode;

  /* Check the parameters */
  assert_param(IS_M2S_GPIO_PIN(xGpio));
  assert_param(IS_M2S_GPIO_MODE(xGpioMode));
  
  /* Configures MCU GPIO */
  if(xGpioMode == M2S_MODE_GPIO_OUT)
  {
    GPIO_Mode = GPIO_Mode_Out_PP_High_Fast;
  }
  else
    if(xGpioMode == M2S_MODE_GPIO_IN)
    {
    GPIO_Mode = GPIO_Mode_In_FL_No_IT;
    }
  else
    if(xGpioMode == M2S_MODE_EXTI_IN)
  {
    GPIO_Mode = GPIO_Mode_In_FL_IT;
    EXTI_SetPinSensitivity(s_vectnM2SGpioLine[xGpio], EXTI_Trigger_Falling_Low);
  }
 
  GPIO_Init(vectpxM2SGpioPort[xGpio], s_vectnM2SGpioPin[xGpio], GPIO_Mode);
    
}


/**
 * @brief  Returns the level of a specified GPIO.
 * @param  xGpio Specifies the GPIO to be read.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: SPIRIT GPIO_0
 *         @arg M2S_GPIO_1: SPIRIT GPIO_1
 *         @arg M2S_GPIO_2: SPIRIT GPIO_2
 *         @arg M2S_GPIO_3: SPIRIT GPIO_3
 * @retval FlagStatus Level of the GPIO. This parameter can be:
 *         SET or RESET.
 */
FlagStatus SdkEvalGpioGetLevel(M2SGpioPin xGpio)
{
  /* Gets the GPIO level */
  uint16_t nDataPort = GPIO_ReadInputData(vectpxM2SGpioPort[xGpio]);
  if(nDataPort & s_vectnM2SGpioPin[xGpio])
    return SET;
  else
    return RESET;
  
}

/**
 * @brief  Sets the level of a specified GPIO.
 * @param  xGpio Specifies the GPIO to be set.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: SPIRIT GPIO_0
 *         @arg M2S_GPIO_1: SPIRIT GPIO_1
 *         @arg M2S_GPIO_2: SPIRIT GPIO_2
 *         @arg M2S_GPIO_3: SPIRIT GPIO_3
 * @param  FlagStatus Level of the GPIO. This parameter can be:
 *         SET or RESET.
 * @retval None.
 */
void SdkEvalGpioSetLevel(M2SGpioPin xGpio, FlagStatus xLevel)
{
  /* Sets the GPIO level */
  GPIO_WriteBit(vectpxM2SGpioPort[xGpio], s_vectnM2SGpioPin[xGpio], (BitAction)xLevel);
  
}

/**
 * @brief  Puts SPIRIT in shutdown mode.
 * @param  None.
 * @retval None.
 */
void SdkEvalEnterShutdown(void)
{
  /* Puts high the GPIO connected to shutdown pin of SPIRIT */
  GPIO_SetBits(M2S_GPIO_SDN_PORT, M2S_GPIO_SDN_PIN);
}


/**
 * @brief  Forces out SPIRIT from shutdown mode.
 * @param  None.
 * @retval None.
 */
void SdkEvalExitShutdown(void)
{
  /* Puts low the GPIO connected to shutdown pin of SPIRIT */
  GPIO_ResetBits(M2S_GPIO_SDN_PORT, M2S_GPIO_SDN_PIN);
  
  /* Delay to allow the circuit POR */
  for(volatile uint32_t i=0;i<0x1F00;i++);
}

FlagStatus SdkEvalCheckShutdown(void)
{
  return SdkEvalGpioGetLevel(M2S_GPIO_SDN);
}

/**
 * @}
 */

/**
 * @}
 */



/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/