#ifndef _USART1_H
#define _USART1_H
#include "stm32f10x.h"

#define USART1_TX GPIO_Pin_9	//PA9
#define USART1_RX GPIO_Pin_10	//PA10
extern u8 ReviceFlat;
extern u8 MaxLenth;
extern u8 usart1_buf[256];
extern u8 flag_rx485;

void usart1_init(u32 baud);
void usart1_sendData(unsigned char *dat,u8 len);
void usart1_receiveData(u8 rxdat[],u8 *data_len);



#endif

