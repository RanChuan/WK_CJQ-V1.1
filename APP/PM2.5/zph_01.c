#include "zph_01.h"



float PM_Ture()//PM2.5数值函数
{
	int k1,k2;
	float PM;
	k1 = Myitoa_16_to_10(PM_value[3]);
	k2 = Myitoa_16_to_10(PM_value[4]); 
	if(PM_value[8]==Check(PM_value))
		{
			//printf("%d.",k1);
			//printf("%d--",k2);
			PM=(k1+k2*0.01)*21;
			return PM;
		}
		return 0;
}

unsigned char Check(unsigned char *i)//PM2.5校验函数
{
	unsigned char j,tempq=0;
	i+=1;
	
	for(j=0;j<7;j++)
	{
		tempq+=*i;
		i++;
	} 
	tempq=(~tempq)+1;
	return(tempq);
}
