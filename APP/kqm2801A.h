#ifndef _KQM2801A_H
#define _KQM2801A_H

#include "stm32f10x.h"

extern u8 TvocValue[4];
extern u8 TVOCValue[2];   //保存TVOC数据

float KQM2801_Value(char whatdo);
void Get_TVOC();
void SoftReset1(void); //软重启
void Set_Resolution1(void);

#endif


