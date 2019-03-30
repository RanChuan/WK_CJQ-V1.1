#ifndef __DATA_H
#define __DATA_H			    
#include "sys.h" 

#include "string.h"

#include "sht21.h"
#include "usart3.h"


#include "tvoc.h"

int ai_to_04(uint8_t M,uint8_t N);


typedef union
{
	float f;
	u8 u[4];
} f_to_u;



#endif
