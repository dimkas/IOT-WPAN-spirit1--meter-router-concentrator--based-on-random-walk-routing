/**
 * @file     SDK_EVAL_DFU_Istr.c
 * @author   MSH RF/ART Team IMS-Systems Lab
 * @version  V1.0.0
 * @date     03/07/2011
 * @brief    ISTR events interrupt service routines.
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
#include "usb_lib.h"
#include "SDK_EVAL_DFU_Prop.h"
#include "SDK_EVAL_DFU_Pwr.h"
#include "SDK_EVAL_DFU_Istr.h"


/** @addtogroup SDK_EVAL_DFU
 * @{
 */


/** @addtogroup SDK_EVAL_DFU_Istr
 * @{
 */


/** @defgroup SDK_EVAL_DFU_Istr_Private_TypesDefinitions      SDK EVAL DFU Istr Private TypesDefinitions
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Istr_Private_Defines      SDK EVAL DFU Istr Private Defines
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Istr_Private_Macros      SDK EVAL DFU Istr Private Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Istr_Private_Variables     SDK EVAL DFU Istr Private Variables
 * @{
 */

__IO uint16_t wIstr;  /*!< ISTR register last read value */
__IO uint8_t bIntPackSOF = 0;  /*!< SOFs received between 2 consecutive packets */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Istr_Private_FunctionPrototypes    SDK EVAL DFU Istr Private FunctionPrototypes
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Istr_Private_Functions      SDK EVAL DFU Istr Private Functions
 * @{
 */

void (*pEpInt_IN[7])(void) =
  {
    EP1_IN_Callback,
    EP2_IN_Callback,
    EP3_IN_Callback,
    EP4_IN_Callback,
    EP5_IN_Callback,
    EP6_IN_Callback,
    EP7_IN_Callback,
  };

void (*pEpInt_OUT[7])(void) =
  {
    EP1_OUT_Callback,
    EP2_OUT_Callback,
    EP3_OUT_Callback,
    EP4_OUT_Callback,
    EP5_OUT_Callback,
    EP6_OUT_Callback,
    EP7_OUT_Callback,
  };


/**
  * @brief  STR events interrupt service routine.
  * @param  None. 
  * @retval None.
  */

void USB_Istr(void)
{

  wIstr = _GetISTR();

#if (IMR_MSK & ISTR_CTR)
  if (wIstr & ISTR_CTR & wInterrupt_Mask)
  {
    /* servicing of the endpoint correct transfer interrupt */
    /* clear of the CTR flag into the sub */
    CTR_LP();
#ifdef CTR_CALLBACK
    CTR_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/  
#if (IMR_MSK & ISTR_RESET)
  if (wIstr & ISTR_RESET & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_RESET);
    Device_Property.Reset();
#ifdef RESET_CALLBACK
    RESET_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_DOVR)
  if (wIstr & ISTR_DOVR & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_DOVR);
#ifdef DOVR_CALLBACK
    DOVR_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_ERR)
  if (wIstr & ISTR_ERR & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_ERR);
#ifdef ERR_CALLBACK
    ERR_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_WKUP)
  if (wIstr & ISTR_WKUP & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_WKUP);
    Resume(RESUME_EXTERNAL);
#ifdef WKUP_CALLBACK
    WKUP_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_SUSP)
  if (wIstr & ISTR_SUSP & wInterrupt_Mask)
  {

    /* check if SUSPEND is possible */
    if (fSuspendEnabled)
    {
      Suspend();
    }
    else
    {
      /* if not possible then resume after xx ms */
      Resume(RESUME_LATER);
    }
    /* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
    _SetISTR((uint16_t)CLR_SUSP);
#ifdef SUSP_CALLBACK
    SUSP_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_SOF)
  if (wIstr & ISTR_SOF & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_SOF);
    bIntPackSOF++;

#ifdef SOF_CALLBACK
    SOF_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_ESOF)
  if (wIstr & ISTR_ESOF & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_ESOF);
    /* resume handling timing is made with ESOFs */
    Resume(RESUME_ESOF); /*!< request without change of the machine state */

#ifdef ESOF_CALLBACK
    ESOF_Callback();
#endif
  }
#endif
} /*!< USB_Istr */

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
