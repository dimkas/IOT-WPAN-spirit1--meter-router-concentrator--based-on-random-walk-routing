/**
******************************************************************************
* @file    Serial.c
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  This file provides all the low level API to manage USART Communication
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
/* Includes ------------------------------------------------------------------*/
#include "Serial.h"
/** @defgroup SERIAL
* @brief This file is the interface between AT Prepro and USART PHY Layer.
* @{
*/
/** @defgroup SERIAL_Private_Variables
* @{
*/
uint32_t INDEX_Rx=0,INDEX_Tx=0,ITERATOR;
uint8_t ReceiveBuffer[BUFFERSIZE];
uint8_t TransmitBuffer[BUFFERSIZE];
uint32_t SerialBaudRate = 115200; 
uint16_t SerialWordLength = USART_WordLength_8b; 
uint16_t SerialStopBits =  USART_StopBits_1; 
uint16_t SerialParity = USART_Parity_No; 
uint16_t SerialHardwareFlowControl = USART_HardwareFlowControl_None; 
/**
* @}
*/
/** @defgroup SERIAL_Private_Functions
* @{
*/
/**
* @brief  Adds an element to the RxBuffer.
* @param  element to be added.
* @retval None
*/
void RxBuffer_addElement(uint8_t value)
{
  if(!RxBuffer_IsFull()){
    ReceiveBuffer[INDEX_Rx++] = value;
  }
  else {
   DisableRx();
  }
}

/**
* @brief  Reads an element to the RxBuffer.
* @param  uint32_t index: denoting the character that user wants to read from 
*                         the received buffer.
* @retval uint8_t value.
*/
uint8_t RxBuffer_readElement(uint32_t index)
{
  if((index < BUFFERSIZE)&&(index < INDEX_Rx))
  {
    return ReceiveBuffer[index];
  }
  return 0x00;
}
/**
* @brief This routine checks whether the Rx Buffer is full or not.
* @param  None.
* @retval uint8_t value.
*/
uint8_t RxBuffer_IsFull(void)
{
  if(INDEX_Rx < BUFFERSIZE)
  {
    return 0;
  }
  else {
    return 1; 
  }
}

/**
* @brief This routine clears Rx index.
* @param  None.
* @retval None.
*/
void RxBuffer_clear(void)
{
  INDEX_Rx = 0;
}
/**
* @brief This routine returns the length of data in Rx Buffer.
* @param  None.
* @retval uint32_t length.
*/
uint32_t RxBuffer_getLength(void)
{
  return (INDEX_Rx-1);  
}

/**
* @brief This routine decrements the Rx index.
* @param  None.
* @retval None.
*/
void RxBuffer_deleteElement(void)
{
  INDEX_Rx--;
}
/**
* @brief This routine returns the pointer to Rx Buffer.
* @param  None.
* @retval uint8_t*: pointer to the RxBuffer.
*/
uint8_t *getRxBuffer(void)
{
  return (uint8_t *)ReceiveBuffer;
}

/**
* @brief  Adds an element to the TxBuffer.
* @param  element to be added.
* @retval None
*/
void TxBuffer_addElement(uint8_t value)
{
  if(!TxBuffer_IsFull())
  {
    TransmitBuffer[INDEX_Tx++] = value;
  }
  else 
  {
    /* ADD A WAIT STATE TO ALLOW Tx TO REDUCE LOAD */   
  }
}

/**
* @brief  This routine will transmit the element from Tx Buffer.
* @param  None.
* @retval uint8_t Status.
*/
uint8_t TxBuffer_SendElement(void)
{
  if(ITERATOR < INDEX_Tx)
  {
    return TransmitBuffer[ITERATOR++];
  }
  else 
  {
    DisableTx();
  }
  return 0x00;
}

/**
* @brief  This routine will checks whether transmit buffer is full.
* @param  None.
* @retval uint8_t Status.
*/
uint8_t TxBuffer_IsFull(void)
{
  if(INDEX_Tx < BUFFERSIZE)
  {
    return 0;
  }
  else {
    return 1; 
  }
}

/**
* @brief  This routine will clears transmit buffer index and iterator.
* @param  None.
* @retval None.
*/
void TxBuffer_clear(void)
{
  INDEX_Tx = 0;
  ITERATOR = 0;
}

/**
* @brief  This routine will disable the USART Rx interrupt.
* @param  None.
* @retval None.
*/
void DisableRx(void)
{
  USART_ITConfig(AT_COM, USART_IT_RXNE  , DISABLE);
}

/**
* @brief  This routine will disable the USART Tx interrupt.
* @param  None.
* @retval None.
*/
void DisableTx(void)
{
  USART_ITConfig(AT_COM, USART_IT_TXE  , DISABLE);
}


/**
* @brief  This routine will enable the USART Rx interrupt.
* @param  None.
* @retval None.
*/
void EnableRx(void)
{
  USART_ITConfig(AT_COM, USART_IT_RXNE  , ENABLE);
}

/**
* @brief  This routine will enable the USART Tx interrupt.
* @param  None.
* @retval None.
*/
void EnableTx(void)
{
  USART_ITConfig(AT_COM, USART_IT_TXE  , ENABLE);
}
/**
* @brief  This routine will reconfigure the USART interface.
* @param  None.
* @retval None.
*/
void USART_ReConfig(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  USART_DeInit(USART2);
  
  USART_InitStructure.USART_BaudRate = SerialBaudRate;
  USART_InitStructure.USART_WordLength = SerialWordLength;
  USART_InitStructure.USART_StopBits = SerialStopBits;
  USART_InitStructure.USART_Parity = SerialParity;
  USART_InitStructure.USART_HardwareFlowControl = SerialHardwareFlowControl;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART2, &USART_InitStructure);
  
  /* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_ITConfig(USART2, USART_IT_RXNE  , ENABLE);
  USART_ITConfig(USART2, USART_IT_TXE  , DISABLE);
  
  /* Enable USART */
  USART_Cmd(USART2, ENABLE);
}
/**
* @}
*/
/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/