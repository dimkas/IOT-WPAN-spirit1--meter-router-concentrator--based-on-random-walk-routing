#include "spirit_msg.h"

spirit_driver spirit ;

void send_spirit_msg_to_buffer(spirit_msg * msg) // send the msg to the driver layer 
{
spirit.vectcTxBuff[0]=msg->src;
spirit.vectcTxBuff[1]=msg->dst;
spirit.vectcTxBuff[2]=msg->preffered_adresse;
spirit.vectcTxBuff[3]=msg->seq;
spirit.vectcTxBuff[4]= (uint8_t) msg->type;
for(uint8_t i=0;i<MAX_ROUTER;i++)
spirit.vectcTxBuff[i+5]=msg->routeurs[i];
for(uint8_t i=0;i<DATA_LENGTH;i++)
spirit.vectcTxBuff[i+5+MAX_ROUTER]=msg->data[i];
spirit.cTxdata=5+MAX_ROUTER+DATA_LENGTH;
}

void get_spirit_msg_from_buffer(spirit_msg * msg) // get msg from driver layer 
{
msg->src=spirit.vectcRxBuff[0];
msg->dst=spirit.vectcRxBuff[1];
msg->preffered_adresse=spirit.vectcRxBuff[2];
msg->seq=spirit.vectcRxBuff[3];
msg->type =  (Type) spirit.vectcRxBuff[4];
for(uint8_t i=0; i<MAX_ROUTER ;i++)
msg->routeurs[i]=spirit.vectcRxBuff[i+5];
for(uint8_t i=0; i<spirit.cRxData-5-MAX_ROUTER ;i++)
msg->data[i]=spirit.vectcRxBuff[i+5+MAX_ROUTER];
msg->state=RX_MSG;
}

uint8_t last_msg ;
void routine_network (spirit_msg * tx_msg  , spirit_msg * rx_msg)
{
static uint8_t i=0 , j=0 ;

          if (spirit.state != RECEIVED_DATA_READY)
          {
                    switch (tx_msg[i].state)
                    {
                      
                    case EMPTY : 
                                /* if the msg is empty , we go to the next one */
                                i++;
                                i%=MAX_SPIRIT_MSG;
                    break;
                    
                    case READY_TO_SEND : 
                                /* send msg to the buffer (of driver layer ) and switch to SENDING state */
                                tx_msg[i].state=SENDING;     
                                send_spirit_msg_to_buffer(tx_msg);
                                spirit.state=TX;
                    break;
                    
                    case SENDING : 
                                /* wait for  TX Flag */
                                if(spirit.xTxDoneFlag)
                                {
                                tx_msg[i].state=EMPTY; 
                                }                   
                    break;
                    }

          }
          else
          {   
            
              #ifdef ENABLE_NETWORK_FILTER
            if (spirit.vectcRxBuff[1]== SpiritPktCommonGetMyAddress())
              {
              #endif 
                    while (rx_msg[j].state!=EMPTY && j < MAX_SPIRIT_MSG )  
                    {
                      j++;
                    }
                    if (rx_msg[j].state==EMPTY)
                    {
                      get_spirit_msg_from_buffer(&rx_msg[j]);
                      spirit.state=IDLE;
                      spirit.xRxDoneFlag=RESET;
                    }
                    else
                    {
                    /* if rx_fifo is full then crush the oldest msg */
                      get_spirit_msg_from_buffer(&rx_msg[0]);
                      spirit.state=IDLE;
                      spirit.xRxDoneFlag=RESET;
                    }
                    last_msg=j;
                    j++;
                    j%=MAX_SPIRIT_MSG;
              #ifdef ENABLE_NETWORK_FILTER
                }
                else
                {                      
                spirit.state=IDLE;
                spirit.xRxDoneFlag=RESET;
                }
              #endif 
          }
}

void init_network (spirit_msg * tx_msg  , spirit_msg * rx_msg)
{

}