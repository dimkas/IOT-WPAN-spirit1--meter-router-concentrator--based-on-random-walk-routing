/**
 * @file    main.h
 * @author  HEA & RF BU - IMS Systems Lab
 * @version V1.0.4
 * @date    April 02, 2012
 * @brief   This file includes routines for main application.
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
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#include "wmbus_appli.h"




/** @defgroup MAIN_Exported_FunctionsPrototype
* @{
*/
extern void Set_SELStatus(void);
extern void Set_UPStatus(void);
extern void Set_DOWNStatus(void);
extern void SetLeftKeyStatus(void);
extern void Set_RightStatus(void);
extern void Set_KeyStatus(void);
/**
* @}
*/
#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
