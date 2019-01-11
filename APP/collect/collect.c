#include "collect.h"
#include "sht21.h"
#include "tvoc.h"
#include "zph_01.h"
//#include "usart3.h"
#include "interruptdef.h"
#include "crc8_16.h" 
#include "flash.h"
#include "deal.h"
#include "usart1.h"
//#include "kqm2801A.h"
#include "tvoc.h"
#include "kqm2801A.h"


u8 collectNumber=1;	//采集器总数量
u8 centerNumber=1;	//集中器总数量
u8 controlNumber=1;	//控制器总数量
u8 collectData[COLLECTSIZE];
u16 collectId=2;  //采集板编号     通过配置给


void addressIdinit(void)
{
	u16 t;
	flash_Read(FLASH_SAVE_ADDR,&t,1);
	if((t&0XFFFF)!=0xFFFF)  //判断是否配置过   配置过  flash在没有数据的时候是0xff
	{
//		collectId=dataBuffer[0];
		collectId=t;
	}
	else 
	{
		Nothing();
	}
}


void collectdeal(void)
{
	if(time3_count>=5)//每5秒采集一次数据
	{
		Get_TempAmdHumi();         //温湿度获取
		Get_TVOC();
		get_data();
		time3_count=0;
	}
}

void returnEnvironmentData(u8 address)   
{
	    u8 crc[2];
			u8 collectData[30];

//			u8 pmInt = (u8)pm;
//			u8 pmB = pmInt/100;
//			u8 pmSG = pmInt%100;
//			u8 pmX = (u8)(pm*10)%10;

	
			collectData[0]=0xff;
			collectData[1]=0xff;
			collectData[2]=0x00;  //地址位
			collectData[3]=address;  //地址位
			collectData[4]=0x03;
			collectData[5]=0x00;
			collectData[6]=0x09;
//			collectData[8]=50;
//			collectData[9]=2;
//			collectData[10]=80;
//			collectData[11]=6;
			collectData[7]=TempAndHumi[0];
			collectData[8]=TempAndHumi[1];
			collectData[9]=TempAndHumi[2];
			collectData[10]=TempAndHumi[3];
			collectData[11]=0;             //pmB;
			collectData[12]=0;            //pmSG;
			collectData[13]=0;             //pmX;
			collectData[14]=TVOCValue[0];            
			collectData[15]=TVOCValue[1];             
			Get_Crc16(collectData,16,crc);
			collectData[16]=crc[0];
			collectData[17]=crc[1];
					
		//	usart1_sendData(collectData,18);
		
}



void Nothing(void)
{
	dataBuffer[0]=2;
	collectId=dataBuffer[0];
}

















