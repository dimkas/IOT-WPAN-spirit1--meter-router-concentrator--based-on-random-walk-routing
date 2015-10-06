/**
 * @file    SDK_EVAL_Com.c
 * @author  High End Analog & RF BU - AMS
 * @version V3.0.0
 * @date    August 7, 2012
 * @brief   This file provides all the low level API to manage the UART.
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
#include "SDK_EVAL_Com.h"

/**
 * @addtogroup SDK_EVAL_STM8L
 * @{
 */
 
/** @addtogroup SDK_EVAL_COM
 * @{
 */



#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)


/**
  * @brief Retargets the C library printf function to the USART.
  * @param[in] c Character to send
  * @retval char Character sent
  * @par Required preconditions:
  * - None
  */
PUTCHAR_PROTOTYPE
{
  /* Write a character to the USART */
  USART_SendData8(USART1, c);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

  return (c);
}
/**
  * @brief Retargets the C library scanf function to the USART.
  * @param[in] None
  * @retval char Character to Read
  * @par Required preconditions:
  * - None
  */
GETCHAR_PROTOTYPE
{
  int c = 0;
  /* Loop until the Read data register flag is SET */
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    c = USART_ReceiveData8(USART1);
    return (c);
  }


/**
 * @brief  Configures COM port.
 * @param  xCom Specifies the COM port to be configured.
 *         This parameter can be one of following parameters:
 *         @arg COM1
 * @param  xUsartInit pointer to a USART_InitTypeDef structure that
 *         contains the configuration information for the specified USART peripheral.
 * @retval None.
 */
void SdkEvalComInit(uint32_t BaudRate, USART_WordLength_TypeDef USART_WordLength,
                USART_StopBits_TypeDef USART_StopBits,
                USART_Parity_TypeDef USART_Parity,  USART_Mode_TypeDef USART_Mode)
{
  /* Enable USART clock */
  CLK_PeripheralClockConfig(EVAL_COM1_CLK, ENABLE);
  
  /* Configure USART Tx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(EVAL_COM1_TX_GPIO_PORT, EVAL_COM1_TX_PIN, ENABLE);
  
  /* Configure USART Rx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(EVAL_COM1_RX_GPIO_PORT, EVAL_COM1_RX_PIN, ENABLE);  
  USART_Init(EVAL_COM1, 
            BaudRate, 
            USART_WordLength, 
            USART_StopBits,
            USART_Parity, 
            USART_Mode);
}

/**
 * @}
 */

/**
 * @}
 */


/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
