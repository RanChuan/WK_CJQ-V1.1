#ifndef _USART1_H
#define _USART1_H
#include "stm32f10x.h"
#include "stdio.h"	
#define RF_AUX PAin(8)
#define RF_M0 PAout(12)
#define RF_M1 PAout(11)
#define USART_REC_LEN  			200  	//定义最大接收字节数 200



void RF_Init(u32 bound);//初始化
u16 RF1_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
u16 RF_GetCmd (uint8_t *buff,u16 len);


				//设置无线配置的时候用
u16 RF_Get (u8 *buff,u16 len);



#endif

