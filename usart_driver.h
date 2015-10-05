#ifndef USART   
#define USART

#include "stm32l1xx.h"
#include "stm32l1xx_usart.h"

void usart_init(void);
void usart_send_string(char*str);


#endif