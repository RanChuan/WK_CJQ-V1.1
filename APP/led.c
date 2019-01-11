#include "led.h"
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 
	GPIO_SetBits(GPIOA,GPIO_Pin_8);						

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //LED2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  
	
//	GPIO_SetBits(GPIOA,GPIO_Pin_11); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 //LED3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				
	//GPIO_SetBits(GPIOA,GPIO_Pin_12); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		 //LED4
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 
	//GPIO_SetBits(GPIOA,GPIO_Pin_13); 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	    		 //LED5
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 
	GPIO_SetBits(GPIOA,GPIO_Pin_14); 		
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //LED6
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
 

