/**
 * @file    SDK_EVAL_DFU_Pwr.h
 * @author  MSH RF/ART Team IMS-Systems Lab
 * @version V1.0.0
 * @date    03/07/2011
 * @brief   Connection/disconnection & power management header.
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
#ifndef __SDK_EVAL_DFU_PWR_H
#define __SDK_EVAL_DFU_PWR_H


/* Includes ------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SDK_EVAL_DFU            SDK EVAL DFU Project
 * @{
 */


/** @defgroup SDK_EVAL_DFU_POWER        SDK EVAL DFU POWER
 * @{
 */


/** @defgroup SDK_EVAL_DFU_POWER_Exported_Types      SDK EVAL DFU POWER Exported Types
 * @{
 */
typedef enum _RESUME_STATE
{
  RESUME_EXTERNAL,
  RESUME_INTERNAL,
  RESUME_LATER,
  RESUME_WAIT,
  RESUME_START,
  RESUME_ON,
  RESUME_OFF,
  RESUME_ESOF
} RESUME_STATE;

typedef enum _DEVICE_STATE
{
  UNCONNECTED,
  ATTACHED,
  POWERED,
  SUSPENDED,
  ADDRESSED,
  CONFIGURED
} DEVICE_STATE;

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_POWER_Exported_Constants    SDK EVAL DFU POWER Exported Constants
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_POWER_Exported_Macros       SDK EVAL DFU POWER Exported Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_POWER_Exported_Variables     SDK EVAL DFU POWER Exported Variables
 * @{
 */ 
extern  __IO uint32_t bDeviceState;             /*!< USB device status */
extern __IO bool fSuspendEnabled;  /*!< true when suspend is possible */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_POWER_Exported_Functions     SDK EVAL DFU POWER Exported Functions
 * @{
 */
void Suspend(void);
void Resume_Init(void);
void Resume(RESUME_STATE eResumeSetVal);
RESULT PowerOn(void);
RESULT PowerOff(void);

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

#endif /* __SDK_EVAL_DFU_PWR_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

