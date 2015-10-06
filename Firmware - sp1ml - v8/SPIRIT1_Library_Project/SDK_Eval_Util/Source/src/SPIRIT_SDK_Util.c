/**
* @file    SPIRIT_SDK_Util.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    February 1, 2015
* @brief   Identification functions for SPIRIT DK.
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
#include "SPIRIT_SDK_Util.h"
#include "stm32l1xx.h"

/**
* @addtogroup SPIRIT_DK                   SPIRIT DK
* @{
*/


/**
* @defgroup SDK_SPIRIT_MANAGEMENT              SDK SPIRIT Management
* @{
*/


/**
* @brief This flag is used to synchronize the TIM3 ISR with the XtalMeasurement routine.
*/
static volatile FlagStatus s_xTIMChCompareModeRaised = RESET;

/**
* @brief This flag is used to synchronize the TIM3 ISR with the XtalMeasurement routine.
*/
static RangeExtType xRangeExtType = RANGE_EXT_NONE;

static uint8_t s_RfModuleBand = 0;
static int32_t s_RfModuleOffset = 0;

#define ENABLE_TCXO()           GPIO_SetBits(GPIOC,GPIO_Pin_2);


/**
* @defgroup SDK_SPIRIT_MANAGEMENT_FUNCTIONS    SDK SPIRIT Management Functions
* @{
*/

/**
* @defgroup IDENTIFICATION_FUNCTIONS      SDK SPIRIT Management Identification Functions
* @{
*/


/**
* @brief This function handles TIM3 global interrupt
* @param None.
* @retval None.
*/
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_CC4))
  {
    /* Set the TIM3 Compare IRQ flag */
    s_xTIMChCompareModeRaised = SET;
    
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
    
  }
  else if(TIM_GetITStatus(TIM3, TIM_IT_CC2))
  {
    /* Set the TIM3 Compare IRQ flag */
    s_xTIMChCompareModeRaised = SET;
    
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    
  }
}

/**
* @brief This function handles TIM4 global interrupt
* @param None.
* @retval None.
*/
void TIM4_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM4, TIM_IT_CC4))
  {
    /* Set the TIM4 Compare IRQ flag */
    s_xTIMChCompareModeRaised = SET;
    
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
    
  }
  else if(TIM_GetITStatus(TIM4, TIM_IT_CC2))
  {
    /* Set the TIM4 Compare IRQ flag */
    s_xTIMChCompareModeRaised = SET;
    
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
    
  }
}

