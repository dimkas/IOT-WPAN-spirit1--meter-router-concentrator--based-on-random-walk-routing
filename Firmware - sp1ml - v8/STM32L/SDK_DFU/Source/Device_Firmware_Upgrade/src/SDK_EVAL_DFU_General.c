/**
 * @file     SDK_EVAL_DFU_General.c
 * @author   MSH RF/ART Team IMS-Systems Lab
 * @version  V1.0.0
 * @date     03/07/2011
 * @brief    This file provides a set of APIs to manage SDK USB for DFU functionality
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
#include "stm32l1xx.h"
#include <stdlib.h>
#include "SDK_EVAL_DFU_Mal.h"
#include "usb_lib.h"
#include "SDK_EVAL_DFU_Desc.h"
#include "SDK_EVAL_DFU_Pwr.h"
#include "SDK_EVAL_DFU_General.h"
#include "SDK_EVAL_Button.h"


/** @addtogroup SDK_EVAL_DFU
 * @{
 */


/** @addtogroup SDK_EVAL_DFU_General
 * @{
 */


/** @defgroup SDK_EVAL_DFU_General_Private_TypesDefinitions      SDK EVAL DFU General Private TypesDefinitions
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_General_Private_Defines      SDK EVAL DFU General Private Defines
 * @{
 */


/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_General_Private_Macros      SDK EVAL DFU General Private Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_General_Private_Variables     SDK EVAL DFU General Private Variables
 * @{
 */

__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_General_Private_FunctionPrototypes    SDK EVAL DFU General Private FunctionPrototypes
 * @{
 */

static void SdkEvalDFUIntToUnicode (uint32_t lValue , uint8_t* pcBuffer , uint8_t cLength);


/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_General_Private_Functions      SDK EVAL DFU General Private Functions
 * @{
 */



/**
 * @brief  Configures main system clocks, USB clock, USB interrupt, GPIO pin for USB pull-up if defined
 *         and the Medium Interface as well as the USB peripheral.
 * @param  None
 * @retval None
 */
void SdkEvalDFUInit(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
#if defined(USB_USE_EXTERNAL_PULLUP)
  GPIO_InitTypeDef  GPIO_InitStructure;
#endif /* USB_USE_EXTERNAL_PULLUP */

  /*!< At this stage the microcontroller clock setting is already configured,
  this is done through SystemInit() function which is called from startup
  file (startup_stm32l1x_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32l1xx.c file */

  /* Enable the SYSCFG module clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Unlock the internal flash */
  FLASH_Unlock();

#if defined(USB_USE_EXTERNAL_PULLUP)
  /* Enable the USB disconnect GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_DISCONNECT, ENABLE);

  /* USB_DISCONNECT used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
#endif /* USB_USE_EXTERNAL_PULLUP */

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

  /* Configure USB interrupt */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Init the media interface */
  MAL_Init();

  /* Enable the USB connection */
  SdkEvalDFUCableConfig(ENABLE);

  /* Initialize USB peripheral */
  USB_Init();

}


/**
 * @brief  Software Connection/Disconnection of USB Cable.
 * @param  xNewState: new state for cable connection.
 *         This parameter can be ENABLE or DISABLE
 * @retval None
 */
void SdkEvalDFUCableConfig (FunctionalState xNewState)
{
  if (xNewState != DISABLE)
  {
    STM32L15_USB_CONNECT;
  }
  else
  {
    STM32L15_USB_DISCONNECT;
  }

}


/**
 * @brief  Power-off system clocks and power while entering suspend mode.
 * @param  None
 * @retval None
 */
void SdkEvalDFUEnterLowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;

}


/**
 * @brief  Restores system clocks and power while exiting suspend mode.
 * @param  None
 * @retval None
 */
void SdkEvalDFULeaveLowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }

}


/**
  * @brief  Reset the device
  * @param  None
  * @retval None
  */
void SdkEvalDFUResetDevice(void)
{
  SdkEvalDFUCableConfig(DISABLE);
  NVIC_SystemReset();
}


/**
 * @brief  Create the serial number string descriptor.
 * @param  None
 * @retval None
 */
void SdkEvalDFUGetSerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(uint32_t*)(0x1FF80050);
  Device_Serial1 = *(uint32_t*)(0x1FF80054);
  Device_Serial2 = *(uint32_t*)(0x1FF80064);

  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    SdkEvalDFUIntToUnicode (Device_Serial0, &g_vectcDFUStringSerial[2] , 8);
    SdkEvalDFUIntToUnicode (Device_Serial1, &g_vectcDFUStringSerial[18], 4);
  }

}


/**
 * @brief  Convert Hex 32Bits value into char.
 * @param  lValue: the HEX words to convert
 * @param  pcBuffer: the buffer in whic put the converted values
 * @param  cLength: the number of byte to convert
 * @retval None
 */
static void SdkEvalDFUIntToUnicode (uint32_t lValue , uint8_t* pcBuffer , uint8_t cLength)
{
  uint8_t idx = 0;

  for( idx = 0 ; idx < cLength ; idx ++)
  {
    if( ((lValue >> 28)) < 0xA )
    {
      pcBuffer[ 2* idx] = (lValue >> 28) + '0';
    }
    else
    {
      pcBuffer[2* idx] = (lValue >> 28) + 'A' - 10;
    }

    lValue = lValue << 4;

    pcBuffer[ 2* idx + 1] = 0;
  }

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
