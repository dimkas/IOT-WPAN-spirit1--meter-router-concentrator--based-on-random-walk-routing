/**
 * @file     SDK_EVAL_DFU_Mal.c
 * @author   MSH RF/ART Team IMS-Systems Lab
 * @version  V1.0.0
 * @date     03/07/2011
 * @brief    Generic media access Layer.
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
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_DFU_Mal.h"
#include "usb_lib.h"
#include "usb_type.h"
#include "SDK_EVAL_DFU_Desc.h"
#include "SDK_EVAL_DFU_Flash_If.h"


/** @addtogroup SDK_EVAL_DFU
 * @{
 */


/** @addtogroup SDK_EVAL_DFU_MAL
 * @{
 */


/** @defgroup SDK_EVAL_DFU_MAL_Private_TypesDefinitions      SDK EVAL DFU MAL Private TypesDefinitions
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_MAL_Private_Defines      SDK EVAL DFU MAL Private Defines
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_MAL_Private_Macros      SDK EVAL DFU MAL Private Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_MAL_Private_Variables     SDK EVAL DFU MAL Private Variables
 * @{
 */

uint16_t (*pMAL_Init) (void);
uint16_t (*pMAL_Erase) (uint32_t SectorAddress);
uint16_t (*pMAL_Write) (uint32_t SectorAddress, uint32_t DataLength);
uint8_t  *(*pMAL_Read)  (uint32_t SectorAddress, uint32_t DataLength);
uint8_t  MAL_Buffer[wTransferSize]; /*!< RAM Buffer for Downloaded Data */

extern ONE_DESCRIPTOR DFU_String_Descriptor[5];

/**
 * @brief   This table holds the Typical Sector Erase and 1024 Bytes Write timings.
 *          These timings will be returned to the host when it checks the device
 *          status during a write or erase operation to know how much time the host
 *          should wait before issuing the next get status request. 
 *          These defines are set in usb_conf.h file.
 */


static const uint16_t  TimingTable[1][2] =
  {
    {  INTERN_FLASH_SECTOR_ERASE_TIME , INTERN_FLASH_SECTOR_WRITE_TIME }, /*!< Internal Flash */
  };

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_MAL_Private_FunctionPrototypes    SDK EVAL DFU MAL Private FunctionPrototypes
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_MAL_Private_Functions      SDK EVAL DFU MAL Private Functions
 * @{
 */

/**
 * @brief  Initializes the Media on the STM32.
 * @param  None.
 * @retval Media Status.
 */
uint16_t MAL_Init(void)
{

  SdkEvalDFUFlashIfInit(); /*!< Internal Flash */

  return MAL_OK;
}


/**
 * @brief  Erase sectors.
 * @param  SectorAddress: Address of flash sector to read.
 * @retval Media Status.
 */
uint16_t MAL_Erase(uint32_t SectorAddress)
{

  switch (SectorAddress & MAL_MASK)
  {
    case INTERNAL_FLASH_BASE:
      pMAL_Erase = SdkEvalDFUFlashIfErase;
      break;
      
    default:
      return MAL_FAIL;
  }
  return pMAL_Erase(SectorAddress);
}


/**
 * @brief  Write sectors.
 * @param  SectorAddress: Address of flash sector to read.
 * @retval Media Status.
 */
uint16_t MAL_Write (uint32_t SectorAddress, uint32_t DataLength)
{

  switch (SectorAddress & MAL_MASK)
  {
    case INTERNAL_FLASH_BASE:
      pMAL_Write = SdkEvalDFUFlashIfWrite;
      break;

    default:
      return MAL_FAIL;
  }
  return pMAL_Write(SectorAddress, DataLength);
}


/**
 * @brief  Read sectors.
 * @param  SectorAddress: Address of flash sector to read.
 * @param  DataLength: Length of data to read.
 * @retval Buffer address pointer.
 */
uint8_t *MAL_Read (uint32_t SectorAddress, uint32_t DataLength)
{

  switch (SectorAddress & MAL_MASK)
  {
    case INTERNAL_FLASH_BASE:
      pMAL_Read = SdkEvalDFUFlashIfRead;
      break;

    default:
      return 0;
  }
  return pMAL_Read (SectorAddress, DataLength);
}


/**
 * @brief  Get Status.
 * @param  SectorAddress: Address of flash sector to read.
 * @param  Cmd: Type of command.
 * @param  buffer: Buffer address pointer.
 * @retval Media Status.
 */
uint16_t MAL_GetStatus(uint32_t SectorAddress , uint8_t Cmd, uint8_t *buffer)
{
  /* !< In order to avoid to change the function prototype, the
  evaluation of the x value is left commented. In SDK the only memory
  supported is the internal Flash */
  /* 0x000000000 --> 0, 0x640000000 --> 1, 0x080000000 --> 2 */
//  uint8_t x = (SectorAddress  >> 26) & 0x03 ; 

  uint8_t y = Cmd & 0x01;
  
  SET_POLLING_TIMING(TimingTable[0][y]);  /*!< x: Erase Write Timing y: Media*/
  return MAL_OK;
}

/**
 * @}
 */


/**
 * @}
 */


/**
 * @}
 */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
