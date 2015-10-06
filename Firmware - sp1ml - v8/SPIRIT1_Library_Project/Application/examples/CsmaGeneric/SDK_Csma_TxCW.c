/**
* @file    SDK_Csma_TxCW.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Example of configuration SPIRIT CSMA.
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
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
*/


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Config.h"
#include "SPIRIT_Config.h"
#include "SDK_Configuration_Common.h"

//#define USE_VCOM

#ifdef STM8L
#include "STM8L_Common.h"
#define XTAL_FREQUENCY          50000000
#define RANGE_TYPE              RANGE_EXT_NONE       //RANGE_EXT_SKYWORKS_169 or RANGE_EXT_SKYWORKS_868
#define FREQUENCY_OFFSET        0
#elif SDK
#include "SPIRIT_SDK_Util.h"
#include "SDK_Common.h"
#ifdef USE_VCOM
#include "SDK_EVAL_VC_General.h"
#endif
#endif

/**
* @addtogroup SDK_Examples
* @{
*/

/**
* @addtogroup SDK_Csma                         SDK CSMA
* @{
*/

/**
* @addtogroup SDK_Csma_TxCW                    SDK CSMA TxCW
* @brief Noise generator device.
* @details This code configures the device as a continous wave emitter in order to occupy the channel
* to permit the validation of the CSMA feature enabled on the device A or B of this example.
* @{
*/

/**
* @defgroup Csma_TxCW_Private_TypesDefinitions         CSMA TxCW Private TypesDefinitions
* @{
*/

/**
*@}
*/


/**
* @defgroup Csma_TxCW_Private_Defines                  CSMA TxCW Private Defines
* @{
*/

/**
*@}
*/


/**
* @defgroup Csma_TxCW_Private_Macros                           CSMA TxCW Private Macros
* @{
*/

/**
*@}
*/

/**
* @defgroup Csma_TxCW_Private_Variables                        CSMA TxCW Private Variables
* @{
*/

/**
  * @brief Radio structure fitting
  */
SRadioInit xRadioInit = {
  XTAL_OFFSET_PPM,
  BASE_FREQUENCY,
  CHANNEL_SPACE,
  CHANNEL_NUMBER,
  MODULATION_SELECT,
  DATARATE,
  FREQ_DEVIATION,
  BANDWIDTH
};
  


/**
*@}
*/

/**
* @defgroup Csma_TxCW_Private_FunctionPrototypes                       CSMA TxCW Private FunctionPrototypes
* @{
*/

/**
*@}
*/


/**
* @defgroup Csma_TxCW_Private_Functions                                CSMA TxCW Private Functions
* @{
*/


/**
* @brief  System main function.
* @param  None
* @retval None
*/
void main (void)
{
  SDK_SYSTEM_CONFIG();
  
  /* Spirit ON */
  SpiritEnterShutdown();
  SpiritExitShutdown();
  SpiritManagementWaExtraCurrent();
  
#ifdef STM8L
  /* Manually set the XTAL_FREQUENCY */
  SpiritRadioSetXtalFrequency(XTAL_FREQUENCY);    
  /* Initialize the frequency offset variable to compensate XTAL offset */
  xRadioInit.lFrequencyBase = xRadioInit.lFrequencyBase + FREQUENCY_OFFSET;
  /* Initialize the signals to drive the range extender application board */
  RANGE_EXT_INIT(RANGE_TYPE);
#elif SDK
  SpiritManagementIdentificationRFBoard();

  /* if the board has eeprom, we can compensate the offset calling SpiritManagementGetOffset
  (if eeprom is not present this fcn will return 0) */
  xRadioInit.lFrequencyBase = xRadioInit.lFrequencyBase + SpiritManagementGetOffset();
  
  /* Initialize the signals to drive the range extender application board */
  SpiritManagementRangeExtInit();  
#endif
  
  /* Spirit Radio config */
  SpiritRadioInit(&xRadioInit);
  
  /* Spirit Radio set power */
  SpiritRadioSetPALeveldBm(0,POWER_DBM);
  SpiritRadioSetPALevelMaxIndex(0);
  
  /* Spirit direct Tx mode - we have to configure a direct mode
    that doesn't use the FIFO in order to not have an autoabort
    during the transmission */
  SpiritDirectRfSetTxMode(PN9_TX_MODE);
  
  /* continous wave config (no modulation) */
  SpiritRadioCWTransmitMode(S_ENABLE);
  
  /* TX command */
  SpiritCmdStrobeTx();
  
  /* infinite loop */
  while (1);
  
}



#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/**
*@}
*/

/**
*@}
*/

/**
*@}
*/

/**
*@}
*/


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
