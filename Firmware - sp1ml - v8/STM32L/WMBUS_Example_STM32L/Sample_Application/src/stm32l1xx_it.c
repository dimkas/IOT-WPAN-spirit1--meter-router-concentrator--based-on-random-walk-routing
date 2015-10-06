/**
******************************************************************************
* @file    RCC/RCC_ClockSwitch/stm32l1xx_it.c 
* @author  MCD Application Team
* @version V1.0.0RC1
* @date    10/25/2010
* @brief   Main Interrupt Service Routines.
*          This file provides template for all exceptions handler and peripherals
*          interrupt service routine.
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
*/ 

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_it.h"
#include "wmbus_appli.h"  

#include "SDK_EVAL_VC_General.h"

/** @addtogroup STM32L1xx_StdPeriph_Examples
* @{
*/

/** @addtogroup RCC_ClockSwitch
* @{
*/ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
* @brief  This function handles NMI exception.
* @param  None
* @retval None
*/
void NMI_Handler(void)
{
}

/**
* @brief  This function handles Hard Fault exception.
* @param  None
* @retval None
*/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
* @brief  This function handles Memory Manage exception.
* @param  None
* @retval None
*/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
* @brief  This function handles Bus Fault exception.
* @param  None
* @retval None
*/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
* @brief  This function handles Usage Fault exception.
* @param  None
* @retval None
*/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
* @brief  This function handles SVCall exception.
* @param  None
* @retval None
*/
void SVC_Handler(void)
{
}

/**
* @brief  This function handles Debug Monitor exception.
* @param  None
* @retval None
*/
void DebugMon_Handler(void)
{
}

/**
* @brief  This function handles PendSV_Handler exception.
* @param  None
* @retval None
*/
void PendSV_Handler(void)
{
}

/******************************************************************************/
/*            STM32L1xx Peripherals Interrupt Handlers                        */
/******************************************************************************/


/**
* @brief  This function handles External line 0 interrupt request.
* @param  None
* @retval None
*/
void EXTI0_IRQHandler(void)
{
  if(0x00 == GPIO_ReadInputDataBit(DOWN_BUTTON_GPIO_PORT, DOWN_BUTTON_PIN))
  {
    Set_DOWNStatus();
    EXTI_ClearITPendingBit(DOWN_BUTTON_EXTI_LINE);
  }
  
}
/**
* @brief  This function handles External line 1 interrupt request.
* @param  None
* @retval None
*/
void EXTI1_IRQHandler(void)
{
  if(0x00 == GPIO_ReadInputDataBit(RIGHT_BUTTON_GPIO_PORT, RIGHT_BUTTON_PIN))
  {
    Set_RightStatus();
    /* Clear the Up Button EXTI line pending bit */
    EXTI_ClearITPendingBit(RIGHT_BUTTON_EXTI_LINE); 
  }
  else// if(0x00 == GPIO_ReadInputDataBit(KEY_BUTTON_VD1_GPIO_PORT, KEY_BUTTON_VD1_PIN))
  {
    Set_RightStatus();
    /* Clear the Right Button EXTI line pending bit */
    EXTI_ClearITPendingBit(KEY_BUTTON_VD1_EXTI_LINE); 
  }
}

/**
* @brief  This function handles External line 2 interrupt request.
* @param  None
* @retval None
*/
void EXTI2_IRQHandler(void)
{
#if (DEVICE_TYPE == METER)
  if(0x00 == GPIO_ReadInputDataBit(SCM_PS_BUTTON_VD1_GPIO_PORT, SCM_PS_BUTTON_VD1_PIN))
  {
    Set_DOWNStatus();
    /* Clear the Up Button EXTI line pending bit */
    EXTI_ClearITPendingBit(SCM_PS_BUTTON_VD1_EXTI_LINE); 
  }
#else
  /* Check the flag status of EXTI line */
  if(EXTI_GetITStatus(SCM_PS_BUTTON_VD1_EXTI_LINE))
  {    
    Set_KeyStatus();
    /* Clear the EXTI line flag */
    EXTI_ClearITPendingBit(SCM_PS_BUTTON_VD1_EXTI_LINE);
  } 
#endif
}



/**
* @brief  This function handles External lines 9 to 5 interrupt request.
* @param  None
* @retval None
*/
void EXTI9_5_IRQHandler(void)
{  
  /* Check the flag status of EXTI line */
  if(EXTI_GetFlagStatus(UP_BUTTON_V2_EXTI_LINE))
  {    
    Set_UPStatus();
    /* Clear the EXTI line flag */
    EXTI_ClearFlag(UP_BUTTON_V2_EXTI_LINE);
  }
  if(EXTI_GetFlagStatus(LEFT_BUTTON_V3_EXTI_LINE))
  {    
    Set_UPStatus();
    /* Clear the EXTI line flag */
    EXTI_ClearFlag(LEFT_BUTTON_V3_EXTI_LINE);
  }
  /* Check the flag status of EXTI line */
  if(EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE))
  {    
    Set_KeyStatus();
    /* Clear the EXTI line flag */
    EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
  }
  if(EXTI_GetITStatus(SCM_PS_BUTTON_EXTI_LINE))
  {
    Set_KeyStatus();
    /* Clear the EXTI line flag */
    EXTI_ClearITPendingBit(SCM_PS_BUTTON_EXTI_LINE);
  }
  
}
/**
* @brief  This function handles External lines 15 to 10 interrupt request.
* @param  None
* @retval None
*/
void EXTI15_10_IRQHandler(void)
{  
  /* Check the flag status of EXTI line 10 */
  if(EXTI_GetITStatus(M2S_GPIO_3_EXTI_LINE))
  {    
    WMBus_PhyInterruptHandler();
    /* Clear the EXTI line 10 flag */
    EXTI_ClearITPendingBit(M2S_GPIO_3_EXTI_LINE); 
  }
  /* Check the flag status of EXTI line */
  if(EXTI_GetFlagStatus(UP_BUTTON_V3_EXTI_LINE))
  {    
    Set_UPStatus();
    /* Clear the EXTI line flag */
    EXTI_ClearFlag(UP_BUTTON_V3_EXTI_LINE);
  }
}
/**
* @brief  This function handles External USB interrupt requests.
* @param  None
* @retval None
*/
void USB_LP_IRQHandler(void)
{
  uint8_t c;
  
  SdkEvalVCIntServRoutine();
  
  if(__io_getcharNonBlocking(&c))
  {
    if(c=='i' || c=='I')
      Set_DOWNStatus();
    else if(c=='l' || c=='L')
      Set_UPStatus();
    else// if(c=='d')
      Set_RightStatus();
  }
}
/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_md.s).                                            */
/******************************************************************************/

/**
* @brief  This function handles PPP interrupt request.
* @param  None
* @retval None
*/
/*void PPP_IRQHandler(void)
{
}*/

/**
* @}
*/ 

/**
* @}
*/ 

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
