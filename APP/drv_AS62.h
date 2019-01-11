#ifndef __DRV_AS62_H__
#define __DRV_AS62_H__

#include "delay.h"
#include "usart1.h"
//#include "lcd_flash.h"
#include "led.h"

extern uint8_t AS32_Param_Config[ 6 ];
extern uint8_t Cal;
void AS32_Param_Init( uint8_t AS_P_Config[6] );


#endif
