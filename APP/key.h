#ifndef KEY_H_
#define KEY_H_
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "sys.h"



extern u8 AllocationFlag;
#define KEY  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//¶ÁÈ¡°´¼ü3(WK_UP) 
#define LED   PBout(2)

//void Light_Init(void);
void Key_Init (void);



#endif


