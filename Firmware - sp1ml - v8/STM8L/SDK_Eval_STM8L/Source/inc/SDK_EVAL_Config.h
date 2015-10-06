/**
 * @file    SDK_EVAL_Config.h
 * @author  High End Analog & RF BU - AMS
 * @version V2.0.2
 * @date    Febrary 7, 2012
 * @brief   This file contains some configurations and useful defines.
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
#ifndef __SDK_EVAL_CONFIG_H
#define __SDK_EVAL_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Com.h"
#include "SDK_EVAL_Led.h"
#include "SDK_EVAL_Gpio.h"
#include "SDK_EVAL_Timers.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup SDK_EVAL_STM8L 	SDK EVAL STM8L
 * @brief This module is used to support STM8L microcontroller family, in 
 * particular, the STM8L discovery board is supported.
 * @details The pins connection is the follows:
 * <table border="1">
 * <tr>
 *   <th>Signal name</th>
 *   <th>STEVAL-IKR002Vx - RF module</th>
 *   <th>STM8L-DISCOVERY connector</th>  
 *   <th>STM8L pin name</th>    
 * </tr>
 * <tr>
 *   <td>SPI CSn</td>
 *   <td>J7, pin 3</td>
 *   <td>P3, pin 7</td>
 *   <td>PB4</td>  
 * </tr>
 * <tr>
 *   <td>SPI SCLK</td>
 *   <td>J7, pin 5</td>
 *   <td>P3, pin 8</td>
 *   <td>PB5</td>  
 * </tr>
  * <tr>
 *   <td>SPI MOSI</td>
 *   <td>J7, pin 9</td>
 *   <td>P3, pin 9</td>
 *   <td>PB6</td>  
 * </tr>
 * <tr>
 *   <td>SPI MISO</td>
 *   <td>J7, pin 7</td>
 *   <td>P3, pin 10</td>
 *   <td>PB7</td>  
 * </tr>
 * <tr>
 *   <td>SPIRIT1 SDN</td>
 *   <td>J6, pin 9</td>
 *   <td>P2, pin 12</td>
 *   <td>PC1</td>  
 * </tr>
 * <tr>
 *   <td>SPIRIT1 GPIO_0</td>
 *   <td>J7, pin 10</td>
 *   <td>P2, pin 4</td>
 *   <td>PE7</td>  
 * </tr>
 * <tr>
 *   <td>SPIRIT1 GPIO_1</td>
 *   <td>J7, pin 8</td>
 *   <td>P2, pin 5</td>
 *   <td>PE6</td>  
 * </tr>
 * <tr>
 *   <td>SPIRIT1 GPIO_2</td>
 *   <td>J7, pin 6</td>
 *   <td>P2, pin 6</td>
 *   <td>PC7</td>  
 * </tr>
 * <tr>
 *   <td>SPIRIT1 GPIO_3</td>
 *   <td>J7, pin 4</td>
 *   <td>P2, pin 9</td>
 *   <td>PC4</td>  
 * </tr>
 * <tr>
 *   <td>SPIRIT1 GND</td>
 *   <td>J6, pin 5/7</td>
 *   <td>GND</td>
 *   <td>GND</td>  
 * </tr>
 * <tr>
 *   <td>SPIRIT1 VBAT</td>
 *   <td>J6, pin 8/10</td>
 *   <td>3.3 VDD</td>
 *   <td>3.3 VDD</td>  
 * </tr>
 * </table>
 * @{  
 */

/** @addtogroup SDK_EVAL_Config         SDK EVAL Config
 * @brief SDK EVAL configuration.
 * @details See the file <i>@ref SDK_EVAL_Config.h</i> for more details.
 * @{
 */
  
 
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
