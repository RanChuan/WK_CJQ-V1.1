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
/*****************************
*����һ�������ģ������ڼ���������ͨѶ
*�����������ɼ�pm2.5��pm10
*****************************/

int main()
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2800);
	NVIC_Configuration();
	delay_init();
	USART3_Init(9600);
	IIC_Init();
	RF_Init(115200);
	time3_init(ENABLE,1000);//ÿ1s��һ������
	LED_Init();
	addressIdinit();
	Light_Init();
	Key_Init ();
	M0=0;
	M1=0;
	IWDG_Init(7,3140); 
	
	while(1){
		IWDG_Feed();
		collectdeal();
		delay_ms(20);
		deal();
	}
	return 0;
}