/**
* @brief  This function can be used to automatically measure the XTAL frequency making use of the
*         Spirit clock output to pin and an STM32L timer in compare mode.
* @param  None.
* @retval None.
*/
#define N_SAMPLES 20
#define SETTLING_PERIODS 4
#define A 0.4
uint32_t SpiritManagementComputeXtalFrequency(void)
{   
  GPIO_TypeDef *pGpioPeriph;
  TIM_TypeDef *pTimerPeriph;
  
  if(SdkEvalGetVersion()==SDK_EVAL_VERSION_D1) {
    pTimerPeriph=TIM4;
    pGpioPeriph=GPIOB;
    /* TIM4 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* GPIOB clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  }
  else {
    pTimerPeriph=TIM3;
    pGpioPeriph=GPIOC;
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    /* GPIOC clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  }    
  
  //#warning It is more safe disable all the other interrupt source.
  /* MCU GPIO, NVIC and timer configuration structures */
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  
  uint32_t lMeasuredXtalFrequency;

  if(SpiritManagementGetRangeExtender()==RANGE_EXT_SKYWORKS_169)
  {
    ENABLE_TCXO();
  }
  
  /* Instance the variables used to compute the XTAL frequency */
  uint8_t CaptureNumber=0;
  uint16_t IC3ReadValue1=0,IC3ReadValue2=0,Capture=0;
  volatile uint16_t cWtchdg = 0;
  uint32_t TIMFreq=0,lXoFreq=0;
  float fXoFreqRounded;
  
  /* TIM3 channel 2 pin (PC.07) configuration */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
  GPIO_Init(pGpioPeriph, &GPIO_InitStructure);
  GPIO_PinAFConfig(pGpioPeriph, GPIO_PinSource7, (pTimerPeriph==TIM3)?(GPIO_AF_TIM3):(GPIO_AF_TIM4));
  
  /* Configure the timer compare channel 2 */
  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
  /* Configure the timer IRQ to be raised on the rising fronts */
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
  /* Input capture selection setting */
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  /* Input capture prescaler setting. Setting it to TIM_ICPSC_DIV8 makes the IRQ are raised every 8 rising fronts detected by hardware.  */
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  /* Disable every kind of capture filter */
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  
  /* Timer initialization */
  TIM_ICInit(pTimerPeriph, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(pTimerPeriph, ENABLE);
  
  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(pTimerPeriph, TIM_IT_CC2, ENABLE);
  
  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ((pTimerPeriph==TIM3)?(TIM3_IRQn):(TIM4_IRQn));
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Disable the clock divider to measure the max frequency of the clock. */
  uint8_t tmp= 0x29; SpiritSpiWriteRegisters(0xB4, 1, &tmp);
  
  /* Spirit1 side clock configuration */
  SpiritGpioClockOutputInit(&(ClockOutputInit){XO_RATIO_1_192, RCO_RATIO_1, EXTRA_CLOCK_CYCLES_0});
  
  /* Instance the structure used to configure the Spirit clock frequency to be divided by a 192 factor. */
  SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_0, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP, SPIRIT_GPIO_DIG_OUT_MCU_CLOCK});
  
  SpiritGpioClockOutput(S_ENABLE);
  
  /* measure the frequency and average it on N_SAMPLES. Moreover cycle to wait for same SETTLING_PERIODS */
  for(uint32_t i=0;i<2*(N_SAMPLES+SETTLING_PERIODS);i++) {
    /* block the routine until the TIM CCP2 IRQ is raised */
    while(!s_xTIMChCompareModeRaised && (cWtchdg!=0xFFFF)) {
      cWtchdg++;    
    }
    
    if(cWtchdg==0xFFFF) {
      break;
    }
    else {
      cWtchdg=0;
    }
    
    /* reset the IRQ raised flag */
    s_xTIMChCompareModeRaised = RESET;
    
    /* if the SETTLING PERIODS expired */
    if(i>=SETTLING_PERIODS*2) {
      /* First TIMER capture */
      if(CaptureNumber == 0)
      {
        /* Get the Input Capture value */
        IC3ReadValue1 = TIM_GetCapture2(pTimerPeriph);
        CaptureNumber = 1;
      }
      /* Second TIMER capture */
      else if(CaptureNumber == 1)
      {
        /* Get the Input Capture value */
        IC3ReadValue2 = TIM_GetCapture2(pTimerPeriph);
        
        /* Capture computation */
        if (IC3ReadValue2 > IC3ReadValue1)
        {
          /* If the TIMER didn't overflow between the first and the second capture. Compute it as the difference between the second and the first capture values. */
          Capture = (IC3ReadValue2 - IC3ReadValue1) - 1;
        }
        else
        {
          /* .. else, if overflowed 'roll' the first measure to be complementar of 0xFFFF */
          Capture = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2) - 1;
        }
        
        /* Punctual frequency computation */
        TIMFreq = (uint32_t) SystemCoreClock / Capture;
        
        /* Averaged frequency computation */
        lXoFreq =(uint32_t)(A*(float)lXoFreq+(1.0-A)*(float)TIMFreq);
        
        CaptureNumber = 0;
      }
    }
  }
  
  /* Compute the real frequency in Hertz tanking in account the MCU and Spirit divisions */
  lXoFreq *=(192*8);
  
  /* Disable the output clock */
  SpiritGpioClockOutput(S_DISABLE);
  
  /* TIM enable counter */
  TIM_Cmd(pTimerPeriph, DISABLE);
  
  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(pTimerPeriph, TIM_IT_CC2, DISABLE);
  
  /* SPIRIT GPIO 0 to the default configuration */
  SpiritGpioSetLevel(SPIRIT_GPIO_0, LOW);

  tmp= 0x21; SpiritSpiWriteRegisters(0xB4, 1, &tmp);
  
  /* Round the measured frequency to be measured as an integer MHz value */
  fXoFreqRounded = (float)lXoFreq/1e6;
  
  if( fXoFreqRounded-(float)((uint32_t)fXoFreqRounded)>0.5)
  {
    lMeasuredXtalFrequency = (((uint32_t)fXoFreqRounded+1)*1000000);
  }
  else
  {
    lMeasuredXtalFrequency = (((uint32_t)fXoFreqRounded)*1000000);
  }
  
  SdkEvalM2SGpioInit(M2S_GPIO_0, M2S_MODE_GPIO_IN);
  
  lMeasuredXtalFrequency=50000000;
  SpiritRadioSetXtalFrequency(lMeasuredXtalFrequency);
  
  return lMeasuredXtalFrequency;
}

