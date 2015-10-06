/**
* @file    SPIRIT_SDK_EEPROM.h
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   SDK EVAL eeprom management
* @details This module exports API to manage the EEPROM of the eval boards.
*          Data stored in the EEPROM are mainly some manifacturing infos,
*          and informations that can be useful when developing applications 
*          with the daughter board. Some of them are the RF band, the offset
*          of the carrier from the nominal frequency and the XTAL frequency.
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
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SPIRIT_SDK_EEPROM_H_
#define SPIRIT_SDK_EEPROM_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"


#ifdef __cplusplus
  "C" {
#endif


/**
 * @addtogroup SDK_EVAL_STM32L
 * @{
 */


/**
 * @defgroup SDK_EEPROM
 * @brief Management of Software Development Kit eval board EEPROM.
 * @details See the file <i>@ref SDK_EVAL_EEPROM.h</i> for more details.
 * @{
 */
    
#define EEPROM_V3_SPI_PERIPH_NB                  SPI1
#define EEPROM_V3_SPI_PERIPH_RCC                 RCC_APB2Periph_SPI1
  

/* Defines for MOSI pin*/
#define EEPROM_V3_SPI_PERIPH_MOSI_PORT           GPIOA
#define EEPROM_V3_SPI_PERIPH_MOSI_PIN            GPIO_Pin_7
#define EEPROM_V3_SPI_PERIPH_MOSI_AF             GPIO_AF_SPI1
#define EEPROM_V3_SPI_PERIPH_MOSI_RCC            RCC_AHBPeriph_GPIOA
#define EEPROM_V3_SPI_PERIPH_MOSI_RCC_SOURCE     GPIO_PinSource7

/* Defines for MISO pin */
#define EEPROM_V3_SPI_PERIPH_MISO_PORT           GPIOA
#define EEPROM_V3_SPI_PERIPH_MISO_PIN            GPIO_Pin_6
#define EEPROM_V3_SPI_PERIPH_MISO_AF             GPIO_AF_SPI1
#define EEPROM_V3_SPI_PERIPH_MISO_RCC            RCC_AHBPeriph_GPIOA
#define EEPROM_V3_SPI_PERIPH_MISO_RCC_SOURCE     GPIO_PinSource6

/* Defines for SCLK pin */
#define EEPROM_V3_SPI_PERIPH_SCLK_PORT           GPIOA
#define EEPROM_V3_SPI_PERIPH_SCLK_PIN            GPIO_Pin_5
#define EEPROM_V3_SPI_PERIPH_SCLK_AF             GPIO_AF_SPI1
#define EEPROM_V3_SPI_PERIPH_SCLK_RCC            RCC_AHBPeriph_GPIOA
#define EEPROM_V3_SPI_PERIPH_SCLK_RCC_SOURCE     GPIO_PinSource5

/* Defines for chip select pin */
#define EEPROM_V3_SPI_PERIPH_CS_PORT             GPIOA
#define EEPROM_V3_SPI_PERIPH_CS_PIN              GPIO_Pin_4
#define EEPROM_V3_SPI_PERIPH_CS_RCC              RCC_AHBPeriph_GPIOA
#define EEPROM_V3_SPI_PERIPH_CS_RCC_SOURCE       GPIO_PinSource4


#define EEPROM_V2_SPI_PERIPH_NB                  SPI1
#define EEPROM_V2_SPI_PERIPH_RCC                 RCC_APB2Periph_SPI1

/* Defines for MOSI pin*/
#define EEPROM_V2_SPI_PERIPH_MOSI_PORT           GPIOE
#define EEPROM_V2_SPI_PERIPH_MOSI_PIN            GPIO_Pin_15
#define EEPROM_V2_SPI_PERIPH_MOSI_AF             GPIO_AF_SPI1
#define EEPROM_V2_SPI_PERIPH_MOSI_RCC            RCC_AHBPeriph_GPIOE
#define EEPROM_V2_SPI_PERIPH_MOSI_RCC_SOURCE     GPIO_PinSource15

/* Defines for MISO pin */
#define EEPROM_V2_SPI_PERIPH_MISO_PORT           GPIOE
#define EEPROM_V2_SPI_PERIPH_MISO_PIN            GPIO_Pin_14
#define EEPROM_V2_SPI_PERIPH_MISO_AF             GPIO_AF_SPI1
#define EEPROM_V2_SPI_PERIPH_MISO_RCC            RCC_AHBPeriph_GPIOE
#define EEPROM_V2_SPI_PERIPH_MISO_RCC_SOURCE     GPIO_PinSource14

/* Defines for SCLK pin */
#define EEPROM_V2_SPI_PERIPH_SCLK_PORT           GPIOE
#define EEPROM_V2_SPI_PERIPH_SCLK_PIN            GPIO_Pin_13
#define EEPROM_V2_SPI_PERIPH_SCLK_AF             GPIO_AF_SPI1
#define EEPROM_V2_SPI_PERIPH_SCLK_RCC            RCC_AHBPeriph_GPIOE
#define EEPROM_V2_SPI_PERIPH_SCLK_RCC_SOURCE     GPIO_PinSource13

/* Defines for chip select pin */
#define EEPROM_V2_SPI_PERIPH_CS_PORT             GPIOC
#define EEPROM_V2_SPI_PERIPH_CS_PIN              GPIO_Pin_12
#define EEPROM_V2_SPI_PERIPH_CS_RCC              RCC_AHBPeriph_GPIOC
#define EEPROM_V2_SPI_PERIPH_CS_RCC_SOURCE       GPIO_PinSource12

    
/* Defines for chip select pin */
#define EEPROM_VD1_SPI_PERIPH_CS_PORT             GPIOA
#define EEPROM_VD1_SPI_PERIPH_CS_PIN              GPIO_Pin_9
#define EEPROM_VD1_SPI_PERIPH_CS_RCC              RCC_AHBPeriph_GPIOA
#define EEPROM_VD1_SPI_PERIPH_CS_RCC_SOURCE       GPIO_PinSource9
    
    
/* EEPROM SPI commands */
#define EEPROM_CMD_WREN    0x06    // Write Enable
#define EEPROM_CMD_WRDI    0x04    // Write Disable
#define EEPROM_CMD_RDSR    0x05    // Read Status Register
#define EEPROM_CMD_WRSR    0x01    // Write Status Register
#define EEPROM_CMD_READ    0x03    // Read from Memory Array
#define EEPROM_CMD_WRITE   0x02    // Write to Memory Array   

/* EEPROM SPI status */
#define EEPROM_STATUS_SRWD    0x80       // Status Register Write Disable
#define EEPROM_STATUS_BP      0x0C       // Block Protect
#define EEPROM_STATUS_WEL     0x02       // Write Enable   
#define EEPROM_STATUS_WIP     0x01       // Write in Progress


void EepromSpiInitialization(void);
void EepromCsPinInitialization(void);

uint8_t EepromStatus(void);
void EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
void EepromWrite(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
uint8_t EepromResetSrwd(void);
uint8_t EepromSetSrwd(void);

uint8_t SdkEvalGetHasEeprom(void);
void SdkEvalSetHasEeprom(uint8_t eeprom);

/**
* @}
*/

/**
* @}
*/

#ifdef __cplusplus
}
#endif


#endif


 /******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

