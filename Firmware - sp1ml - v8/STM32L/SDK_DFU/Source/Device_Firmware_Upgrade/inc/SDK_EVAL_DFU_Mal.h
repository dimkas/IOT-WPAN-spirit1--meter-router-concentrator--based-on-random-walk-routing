/**
 * @file    SDK_EVAL_DFU_Mal.h
 * @author  MSH RF/ART Team IMS-Systems Lab
 * @version V1.0.0
 * @date    03/07/2011
 * @brief   Header for SDK_EVAL_DFU_Mal.c file.
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
#ifndef __SDK_EVAL_DFU_MAL_H
#define __SDK_EVAL_DFU_MAL_H


/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "SDK_EVAL_DFU_Desc.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SDK_EVAL_DFU            SDK EVAL DFU Project
 * @{
 */


/** @defgroup SDK_EVAL_DFU_MAL          SDK EVAL DFU MAL
 * @{
 */


/** @defgroup SDK_EVAL_DFU_MAL_Exported_Types      SDK EVAL DFU MAL Exported Types
 * @{
 */
/* Insert here all the structures, enumeration and defines, macros related to types definition */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_MAL_Exported_Constants    SDK EVAL DFU MAL Exported Constants
 * @{
 */
  
#define MAL_OK   0
#define MAL_FAIL 1
#define MAX_USED_MEDIA 3
#define MAL_MASK 0xFC000000

#define INTERNAL_FLASH_BASE 0x08000000
  
/**
 * @}
 */  
  

/** @defgroup SDK_EVAL_DFU_MAL_Exported_Macros       SDK EVAL DFU MAL Exported Macros
 * @{
 */
#define _1st_BYTE(x)  (uint8_t)((x)&0xFF)             /*!< 1st addressing cycle */
#define _2nd_BYTE(x)  (uint8_t)(((x)&0xFF00)>>8)      /*!< 2nd addressing cycle */
#define _3rd_BYTE(x)  (uint8_t)(((x)&0xFF0000)>>16)   /*!< 3rd addressing cycle */
#define _4th_BYTE(x)  (uint8_t)(((x)&0xFF000000)>>24) /*!< 4th addressing cycle */
  
#define SET_POLLING_TIMING(x)   buffer[1] = _1st_BYTE(x);\
                                buffer[2] = _2nd_BYTE(x);\
                                buffer[3] = _3rd_BYTE(x);   

/**
 * @}
 */

  
/** @defgroup SDK_EVAL_DFU_MAL_Exported_Variables     SDK EVAL DFU MAL Exported Variables
 * @{
 */  
extern uint8_t  MAL_Buffer[wTransferSize]; /*!< RAM Buffer for Downloaded Data */  
  
/**
 * @}
 */

  
  
/** @defgroup SDK_EVAL_DFU_MAL_Exported_Functions     SDK EVAL DFU MAL Exported Functions
 * @{
 */
uint16_t MAL_Init (void);
uint16_t MAL_Erase (uint32_t SectorAddress);
uint16_t MAL_Write (uint32_t SectorAddress, uint32_t DataLength);
uint8_t  *MAL_Read (uint32_t SectorAddress, uint32_t DataLength);
uint16_t MAL_GetStatus(uint32_t SectorAddress ,uint8_t Cmd, uint8_t *buffer);

/**
 * @}
 */

/**
 * @}
 */


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* __SDK_EVAL_DFU_MAL_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

















/**
*
* @file    SDK_EVAL_DFU_Mal.h
* @author  ART Team IMS-Systems Lab
* @version V1.0
* @date    09/11/2010
* @brief   Header for SDK_dfu_mal.c file.
*
* @details
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
* <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
*
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_DFU_MAL_H
#define __SDK_EVAL_DFU_MAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "SDK_EVAL_DFU_Desc.h"

/** @addtogroup SDK_EVAL_DFU            SDK EVAL DFU Project
  * @{
  */


/** @defgroup SDK_EVAL_DFU_MAL
  * @{
  */

/** @defgroup SDK_EVAL_DFU_MAL_Exported_Constants    SDK EVAL DFU MAL Exported Constants
 * @{
 */
#define MAL_OK   0
#define MAL_FAIL 1
#define MAX_USED_MEDIA 3
#define MAL_MASK 0xFC000000

#define INTERNAL_FLASH_BASE 0x08000000


/**
  * @}
  */ 
   
/** @defgroup SDK_EVAL_DFU_MAL_Exported_Macros    SDK EVAL DFU MAL Exported Macros
 * @{
 */ 
   
#define _1st_BYTE(x)  (uint8_t)((x)&0xFF)             /*!< 1st addressing cycle */
#define _2nd_BYTE(x)  (uint8_t)(((x)&0xFF00)>>8)      /*!< 2nd addressing cycle */
#define _3rd_BYTE(x)  (uint8_t)(((x)&0xFF0000)>>16)   /*!< 3rd addressing cycle */
#define _4th_BYTE(x)  (uint8_t)(((x)&0xFF000000)>>24) /*!< 4th addressing cycle */
   
#define SET_POLLING_TIMING(x)   buffer[1] = _1st_BYTE(x);\
                                buffer[2] = _2nd_BYTE(x);\
                                buffer[3] = _3rd_BYTE(x);  

/**
  * @}
  */ 


/** @defgroup SDK_EVAL_DFU_MAL_Exported_Functions     SDK EVAL DFU MAL Exported Functions
 * @{
 */

/**
  * @}
  */ 
   
   
/**
  * @}
  */ 

/**
  * @}
  */ 


#ifdef __cplusplus
}
#endif

#endif /* __SDK_EVAL_DFU_MAL_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
