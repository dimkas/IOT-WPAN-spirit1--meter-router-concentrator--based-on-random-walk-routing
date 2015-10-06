/**
******************************************************************************
* @file    Serial.h
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  This file contains header for serial.c
*
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software 
* distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SERIAL_H
#define __SERIAL_H
/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"


/**
 * @defgroup AT_CMD_SERIAL AT Command serial port lower layer
 * @brief Configuration and management of AT Command serial port lower layer.
 * @details See the file <i>@ref serial.h</i> for more details.
 * @{
 */
/**
 * @defgroup AT_CMD_SERIAL AT Command serial port lower layer exported defines
 * @{
 */
#define BUFFERSIZE 255
#define AT_COM USART2
/**
 *@}
 */

/**
 * @defgroup AT_CMD_SERIAL AT Command serial port lower layer Exported variables
 * @{
 */
extern uint32_t SerialBaudRate;
extern uint16_t SerialWordLength;
extern uint16_t SerialStopBits;
extern uint16_t SerialParity;
extern uint16_t SerialHardwareFlowControl;
/**
 *@}
 */
/** @defgroup AT_CMD_SERIAL_Functions AT Command serial port lower layer Exported functions
 * @{
 */
void RxBuffer_addElement(uint8_t value);
uint8_t RxBuffer_readElement(uint32_t index);
void RxBuffer_deleteElement(void);
uint32_t RxBuffer_getLength(void);
uint8_t RxBuffer_IsFull(void);
void RxBuffer_clear(void);
void TxBuffer_addElement(uint8_t value);
uint8_t TxBuffer_SendElement(void);
uint8_t TxBuffer_IsFull(void);
void TxBuffer_clear(void);
void DisableRx(void);
void DisableTx(void);
void EnableRx(void);
void EnableTx(void);
uint8_t *getRxBuffer(void);
extern void USART_ReConfig(void);
/**
 * @}
 */

/**
 * @}
 */
#endif/* __SERIAL_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/