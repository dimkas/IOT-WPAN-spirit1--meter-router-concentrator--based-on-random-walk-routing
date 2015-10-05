#include "network.h"


uint8_t fifo_is_full(spirit_msg* msg)
{
    for  (  uint8_t i=0; i<MAX_SPIRIT_MSG ; i++ )
    { 
    if (msg[i].state == EMPTY)
    return 0 ; 
    }
    return 1 ; 
}


void send_spirit_msg(spirit_msg msg , Network* network) // use is_tx_fifo_full first to verify that you can send the msg 
{
static uint8_t i =0;

while (network->tx_fifo[i].state != EMPTY ) /* looking for empty packet to use */
      {
      i++;
      }
      if (network->tx_fifo[i].state == EMPTY ) /* use to the empty packet to send the new msg  */
      {
        network->tx_fifo[i].src=msg.src ;
        network->tx_fifo[i].dst=msg.dst ;
        network->tx_fifo[i].preffered_adresse=msg.preffered_adresse ;
        network->tx_fifo[i].type=msg.type ;
        network->tx_fifo[i].seq=msg.seq ;
        for(uint8_t a=0;a<MAX_ROUTER;a++)
        network->tx_fifo[i].routeurs[a] = msg.routeurs[a] ;
        for(uint8_t a=0;a<DATA_LENGTH;a++)
        network->tx_fifo[i].data[a] = msg.data[a] ;
        network->tx_fifo[i].state=READY_TO_SEND;
      }
}