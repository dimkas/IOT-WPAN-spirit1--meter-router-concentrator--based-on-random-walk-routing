#include "application.h"


__IO uint8_t flag100mic;
#ifdef METER

void build_msg(Meter* meter)
{
/* your data to send  */
meter->msg.data[0]='T';
meter->msg.data[1]=':';
meter->msg.data[2]='4';
meter->msg.data[3]='0';
meter->msg.data[4]=':';
meter->msg.data[5]='T';
meter->msg.data[6]='E';
meter->msg.data[7]='S';
meter->msg.data[8]='T';
/****************************/

meter->msg.seq++;
meter->msg.seq%=255;
//..
}

void build_interrupt_msg(Meter* meter)
{
meter->msg.seq++;
meter->msg.seq%=255;
//..
}

void interrupt(Meter* meter)
{
  meter->type=APRIODIC;
  meter->state=BUILD_MSG;
  meter->period_time=0;
}

void AppDecTemposrisateurs(Meter * meter)
{
      if(flag100mic)
      {
      flag100mic=0;
      if(meter->ack_time)
      meter->ack_time--;
      if(meter->period_time)
      meter->period_time--;
      }
}

void init_application(Meter * meter )
{
/* initialisation */
meter->ack_time_out=ACK_TIME_OUT;
meter->period_delay=PERIOD_DELAY;
meter->period_time=meter->period_delay;
meter->state=BUILD_MSG;
meter->type=PRIODIC;

/* configure msg */
meter->msg.seq=0;
meter->msg.src=SpiritPktCommonGetMyAddress();
meter->msg.dst=0x00 /*SpiritPktCommonGetTransmittedDestAddress()*/; //// 0x00 just for test 
meter->msg.preffered_adresse=PEREFFERED_ADRESS;
#ifdef NEED_ACK1
meter->msg.type= (Type) NEED_ACK ;
#endif
#ifdef NO_ACK1
meter->msg.type=NO_ACK;
#endif
}

void routine_application(Meter * meter )
{
   static uint8_t i=0 ;
 AppDecTemposrisateurs(meter); 
      switch(meter->state)
      {
          case BUILD_MSG:
                          if(!meter->period_time ) // period time out
                          {
                            meter->state=SEND_DATA;
                            /*  load the timer  */ 
                            meter->period_time=meter->period_delay;
                            /* initialise the ack_retry counter  */
                            meter->ack_retry=0;
                            
                            if(meter->type==PRIODIC)
                            build_msg(meter);
                            else
                            {
                            build_interrupt_msg(meter);
                            meter->type=PRIODIC;
                            }
                            
                          }
          break;
          case SEND_DATA:
                          if(!fifo_is_full(meter->network.tx_fifo))
                          {
                          SdkDelayMs(50);
                          send_spirit_msg(meter->msg,&meter->network);  
                          }
                          if( meter->msg.type == NEED_ACK )
                          {
                            /* load the timer of ACK */ 
                          meter->ack_time=meter->ack_time_out; 
                          meter->state=WAIT_ACK;                     
                          }
                          else 
                          {
                          meter->state=BUILD_MSG;
                          }
          break;
          case WAIT_ACK:
                          if(!meter->ack_time ) // if ack_time_out  ==> resend the msg if we do not passed the max_ack_retry
                          {
                                   if(meter->ack_retry < MAX_ACK_RETRY )
                                   {
                                   meter->state=SEND_DATA;
                                   meter->ack_retry++;
                                   }
                                   else
                                   {
                                   meter->state=BUILD_MSG;
                                   }
                          }
                          else //  verify if we receive an acquittal
                          {
                                  if(meter->network.rx_fifo[i].state==RX_MSG && meter->network.rx_fifo[i].type== ACK  && meter->network.rx_fifo[i].seq==meter->msg.seq  )
                                  {
                                  meter->state=BUILD_MSG;
                                  meter->network.rx_fifo[i].state=EMPTY;
                                  }              
                                  else
                                  {
                                  i++;
                                  i%=MAX_SPIRIT_MSG;
                                  }
                          }      
          break;
      }
}
#endif
#ifdef CONCENTRATOR

void send_msg_to_server(spirit_msg msg)
{
//// send data to server 
}

void init_application(Concentrator * concentrator )
{

}

