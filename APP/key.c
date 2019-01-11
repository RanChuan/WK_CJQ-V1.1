#include "key.h"




u8 AllocationFlag=0;



void Key_Init (void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA15

	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);//KEY1
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
	
}
     

void EXTI15_10_IRQHandler(void)
{
		delay_ms(10);//消抖

	if(EXTI_GetITStatus(EXTI_Line15)!=RESET)//KEY1
	{

		if(KEY==0)
		{
//			GPIO_ResetBits(GPIOB,GPIO_Pin_2);						 //PB.2 输出低
//			LED=!LED;
			AllocationFlag=!AllocationFlag;
		}
		EXTI_ClearFlag(EXTI_Line15);
		
	}
	
}
