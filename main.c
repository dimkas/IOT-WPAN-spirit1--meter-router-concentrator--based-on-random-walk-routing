/* Includes ------------------------------------------------------------------*/
#include "application.h"

#ifdef METER
Meter app ;
#elif CONCENTRATOR
Concentrator app ;
#elif ROUTER
Router app ;
#endif

extern spirit_driver spirit ;
void M2S_GPIO_2_EXTI_IRQ_HANDLER(void)
{
  /* Check the flag status of EXTI line */
  if(EXTI_GetITStatus(M2S_GPIO_2_EXTI_LINE))
  {

    /* Get the IRQ status */
    SpiritIrqGetStatus(&spirit.xIrqStatus);
    /* Check the SPIRIT TX_DATA_SENT IRQ flag */
    if(spirit.xIrqStatus.IRQ_TX_DATA_SENT)
    {
      /* set the tx_done_flag  */
      spirit.xTxDoneFlag = SET; 
 
    }
    if(spirit.xIrqStatus.IRQ_MAX_BO_CCA_REACH)
    {
      /* Send a Tx command */
      SpiritCmdStrobeTx();
    }
        /* Check the SPIRIT RX_DATA_DISC IRQ flag */
    if(spirit.xIrqStatus.IRQ_RX_DATA_DISC)
    {
      SdkEvalLedToggle(LED1);     
      /* RX command */
      SpiritCmdStrobeRx();  
    }
    /* Check the SPIRIT RX_DATA_READY IRQ flag */
    if(spirit.xIrqStatus.IRQ_RX_DATA_READY)
    {      
      spirit.xRxDoneFlag = SET;
      /* Get the RX FIFO size */
      spirit.cRxData=SpiritLinearFifoReadNumElementsRxFifo()- 2 ;
      /* Read the RX FIFO */
      SpiritSpiReadLinearFifo(spirit.cRxData, spirit.vectcRxBuff); 
      /* Flush the RX FIFO */
      SpiritCmdStrobeFlushRxFifo(); 
      spirit.state=RECEIVED_DATA_READY; //   data received :)
      /* RX command */
      SpiritCmdStrobeRx();  
    }
    
    /* Clear the EXTI line flag */
    EXTI_ClearITPendingBit(M2S_GPIO_2_EXTI_LINE);
  }
  
}

void main (void)
{
  /* initilisation */
  Init_spirit_driver(&spirit);
  init_network(app.network.tx_fifo , app.network.rx_fifo);
  init_application(&app );

  /* infinite loop */
  while (1)
  {
  routine_spirit_driver (&spirit);
  routine_network(app.network.tx_fifo , app.network.rx_fifo) ;
  routine_application(&app );
  }
}



#ifdef  USE_FULL_ASSERT
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


/********** (C) COPYRIGHT 2015 Nid *********/
