
#define SDK_SYSTEM_CONFIG()           { NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0000 ); \
                                        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE, ENABLE ); \
                                        SdkEvalIdentification(); \
                                        SdkStartSysTick(); \
                                        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); \
                                        SdkEvalPmRfSwitchInit(); \
                                        SdkEvalPmRfSwitchToVRf(); \
                                        SdkEvalPmI2CInit(); \
                                        SdkEvalPmADCInit(); \
                                        SdkEvalPmRegulateVRfI(3.3); \
                                        SdkEvalLedInit(LED1); \
                                        SdkEvalLedInit(LED2); \
                                        SdkEvalLedInit(LED3); \
                                        SdkEvalLedInit(LED4); \
                                        SdkEvalLedInit(LED5); \
                                        SdkEvalM2SGpioInit(M2S_GPIO_SDN,M2S_MODE_GPIO_OUT); \
                                          SpiritSpiInit();  \
                                        EepromSpiInitialization(); \
                                      }