/* This function is used to detect the pa ext board, due to the unworking measurement algorithm */
uint32_t SpiritManagementComputeXtalFrequencyGpio2(void)
{
  
  GPIO_TypeDef *pGpioPeriph;
  TIM_TypeDef *pTimerPeriph;
  
  if(SdkEvalGetVersion()==SDK_EVAL_VERSION_D1)
  {
    pTimerPeriph=TIM4;
    pGpioPeriph=GPIOB;
    /* TIM4 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* GPIOB clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  }
  else
  {
    pTimerPeriph=TIM3;
    pGpioPeriph=GPIOC;
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    /* GPIOC clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  }  
  
  /* MCU GPIO, NVIC and timer configuration structures */
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  
  uint32_t lMeasuredXtalFrequency;
  
  /* Instance the variables used to compute the XTAL frequency */
  uint8_t CaptureNumber=0;
  uint16_t IC3ReadValue1=0,IC3ReadValue2=0,Capture=0;
  volatile uint16_t cWtchdg = 0;
  uint32_t TIM3Freq=0,lXoFreq=0;
  float fXoFreqRounded;
  
  
  /* TIM3 channel 2 pin (PC.07) configuration */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_Init(pGpioPeriph, &GPIO_InitStructure);
  GPIO_PinAFConfig(pGpioPeriph, GPIO_PinSource9, (pTimerPeriph==TIM3)?(GPIO_AF_TIM3):(GPIO_AF_TIM4));
  
  /* Configure the timer compare channel 2 */
  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_4;
  /* Configure the timer IRQ to be raised on the rising fronts */
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
  /* Input capture selection setting */
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  /* Input capture prescaler setting. Setting it to TIM_ICPSC_DIV8 makes the IRQ are raised every 8 rising fronts detected by hardware.  */
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  /* Disable every kind of capture filter */
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  
  /* Timer initialization */
  TIM_ICInit(pTimerPeriph, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(pTimerPeriph, ENABLE);
  
  /* Enable the CC4 Interrupt Request */
  TIM_ITConfig(pTimerPeriph, TIM_IT_CC4, ENABLE);
  
  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ((pTimerPeriph==TIM3)?(TIM3_IRQn):(TIM4_IRQn));
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Spirit1 side clock configuration */
  SpiritGpioClockOutputInit(&(ClockOutputInit){XO_RATIO_1_192, RCO_RATIO_1, EXTRA_CLOCK_CYCLES_0});
  
  /* Instance the structure used to configure the Spirit clock frequency to be divided by a 192 factor. */
  SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_2, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP, SPIRIT_GPIO_DIG_OUT_MCU_CLOCK});
  SpiritGpioClockOutput(S_ENABLE);
  
  /* measure the frequency and average it on N_SAMPLES. Moreover cycle to wait for same SETTLING_PERIODS */
  for(uint32_t i=0;i<2*(N_SAMPLES+SETTLING_PERIODS);i++)
  {
    /* block the routine until the TIM3 CCP2 IRQ is raised */
    while(!s_xTIMChCompareModeRaised && (cWtchdg!=0xFFFF))
      cWtchdg++;    
    
    if(cWtchdg==0xFFFF)
      break;
    else
      cWtchdg=0;
    
    /* reset the IRQ raised flag */
    s_xTIMChCompareModeRaised = RESET;
    
    /* if the SETTLING PERIODS expired */
    if(i>=SETTLING_PERIODS*2)
    {
      /* First TIMER3 capture */
      if(CaptureNumber == 0)
      {
        /* Get the Input Capture value */
        IC3ReadValue1 = TIM_GetCapture4(pTimerPeriph);
        CaptureNumber = 1;
      }
      /* Second TIMER3 capture */
      else if(CaptureNumber == 1)
      {
        /* Get the Input Capture value */
        IC3ReadValue2 = TIM_GetCapture4(pTimerPeriph);
        
        /* Capture computation */
        if (IC3ReadValue2 > IC3ReadValue1)
        {
          /* If the TIMER3 didn't overflow between the first and the second capture. Compute it as the difference between the second and the first capture values. */
          Capture = (IC3ReadValue2 - IC3ReadValue1) - 1;
        }
        else
        {
          /* .. else, if overflowed 'roll' the first measure to be complementar of 0xFFFF */
          Capture = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2) - 1;
        }
        
        /* Punctual frequency computation */
        TIM3Freq = (uint32_t) SystemCoreClock / Capture;
        
        /* Averaged frequency computation */
        lXoFreq =(uint32_t)(A*(float)lXoFreq+(1.0-A)*(float)TIM3Freq);
        
        CaptureNumber = 0;
      }
    }
  }
  
  /* Compute the real frequency in Hertz tanking in account the MCU and Spirit divisions */
  lXoFreq *=(192*8);
  
  /* Disable the output clock */
  SpiritGpioClockOutput(S_DISABLE);
  
  /* TIM enable counter */
  TIM_Cmd(pTimerPeriph, DISABLE);
  
  /* Enable the CC4 Interrupt Request */
  TIM_ITConfig(pTimerPeriph, TIM_IT_CC4, DISABLE);
  
  /* SPIRIT GPIO 2 to the default configuration */
  SpiritGpioSetLevel(SPIRIT_GPIO_2, LOW);
  
  /* Round the measured frequency to be measured as an integer MHz value */
  fXoFreqRounded = (float)lXoFreq/1e6;
  
  if( fXoFreqRounded-(float)((uint32_t)fXoFreqRounded)>0.5)
  {  
    lMeasuredXtalFrequency = (((uint32_t)fXoFreqRounded+1)*1000000);
  }
  else
  {
    lMeasuredXtalFrequency = (((uint32_t)fXoFreqRounded)*1000000);
  }
  
  SdkEvalM2SGpioInit(M2S_GPIO_2, M2S_MODE_GPIO_IN);
  
  return lMeasuredXtalFrequency;
}



