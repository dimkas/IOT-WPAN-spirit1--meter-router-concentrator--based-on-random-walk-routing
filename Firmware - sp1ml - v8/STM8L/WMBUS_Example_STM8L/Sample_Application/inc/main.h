/**
 * @file    main.h
 * @author  MSH RF/IMS-Systems Lab
 * @version V1.0.0
 * @date    05/04/2011
 * @brief   This file contains header for main.c  
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
#ifndef __MAIN_H
#define __MAIN_H
/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "wmbus_appli.h"


/** @defgroup MAIN_Exported_Types
* @{
*/
//typedef enum{
// DEVICE_TYPE =(uint8_t)0x01,
// WMBUS_MODE, 
// DEVICE_MODE,
// RF_POWER
//}ConfigurationParam_T;
/**
* @}
*/

/** @defgroup MAIN_Exported_FunctionsPrototype
* @{
*/
extern void Set_SELStatus(void);
extern void Set_UPStatus(void);
extern void Set_DOWNStatus(void);
extern void SetLeftKeyStatus(void);
extern void Set_RightStatus(void);
extern void Set_KeyStatus(void);
extern void Set_DataStatus(void);
void Set_LEFTStatus(void);
/**
* @}
*/
#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
