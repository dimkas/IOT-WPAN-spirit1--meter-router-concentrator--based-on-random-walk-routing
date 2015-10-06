/**
* @file    SDK_EVAL_Spirit_Spi_Driver.c
* @author  High End Analog & RF BU - AMS
* @version V3.0.0
* @date    August 7, 2012
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
* <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
*
*/

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "SDK_EVAL_Spi_Config.h"
#include "MCU_Interface.h"

/**
 * @addtogroup SDK_EVAL_STM8L
 * @{
 */
 
/** @addtogroup  SDK_EVAL_SPI
 * @brief This file implements the SPIRIT1 Library SPI interface functions. Please see the MCU_Interface.h file of Spirit1_Library for more details.
 * @{
 */


#define CS_TO_SCLK_DELAY  0x0001
#define CLK_TO_CS_DELAY   0x0001


#define HEADER_WRITE_MASK     0x00 /*!< Write mask for header byte*/
#define HEADER_READ_MASK      0x01 /*!< Read mask for header byte*/
#define HEADER_ADDRESS_MASK   0x00 /*!< Address mask for header byte*/
#define HEADER_COMMAND_MASK   0x80 /*!< Command mask for header byte*/

#define LINEAR_FIFO_ADDRESS 0xFF  /*!< Linear FIFO address*/


#define BUILT_HEADER(add_comm, w_r) (add_comm | w_r)  /*!< macro to build the header byte*/
#define WRITE_HEADER    BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_WRITE_MASK) /*!< macro to build the write header byte*/
#define READ_HEADER     BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_READ_MASK)  /*!< macro to build the read header byte*/
#define COMMAND_HEADER  BUILT_HEADER(HEADER_COMMAND_MASK, HEADER_WRITE_MASK) /*!< macro to build the command header byte*/



#define SPI_ENTER_CRITICAL()           //SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3, 0x0F, 0x0F, DISABLE);
#define SPI_EXIT_CRITICAL()            //SdkEvalM2SGpioInterruptCmd(M2S_GPIO_3, 0x0F, 0x0F, ENABLE);


/**
* @brief  Initializes the SPI for SPIRIT
* @param  None
* @retval None
*/
void SdkEvalSpiInit(void)
{
  /* Enable SPI clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
  
  /* Configure SPI pins:SCLK, MISO and MOSI */
  GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, ENABLE);
  GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_2 | GPIO_Pin_3, ENABLE);
  
  /* Configure SPI pin: CS */
  GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);
  
  SPI_Init(SPIRIT_SPI_PERIPH_NB, SPI_FirstBit_MSB,
           SPI_BaudRatePrescaler_4,
           SPI_Mode_Master, SPI_CPOL_Low,
           SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, 0x07);
  
  SPI_Cmd(SPIRIT_SPI_PERIPH_NB, ENABLE); /* SPIRIT_SPI enable */
  
}

#define SpiritSPICSLow()        {GPIOB->ODR&=(uint8_t)(~GPIO_Pin_4);}
#define SpiritSPICSHigh()       {GPIOB->ODR|=GPIO_Pin_4;}

/**
* @brief  Write single or multiple SPIRIT register
* @param  cRegAddress: base register's address to be write
* @param  cNbBytes: number of registers and bytes to be write
* @param  pcBuffer: pointer to the buffer of values have to be written into registers
* @retval SPIRIT status
*/
StatusBytes SdkEvalSpiWriteRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t header[2];
  uint16_t tmpstatus = 0x0000;
  StatusBytes *status=(StatusBytes *)&tmpstatus;
  
  /* Built the header bytes */
  header[0]=WRITE_HEADER;
  header[1]=cRegAddress;
  
  SPI_ENTER_CRITICAL();
  
  /* Puts the SPI chip select low to start the transaction */
  SpiritSPICSLow();
  
  for(volatile uint16_t i=0;i<CS_TO_SCLK_DELAY;i++);
  
  /* Writes the header bytes and read the SPIRIT status bytes */
  for(int i=0; i<2; i++)
  {
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIRIT_SPI_PERIPH_NB, header[i]);
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_RXNE) == RESET);
    tmpstatus += ((uint16_t)(SPI_ReceiveData(SPIRIT_SPI_PERIPH_NB)))<<(i*8);
  }
  
  /* Writes the registers according to the number of bytes */
  for(int index=0; index<cNbBytes; index++)
  {
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIRIT_SPI_PERIPH_NB, pcBuffer[index]);   
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_RXNE) == RESET);
    SPI_ReceiveData(SPIRIT_SPI_PERIPH_NB);
  }
  
  /* Puts the SPI chip select high to end the transaction */
  SpiritSPICSHigh();
  
  /* To be sure to don't rise the Chip Select before the end of last sending */
  while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
  
  SPI_EXIT_CRITICAL();
  
  return *status;
  
}

/**
* @brief  Read single or multiple SPIRIT register
* @param  cRegAddress: base register's address to be read
* @param  cNbBytes: number of registers and bytes to be read
* @param  pcBuffer: pointer to the buffer of registers' values read
* @retval SPIRIT status
*/
StatusBytes SdkEvalSpiReadRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint16_t tmpstatus = 0x0000;
  StatusBytes *status=(StatusBytes *)&tmpstatus;
  
  uint8_t header[2];
  uint8_t dummy=0xFF;
  
  /* Built the header bytes */
  header[0]=READ_HEADER;
  header[1]=cRegAddress;
  
  SPI_ENTER_CRITICAL();
  
  /* Put the SPI chip select low to start the transaction */
  SpiritSPICSLow();
  
  for(volatile uint16_t i=0;i<CS_TO_SCLK_DELAY;i++);
  
  /* Write the header bytes and read the SPIRIT status bytes */
  for(int i=0; i<2; i++)
  {
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIRIT_SPI_PERIPH_NB, header[i]);
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_RXNE) == RESET);
    tmpstatus += ((uint16_t)(SPI_ReceiveData(SPIRIT_SPI_PERIPH_NB)))<<(i*8);
  }
  
  /* Read the registers according to the number of bytes */
  for(int index=0; index<cNbBytes; index++)
  {
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIRIT_SPI_PERIPH_NB, dummy);
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_RXNE) == RESET);
    *pcBuffer = SPI_ReceiveData(SPIRIT_SPI_PERIPH_NB);
    pcBuffer++;
  }
  
  while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  SpiritSPICSHigh();
  
  SPI_EXIT_CRITICAL();
  
  return *status;
  
}