/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
uint8_t EepromIdentification(void)
{
  uint8_t status;
  status = EepromSetSrwd();
  status = EepromStatus();
  if((status&0xF0) == EEPROM_STATUS_SRWD) { // 0xF0 mask [SRWD 0 0 0]
    status = 1;
    EepromResetSrwd();
  }
  else
    status = 0;
  
  return status;
}


void SpiritManagementIdentificationRFBoard(void)
{
    do{
      /* Delay for state transition */
      for(volatile uint8_t i=0; i!=0xFF; i++);
      
      /* Reads the MC_STATUS register */
     SpiritRefreshStatus();
    }while(g_xStatus.MC_STATE!=MC_STATE_READY);

  SdkEvalSetHasEeprom(EepromIdentification());
  
  if(!SdkEvalGetHasEeprom()) // EEPROM is not present
  {
    SpiritManagementComputeXtalFrequency();
  }
  else  // EEPROM found
  {
    //read the memory and set the variable
    uint8_t tmpBuffer[32];
    EepromRead(0x0000, 32, tmpBuffer);
    uint32_t xtal;
    float foffset=0;
    if(tmpBuffer[0]==0 || tmpBuffer[0]==0xFF) {
      SpiritManagementComputeXtalFrequency();
      return;
    }
    switch(tmpBuffer[1]) {
    case 0:
      xtal = 24000000;
      SpiritRadioSetXtalFrequency(xtal);
      break;
    case 1:
      xtal = 25000000;
      SpiritRadioSetXtalFrequency(xtal);
      break;
    case 2:
      xtal = 26000000;
      SpiritRadioSetXtalFrequency(xtal);
      break;
    case 3:
      xtal = 48000000;
      SpiritRadioSetXtalFrequency(xtal);
      break;
    case 4:
      xtal = 50000000;
      SpiritRadioSetXtalFrequency(xtal);
      break;
    case 5:
      xtal = 52000000;
      SpiritRadioSetXtalFrequency(xtal);
      break;
    default:
      SpiritManagementComputeXtalFrequency();
      break;
    }
        
    RangeExtType range;
    if(tmpBuffer[5]==0) {
      range = RANGE_EXT_NONE;
    }
    else if(tmpBuffer[5]==1) {
      range = RANGE_EXT_SKYWORKS_169;
    }
    else if(tmpBuffer[5]==2) {
      range = RANGE_EXT_SKYWORKS_868;
    }
    else {
      range = RANGE_EXT_NONE;
    }
    SpiritManagementSetRangeExtender(range);
    
    SpiritManagementSetBand(tmpBuffer[3]);
    

    EepromRead(0x0021,4,tmpBuffer);
    for(uint8_t i=0;i<4;i++)
    {
      ((uint8_t*)&foffset)[i]=tmpBuffer[3-i];
    }
    SpiritManagementSetOffset((int32_t)foffset);
  }
}

