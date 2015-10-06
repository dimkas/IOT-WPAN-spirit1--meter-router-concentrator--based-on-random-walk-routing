/**
 * @file    SDK_EVAL_DFU_Istr.h
 * @author  MSH RF/ART Team IMS-Systems Lab
 * @version V1.0.0
 * @date    03/07/2011
 * @brief   Header for SDK_EVAL_DFU_Istr.c file.
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
#ifndef __SDK_EVAL_DFU_ISTR_H
#define __SDK_EVAL_DFU_ISTR_H


/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SDK_EVAL_DFU            SDK EVAL DFU Project
 * @{
 */


/** @defgroup SDK_EVAL_DFU_Istr         SDK EVAL DFU Istr
 * @{
 */


/** @defgroup SDK_EVAL_DFU_Istr_Exported_Types      SDK EVAL DFU Istr Exported Types
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Istr_Exported_Constants    SDK EVAL DFU Istr Exported Constants
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Istr_Exported_Macros       SDK EVAL DFU Istr Exported Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Istr_Exported_Functions     SDK EVAL DFU Istr Exported Functions
 * @{
 */

 void USB_Istr(void);

/* function prototypes Automatically built defining related macros */

void EP1_IN_Callback(void);
void EP2_IN_Callback(void);
void EP3_IN_Callback(void);
void EP4_IN_Callback(void);
void EP5_IN_Callback(void);
void EP6_IN_Callback(void);
void EP7_IN_Callback(void);

void EP1_OUT_Callback(void);
void EP2_OUT_Callback(void);
void EP3_OUT_Callback(void);
void EP4_OUT_Callback(void);
void EP5_OUT_Callback(void);
void EP6_OUT_Callback(void);
void EP7_OUT_Callback(void);

#ifdef CTR_CALLBACK
void CTR_Callback(void);
#endif

#ifdef DOVR_CALLBACK
void DOVR_Callback(void);
#endif

#ifdef ERR_CALLBACK
void ERR_Callback(void);
#endif

#ifdef WKUP_CALLBACK
void WKUP_Callback(void);
#endif

#ifdef SUSP_CALLBACK
void SUSP_Callback(void);
#endif

#ifdef RESET_CALLBACK
void RESET_Callback(void);
#endif

#ifdef SOF_CALLBACK
void SOF_Callback(void);
#endif

#ifdef ESOF_CALLBACK
void ESOF_Callback(void);
#endif

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

#endif /* __SDK_EVAL_DFU_ISTR_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
