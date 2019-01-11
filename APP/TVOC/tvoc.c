#include "tvoc.h"

//void My_TVOC(char whatdo,uint8_t reg[4])
//{
//	uint8_t i;
//	IIC_SCL = 0;
//	SDA_OUT();
//	IIC_SDA = 0;
//	delay_us(0x05);
//	IIC_SCL = 1;
//	delay_us(0x01);
//	IIC_SDA = 1;
//	delay_us(5);
//	
//	
//	IIC_Start();
//	SDA_OUT();
//	IIC_SCL=0;
////*******************************1         0
//	IIC_SDA=0;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//	delay_us(2);
////*******************************2         1
//	IIC_SDA=1;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//	delay_us(2);
////*******************************3         0
//	IIC_SDA=0;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//	delay_us(2);
////*******************************4        1
//	IIC_SDA=1;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//	delay_us(2);
////*******************************5				1
//	IIC_SDA=1;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//	delay_us(2);
////*******************************6				1
//	IIC_SDA=1;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//	delay_us(2);
////*******************************7				1
//	IIC_SDA=1;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//	delay_us(2);
////*******************************8				1
//	IIC_SDA=1;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//	delay_us(2);
////*******************************9				1
//	IIC_SDA=1;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
//	delay_us(2);
//	reg[0]=IIC_Read_Byte(0);
//	delay_us(50);
//	reg[1]=IIC_Read_Byte(0);
//	delay_us(50);
//	reg[2]=IIC_Read_Byte(0);
//	delay_us(50);
//	reg[3]=IIC_Read_Byte(0);
//	IIC_Stop();
//}

float TVOC_Ture()//TVOCÊýÖµº¯Êý
{
	int TVOC;
	if((TVOC_value[1]|TVOC_value[2])!=0xFF)
	{
		if(TVOC_value[3]==TVOC_value[0]+TVOC_value[1]+TVOC_value[2])
		{
			TVOC =ai_to_04( TVOC_value[1] , TVOC_value[2]);
			//printf("%c%c%d",TVOC_value[1],TVOC_value[2],TVOC);
			return TVOC;
		}
	}
		return 0;
}


