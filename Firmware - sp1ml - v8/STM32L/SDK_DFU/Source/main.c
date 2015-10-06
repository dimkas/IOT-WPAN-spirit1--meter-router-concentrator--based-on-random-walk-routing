/**
 * @file    main.c
 * @author  MSH RF/ART Team IMS-Systems Lab
 * @version V1.0.0
 * @date    03/07/2011
 * @brief   SDK Device Firmware Upgrade (DFU) main program body.
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


/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "SDK_EVAL_DFU_Prop.h"
#include "SDK_EVAL_DFU_Pwr.h"
#include "SDK_EVAL_DFU_Mal.h"
#include "SDK_EVAL_DFU_General.h"
#include "SDK_EVAL_Button.h"
#include "SDK_EVAL_Led.h"
#include "SDK_EVAL_Config.h"

/** @defgroup SDK_EVAL_DFU_Program    SDK EVAL DFU Program
 * @{
 */


/** @defgroup SDK_EVAL_DFU_Program_Private_TypesDefinitions      SDK EVAL DFU Program Private TypesDefinitions
 * @{
 */

typedef  void (*pFunction)(void);

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Program_Private_Defines      SDK EVAL DFU Program Private Defines
 * @{
 */

#define DATA_EEPROM_START_ADDR     0x08080000

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Program_Private_Macros      SDK EVAL DFU Program Private Macros
 * @{
 */
#define SdkDfuReadDfuFlag()           *((uint8_t*)(DATA_EEPROM_START_ADDR));

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Program_Private_Variables     SDK EVAL DFU Program Private Variables
 * @{
 */

uint8_t DeviceState;
uint8_t DeviceStatus[6];
pFunction Jump_To_Application;
uint32_t JumpAddress;

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Program_Private_FunctionPrototypes    SDK EVAL DFU Program Private FunctionPrototypes
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_DFU_Program_Private_Functions      SDK EVAL DFU Program Private Functions
 * @{
 */

/**
  * @brief  Function used to reset the DFU MODE flag on E2PROM.
  * @param  None
  * @retval None
  */
void SdkDfuResetDfuFlag(void){

  /* Unlock the FLASH PECR register and Data EEPROM memory */
  DATA_EEPROM_Unlock();

  /* Clear all pending flags */
  FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                  | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);

  /* Write 0 on the E2PROM data address 0x08080000 */
  FLASH_Status FLASHStatus = DATA_EEPROM_ProgramByte(DATA_EEPROM_START_ADDR, 0);

  /* Clear all the Flash flags */
  if(FLASHStatus != FLASH_COMPLETE)
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                    | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);


  /* Lock the FLASH PECR register and Data EEPROM memory */
  DATA_EEPROM_Lock();

}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured,
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f10x_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f10x.c file
  */
  
  /* Identify the board to distinguish the SDK motherboard from the SDK dongle */
  SdkEvalIdentification();
  
  /* Declare and read the DFU flag on E2Prom */
  uint8_t cDfuFlag = SdkDfuReadDfuFlag();

  /* Initialize the Key push-button */
  SdkEvalPushButtonInit(BUTTON_SCM_PS, BUTTON_MODE_GPIO);


  /* Check if the Key push-button on is pressed */
   if (!(SdkEvalPushButtonGetState(BUTTON_SCM_PS) == RESET || cDfuFlag))
  { /* Test if user code is programmed starting from address 0x8003000 */

    if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
    { /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*((__IO uint32_t*) ApplicationAddress));
      
      Jump_To_Application();
    }
  } /*!< Otherwise enters DFU mode to allow user to program his application */

  /* Reset the DFU flag on E2Prom */
  SdkDfuResetDfuFlag();

  /* Enter DFU mode */
  DeviceState = STATE_dfuERROR;
  DeviceStatus[0] = STATUS_ERRFIRMWARE;
  DeviceStatus[4] = DeviceState;

  /* DFU initialization */
  SdkEvalDFUInit();

  /* LED2 initialization */
  SdkEvalLedInit(LED2);

  /* Main loop */
  while (1)
  {
    /* LED2 toggling */
    SdkEvalLedToggle(LED2);
    
    /* pause */
    for(volatile uint32_t i = 0 ; i<0x3FFFFF ; i++);
  }
}



#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
 * @}
 */


/**
 * @}
 */





/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
