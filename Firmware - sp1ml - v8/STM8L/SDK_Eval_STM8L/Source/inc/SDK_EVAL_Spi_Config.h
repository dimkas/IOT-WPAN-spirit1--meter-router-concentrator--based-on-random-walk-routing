/**
 * @file    SDK_EVAL_Spirit_Spi_Config.h
 * @author  High End Analog & RF BU - AMS
 * @version V2.0.2
 * @date    Febrary 7, 2012
 * @brief   This file provides all the low level SPI API to access to SPIRIT.
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
#ifndef __SDK_EVAL_SPIRIT_SPI_CONFIG_H
#define __SDK_EVAL_SPIRIT_SPI_CONFIG_H


  /* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup SDK_EVAL_STM8L
 * @{
 */

/** @addtogroup  SDK_EVAL_SPI				 SDK EVAL Spi
 * @{
 */


#define SPIRIT_SPI_PERIPH_NB                  SPI1
#define SPIRIT_SPI_PERIPH_RCC                 CLK_Peripheral_SPI1

/* Defines for MOSI pin*/
#define SPIRIT_SPI_PERIPH_MOSI_PORT           GPIOA
#define SPIRIT_SPI_PERIPH_MOSI_PIN            GPIO_Pin_3
#define SPIRIT_SPI_PERIPH_MOSI_AF             GPIO_Mode_Out_PP_High_Fast

/* Defines for MISO pin */
#define SPIRIT_SPI_PERIPH_MISO_PORT           GPIOA
#define SPIRIT_SPI_PERIPH_MISO_PIN            GPIO_Pin_2
#define SPIRIT_SPI_PERIPH_MISO_AF             GPIO_Mode_In_FL_No_IT

/* Defines for SCLK pin */
#define SPIRIT_SPI_PERIPH_SCLK_PORT           GPIOC
#define SPIRIT_SPI_PERIPH_SCLK_PIN            GPIO_Pin_6
#define SPIRIT_SPI_PERIPH_SCLK_AF             GPIO_Mode_Out_PP_High_Fast

/* Defines for chip select pin */
#define SPIRIT_SPI_PERIPH_CS_PORT             GPIOC
#define SPIRIT_SPI_PERIPH_CS_PIN              GPIO_Pin_5
#define SPIRIT_SPI_PERIPH_CS_AF               GPIO_Mode_Out_PP_High_Fast


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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/