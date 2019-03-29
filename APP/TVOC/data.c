
#include "data.h"








int ai_to_04(uint8_t M,uint8_t N)
{
		char T_H_P[10];
		int m;
		sprintf(T_H_P,"%02x%02x",M,N);
		sscanf(T_H_P,"%x",&m );  
		return m;	
}
