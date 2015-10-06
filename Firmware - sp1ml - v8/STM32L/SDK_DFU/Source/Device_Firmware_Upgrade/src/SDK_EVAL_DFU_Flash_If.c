/**
 * @file     SDK_EVAL_DFU_Flash_If.c
 * @author   MSH RF/ART Team IMS-Systems Lab
 * @version  V1.0.0
 * @date     03/07/2011
 * @brief    Specific media access Layer for internal flash.
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
#include "SDK_EVAL_DFU_Flash_If.h"
#include "SDK_EVAL_DFU_Mal.h"


/** @addtogroup SDK_EVAL_DFU
 * @{
 */


/** @addtogroup SDK_EVAL_DFU_Flash_Interface
 * @{
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface_Private_TypesDefinitions      SDK EVAL DFU Flash Interface Private TypesDefinitions
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface_Private_Defines      SDK EVAL DFU Flash Interface Private Defines
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface_Private_Macros      SDK EVAL DFU Flash Interface Private Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface_Private_Variables     SDK EVAL DFU Flash Interface Private Variables
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface_Private_FunctionPrototypes    SDK EVAL DFU Flash Interface Private FunctionPrototypes
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Flash_Interface_Private_Functions      SDK EVAL DFU Flash Interface Private Functions
 * @{
 */

/**
 * @brief  Initializes the Media on the STM32L.
 * @param  None.
 * @retval Media Status.
 */
uint16_t SdkEvalDFUFlashIfInit(void)
{
  return MAL_OK;
  
}


/**
 * @brief  Erase sector.
 * @param  SectorAddress: Address of flash sector to erase.
 * @retval Media Status.
 */
uint16_t SdkEvalDFUFlashIfErase(uint32_t lSectorAddress)
{
  FLASH_ClearFlag(FLASH_FLAG_PGAERR | FLASH_FLAG_OPTVERR);
  FLASH_ErasePage(lSectorAddress);  
  return MAL_OK;
  
}


/**
 * @brief  Write sectors.
 * @param  SectorAddress: Address of flash sector to write.
 * @param  DataLength: Length of data to write.
 * @retval Media Status.
 */
uint16_t SdkEvalDFUFlashIfWrite(uint32_t lSectorAddress, uint32_t lDataLength)
{
  uint32_t idx = 0;
  
  __IO uint32_t* malPointer = (uint32_t *)MAL_Buffer;
  __IO uint32_t* memPointer = (uint32_t *)lSectorAddress;
  __IO uint32_t memBuffer[32]; /* Temporary buffer holding data that will be written in a half-page space */
  __IO uint32_t* mempBuffer = memBuffer;  
  __IO uint32_t* tmp;
  
  if  (lDataLength & 0x3) /* Not an aligned data */
  {
    for (idx = lDataLength; idx < ((lDataLength & 0xFFFC) + 4); idx++)
    {
      MAL_Buffer[idx] = 0xFF;
    }
  }
  
  /* Reinitialize the intermediate buffer pointer */
  mempBuffer = memBuffer;
  
  /* If the address is not aligned to half-page fill the first location with existing data */
  if (((uint32_t)memPointer & 0x7F) != 0)
  {
    /* get the aligned address */
    tmp = (uint32_t *)((uint32_t)memPointer & 0xFFFFFF80);
    
    /* Read the first part from the memory */
    while (tmp < memPointer)
    {
      *(uint32_t *)(mempBuffer++) = *(uint32_t *)(tmp++);
    }
  }    
  
  while (malPointer < (uint32_t*)(MAL_Buffer + lDataLength))
  {    
    /* Fill with the received buffer */
    while (mempBuffer < (memBuffer + 32))
    {
      /* If there are still data available in the received buffer */
      if (malPointer < ((uint32_t *)MAL_Buffer + lDataLength))
      {
        *(uint32_t *)(mempBuffer++) = *(uint32_t *)(malPointer++);
      }
      else /* no more data avaible in the received buffer: fill remaining with dummy 0 */
      {
        *(uint32_t *)(mempBuffer++) = 0;
      }
    }
    
    /* Write the buffer to the memory*/    
    FLASH_ProgramHalfPage(((uint32_t)memPointer & 0xFFFFFF80), (uint32_t *)(memBuffer));    
    
    /* Increment the memory pointer */ 
    memPointer = (uint32_t *)(((uint32_t)memPointer & 0xFFFFFF80) + (32*4));
    
    /* Reinitialize the intermediate buffer pointer */
    mempBuffer = memBuffer;
  }
  
  return MAL_OK;
  
}


/**
 * @brief  Read sectors.
 * @param  SectorAddress: Address of flash sector to read.
 * @param  DataLength: Length of data to read.
 * @retval Buffer address pointer.
 */
uint8_t *SdkEvalDFUFlashIfRead (uint32_t lSectorAddress, uint32_t lDataLength)
{
  return  (uint8_t*)(lSectorAddress);
  
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