/**
* @brief  Send a command
* @param  cCommandCode: command code to be sent
* @retval SPIRIT status
*/
StatusBytes SdkEvalSpiCommandStrobes(uint8_t cCommandCode)
{
  uint8_t header[2];
  uint16_t tmpstatus = 0x0000;
  StatusBytes *status=(StatusBytes *)&tmpstatus;
  
  /* Built the header bytes */
  header[0]=COMMAND_HEADER;
  header[1]=cCommandCode;
  
  SPI_ENTER_CRITICAL();
  
  /* Puts the SPI chip select low to start the transaction */
  SpiritSPICSLow();
  
  for(volatile uint16_t i=0;i<CS_TO_SCLK_DELAY;i++);
  
  /* Writes the header bytes and read the SPIRIT status bytes */
  for(int i=0; i<2; i++)
  {
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIRIT_SPI_PERIPH_NB, header[i]);
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_RXNE) == RESET);
    tmpstatus += ((uint16_t)(SPI_ReceiveData(SPIRIT_SPI_PERIPH_NB)))<<(i*8);
  }
  
  while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
  
  /* Puts the SPI chip select high to end the transaction */
  SpiritSPICSHigh();
  
  SPI_EXIT_CRITICAL();
  
  return *status;
  
}


/**
* @brief  Write data into TX FIFO
* @param  cNbBytes: number of bytes to be written into TX FIFO
* @param  pcBuffer: pointer to data to write
* @retval SPIRIT status
*/
StatusBytes SdkEvalSpiWriteFifo(uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint16_t tmpstatus = 0x0000;
  StatusBytes *status=(StatusBytes *)&tmpstatus;
  
  uint8_t header[2];
  
  /* Built the header bytes */
  header[0]=WRITE_HEADER;
  header[1]=LINEAR_FIFO_ADDRESS;
  
  SPI_ENTER_CRITICAL();
  
  /* Put the SPI chip select low to start the transaction */
  SpiritSPICSLow();
  
  for(volatile uint16_t i=0;i<CS_TO_SCLK_DELAY;i++);
  
  /* Write the header bytes and read the SPIRIT status bytes */
  for(int i=0; i<2; i++)
  {
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIRIT_SPI_PERIPH_NB, header[i]);
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_RXNE) == RESET);
    tmpstatus += ((uint16_t)(SPI_ReceiveData(SPIRIT_SPI_PERIPH_NB)))<<(i*8);
  }
  
  /* Write the data into the FIFO according to the number of bytes */
  for(int index=0; index<cNbBytes; index++)
  {
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIRIT_SPI_PERIPH_NB, pcBuffer[index]);
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_RXNE) == RESET);
    SPI_ReceiveData(SPIRIT_SPI_PERIPH_NB);
  }
  
  /* To be sure to don't rise the Chip Select before the end of last sending */
  while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  SpiritSPICSHigh();
  
  SPI_EXIT_CRITICAL();
  
  return *status;
  
}


/**
* @brief  Read data from RX FIFO
* @param  cNbBytes: number of bytes to read from RX FIFO
* @param  pcBuffer: pointer to data read from RX FIFO
* @retval SPIRIT status
*/
StatusBytes SdkEvalSpiReadFifo(uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint16_t tmpstatus = 0x0000;
  StatusBytes *status=(StatusBytes *)&tmpstatus;
  
  SPI_ENTER_CRITICAL();
  
  uint8_t header[2];
  uint8_t dummy=0xFF;
  
  /* Built the header bytes */
  header[0]=READ_HEADER;
  header[1]=LINEAR_FIFO_ADDRESS;
  
  /* Put the SPI chip select low to start the transaction */
  SpiritSPICSLow();
  
  for(volatile uint16_t i=0;i<CS_TO_SCLK_DELAY;i++);
  
  /* Write the header bytes and read the SPIRIT status bytes */
  for(int i=0; i<2; i++)
  {
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIRIT_SPI_PERIPH_NB, header[i]);
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_RXNE) == RESET);
    tmpstatus += ((uint16_t)(SPI_ReceiveData(SPIRIT_SPI_PERIPH_NB)))<<(i*8);
  }
  
  /* Read the data from the FIFO according to the number of bytes */
  for(int index=0; index<cNbBytes; index++)
  {
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIRIT_SPI_PERIPH_NB, dummy);
    while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_RXNE) == RESET);
    *pcBuffer = SPI_ReceiveData(SPIRIT_SPI_PERIPH_NB);
    pcBuffer++;
  }
  
  /* To be sure to don't rise the Chip Select before the end of last sending */
  while (SPI_GetFlagStatus(SPIRIT_SPI_PERIPH_NB, SPI_FLAG_TXE) == RESET);
  
  /* Put the SPI chip select high to end the transaction */
  SpiritSPICSHigh();
  
  SPI_EXIT_CRITICAL();
  
  return *status;
  
}

/**
 * @}
 */

/**
* @}
*/



/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/