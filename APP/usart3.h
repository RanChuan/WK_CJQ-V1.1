#ifndef __USART3_H
#define __USART3_H

#define MAX_RCV_LEN  100
#include "stdio.h"
#include "sys.h"


extern void USART3_Init(u32 bound);
//extern u8 TVOC_value[4];
//extern unsigned char  usart3_rcv_buf[MAX_RCV_LEN];
//extern volatile unsigned int   usart3_rcv_len;
int Myitoa_16_to_10(unsigned char num);
void usart2_sendData(unsigned char *dat,u8 len);
void usart2_receiveData(u8 rxdat[],u8 *data_len);
void something(void);
void get_data(void);
#endif