void SpiritManagementSetBand(uint8_t value)
{
  s_RfModuleBand = value;
}

uint8_t SpiritManagementGetBand(void)
{
  return s_RfModuleBand;
}

void SpiritManagementSetOffset(int32_t value)
{
  s_RfModuleOffset=value;
}

int32_t SpiritManagementGetOffset(void)
{
  return s_RfModuleOffset;
}


/**
* @}
*/



/**
* @}
*/



/**
* @defgroup RANGE_EXT_MANAGEMENT_FUNCTIONS              SDK SPIRIT Management Range Extender Functions
* @{
*/

void SpiritManagementRangeExtInit(void)
{
  RangeExtType range_type = SpiritManagementGetRangeExtender();
    
  if(range_type==RANGE_EXT_SKYWORKS_169) {
    /* TCXO optimization power consumption */
    SpiritGeneralSetExtRef(MODE_EXT_XIN);
    uint8_t tmp = 0x01; SpiritSpiWriteRegisters(0xB6,1,&tmp);
    
    /* CSD control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_0, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_TX_RX_MODE});
    
    /* CTX/BYP control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_1, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_TX_STATE});
    
    /* Vcont control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_2, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_RX_STATE});
  }
  else if(range_type==RANGE_EXT_SKYWORKS_868) {   
    /* CSD control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_0, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_TX_RX_MODE});
    
    /* CTX/BYP control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_1, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_RX_STATE});
    
    /* Vcont control */
    SpiritGpioInit(&(SGpioInit){SPIRIT_GPIO_2, SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP, SPIRIT_GPIO_DIG_OUT_TX_STATE});
  }
}

RangeExtType SpiritManagementGetRangeExtender(void)
{
  return xRangeExtType;
}

void SpiritManagementSetRangeExtender(RangeExtType xRangeType)
{
  xRangeExtType = xRangeType;
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


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
