#ifndef _INTERRUPTDEF_H
#define _INTERRUPTDEF_H
#include "stm32f10x.h"

extern u16 time3_count;
extern u8 controlId;
void nvic_config(void);
void time2_init(u8 config,u16 ms);
void time3_init(u8 config,u16 ms);
void SYSTIME_STOP (void);


void SYSTIME_START (void);




#endif
