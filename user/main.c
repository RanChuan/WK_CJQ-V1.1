#include "public.h"
#include "collect.h"
#include "usart3.h"
#include "delay.h"
#include "sys.h"
#include "interruptdef.h"
//#include "usart1.h"
#include "usart1.h"
#include "deal.h"
#include "led.h"
#include "myiic.h"
//#incldue "flash.h"
#include "light.h"
#include "key.h"
#include "wdg.h"
#include "sgp30.h"
#include "sensirion_i2c.h"

/*****************************
*串口一（串口四）用来于集中器进行通讯
*串口三用来采集pm2.5和pm10
*****************************/

int main()
{
	u8 sgp_state=0;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2800);
	NVIC_Configuration();
	delay_init();
	USART3_Init(9600);
	IIC_Init();
	RF_Init(115200);
	time3_init(ENABLE,1000);//每1s加一个计数
	LED_Init();
	addressIdinit();
	Light_Init();
	Key_Init ();
	M0=0;
	M1=0;
	if (sgp_probe()==STATUS_OK)
	{
		sgp_state=1;
	}
	
	IWDG_Init(7,3140); 

	
	while(1)
	{
		IWDG_Feed();
		collectdeal();
		delay_ms(20);
		deal();
		if (sgp_state==0)
		{
			if (sgp_probe()==STATUS_OK)
			{
				sgp_state=1;
			}
		}
	}
	return 0;
}