void routine_application(Concentrator * concentrator )
{
 static uint8_t i=0 ;
      switch(concentrator->state)
      {
          case LISTENING:
              if(concentrator->network.rx_fifo[i].state==RX_MSG)
              {
              concentrator->state=SEND_DATA_TO_SERVER;
              }              
              else
              {
              i++;
              i%=MAX_SPIRIT_MSG;
              }
          break;
          case SEND_DATA_TO_SERVER:
                send_msg_to_server (concentrator->network.rx_fifo[i]);
                if(concentrator->network.rx_fifo[i].type==NEED_ACK)
                concentrator->state=SEND_ACK;
                else
                {
                concentrator->state=LISTENING;
                concentrator->network.rx_fifo[i].state=EMPTY;
                }
          break;
          case SEND_ACK:
               concentrator->state=LISTENING;
               /* prepare ack msg to send */
               concentrator->network.rx_fifo[i].type=ACK;
               
               
               for(uint8_t j=0; j<11 ;j++)
               concentrator->network.rx_fifo[i].data[j]=0;


               concentrator->network.rx_fifo[i].dst= concentrator->network.rx_fifo[i].src;
               SpiritPktCommonSetDestinationAddress(concentrator->network.rx_fifo[i].dst); 
               concentrator->network.rx_fifo[i].src = MY_ADDRESS /*SpiritPktCommonGetMyAddress()*/;
               
               /* add ack to tx_fifo if it is not full  */
               if(!fifo_is_full(concentrator->network.tx_fifo))
               {SdkDelayMs(50);
               send_spirit_msg(concentrator->network.rx_fifo[i] , &concentrator->network) ;
               }
               /* forget the received msg */
               concentrator->network.rx_fifo[i].state=EMPTY;
          break;
      }
}

#endif
#ifdef ROUTER

extern uint16_t random ;
uint16_t get_random_int()
{
return random ;
}

uint8_t Add_me_as_router(spirit_msg * msg)
{
uint8_t i ;
            for(i=0 ; i<MAX_ROUTER ; i++)
            {
                    if(!msg->routeurs[i])
                    {
                    msg->routeurs[i]= MY_ADDRESS /* SpiritPktCommonGetMyAddress()*/ ; // to change if it take much time 
                    return 1;
                    }
            }
            return 0;
}


uint8_t is_equal(spirit_msg  msg1 , spirit_msg  msg2) 
{
if ( msg1.dst==msg2.dst && msg1.src==msg2.src && msg1.seq==msg2.seq && msg1.type==msg2.type && msg1.state==msg2.state )
return 1 ;
else
return 0 ;
}

void AppDecTemposrisateurs(Router * router )
{
      if(flag100mic)
      {
      flag100mic=0;
      if(router->time_to_wait)
      router->time_to_wait--;
      }
}

void init_application(Router * router )
{
/* initialisation */
router->seuil=SEUIL;
}

extern uint8_t last_msg ;
void routine_application(Router * router )
{
   static uint8_t i=0 ;
 AppDecTemposrisateurs(router); 
      switch(router->state)
      {
          case LISTENING1:
                        if(router->network.rx_fifo[i].state==RX_MSG)
                        {
                        
                        router->state=GET_RANDOM;
                        }              
                        else
                        {
                        i++;
                        i%=MAX_SPIRIT_MSG;
                        }
          break;
          case GET_RANDOM:
                        router->random=get_random_int();
                         /*  
                        verify threshold
                        verify the number of max routing for a packet
                        verify the preffered_adresse
                        */ 
                        if (router->random > router->seuil || router->network.rx_fifo[i].routeurs[MAX_ROUTER-1]  || (router->network.rx_fifo[i].preffered_adresse != MY_ADDRESS /*SpiritPktCommonGetMyAddress()*/ &&  router->network.rx_fifo[i].preffered_adresse!=BROADCAST_ADDRESS) ) // no send 
                        {
                        router->state=LISTENING1;
                        router->network.rx_fifo[i].state=EMPTY ;
                        }
                        else 
                        {         if (router->network.rx_fifo[i].preffered_adresse==MY_ADDRESS /*SpiritPktCommonGetMyAddress()*/) // my adresse is the preffered
                                  {
                                  router->state=SEND_MSG; // send without waiting
                                  }
                                  else if(router->network.rx_fifo[i].preffered_adresse==BROADCAST_ADDRESS) // send using random walk
                                  {                                 
                                  router->state=WAIT;
                                  router->time_to_wait=router->random;
                                  }
                        }
          break;
          case WAIT:
                        if(!router->time_to_wait) // time_to_wait is out ==> send the  msg 
                          router->state=SEND_MSG;
                        else //verify if we receive the same msg (from other router ...)
                        {
                              if(is_equal(router->network.rx_fifo[i] , router->network.rx_fifo[last_msg] ) && i != last_msg )
                              {
                              router->state=LISTENING1;
                              router->network.rx_fifo[last_msg].state=EMPTY ;
                              router->network.rx_fifo[i].state=EMPTY ;
                              }
                        }
          break;
          case SEND_MSG :
                        Add_me_as_router(&router->network.rx_fifo[i]);
                        if(!fifo_is_full(router->network.tx_fifo))
                        {
                        SpiritPktCommonSetDestinationAddress(router->network.rx_fifo[i].dst);
                        send_spirit_msg(router->network.rx_fifo[i] , &router->network) ;
                        }
                        router->state=LISTENING1;
                        router->network.rx_fifo[i].state=EMPTY ;
          break;
      }
}

#endif
