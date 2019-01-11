#ifndef _flash_H
#define _flash_H
#include "stm32f10x.h"

//stm32f103RCT6 flash
//#define SCREEN_START_ADDR (0x08000000+127*2048)	//串口屏幕数据地址
//#define ICO_START_ADDR (0x08000000+126*2048)		//串口屏幕图标保存地址
#define SCREEN_START_ADDR (0x08000000+63*1024)	//串口屏幕数据地址
#define ICO_START_ADDR (0x08000000+62*1024)		//串口屏幕图标保存地址
void flash_Write(u32 flashAddr,u16 buf[],u8 len);
void flash_Read(u32 flashAddr,u16 buf[],u8 len);
#endif
