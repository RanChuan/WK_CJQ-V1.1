#ifndef _SHT21_H
#define _SHT21_H
#include "stm32f10x.h"

extern u8 TempAndHumi[8];

float SHT2X_MeasureNHM(char whatdo);
void Get_TempAmdHumi(void);

#endif
