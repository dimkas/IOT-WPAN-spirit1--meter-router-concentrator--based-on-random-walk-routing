/**
 * @file    SDK_EVAL_Com.h
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
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_COM_H
#define __SDK_EVAL_COM_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @addtogroup SDK_EVAL_STM8L
 * @{
 */

/** @addtogroup SDK_EVAL_COM			SDK EVAL Com
 * @brief Management of STM8L discovery COM ports.
 * @details See the file <i>@ref SDK_EVAL_Com.h</i> for more details.
 * @{
 */


/**
 * @brief Definition for connected to USART1
 */
#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    CLK_Peripheral_USART1
#define EVAL_COM1_TX_PIN                 GPIO_Pin_3
#define EVAL_COM1_TX_GPIO_PORT           GPIOC
#define EVAL_COM1_RX_PIN                 GPIO_Pin_2
#define EVAL_COM1_RX_GPIO_PORT           GPIOC

void SdkEvalComInit(uint32_t BaudRate, USART_WordLength_TypeDef
                USART_WordLength, USART_StopBits_TypeDef USART_StopBits,
                USART_Parity_TypeDef USART_Parity,  USART_Mode_TypeDef USART_Mode);

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