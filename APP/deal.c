#include "deal.h"
#include "usart3.h"
//#include "usart1.h"
#include "collect.h"
#include "interruptdef.h"
#include "crc8_16.h"
#include "flash.h"
#include "usart1.h"
#include "key.h"
#include "interruptdef.h"
#include "sht21.h"
#include "kqm2801A.h"

#include "delay.h"
#include "zph_01.h"


#define LED   PBout(2)
u16 dataBuffer[20];


extern u8 AllocationFlag;

			//处理无线命令
void deal(void)
{
	u8 recv[40]={0};
	u8 crc[2];
	u16 length=0;
	LED=!AllocationFlag;
	if(RF_GetCmd(recv,40))
	{
		length=(recv[5]<<8)|recv[6];
		Get_Crc16(recv,length+7,crc);
		if (crc[0]==recv[length+7]&&crc[1]==recv[length+8])
		{
			if ((collectId==((recv[2]<<8)|recv[3]))||(recv[4]>=6&&recv[4]<=8))
			{
				switch (recv[4])
				{
					case 0x01:
						cmd_0x01(recv);
						break;
					case 0x04:
						cmd_0x04(recv);
						break;
					case 0x06:
						cmd_0x06(recv);
						break;
					case 0x07: 
						cmd_0x07(recv);
						break;
					case 0x09:
						cmd_0x09(recv);
						break;
					default :
						break;
				}
			}
		}
	}
}











//获取指定内存地址的值0,成功，1，失败
u32 getMemValue (u8 *buff,u32 addr,u16 len)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u8 *base_addr=(u8 *)addr;
	if (base_addr==0) return 1;
	//OS_ENTER_CRITICAL();
	while(len--)
	{
		*buff++=*base_addr++;
	}
	//OS_EXIT_CRITICAL();
	return 0;
}



u32 setMemValue (u8 *buff,u32 addr,u16 len)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	u8 *base_addr=(u8 *)addr;
	if (base_addr==0) return 1;
	//OS_ENTER_CRITICAL();
	while(len--)
	{
		*base_addr++=*buff++;
	}
	//OS_EXIT_CRITICAL();
	return 0;
}





#include "string.h"


void cmd_0x04 (u8 *recv)
{
	u32 addr;
	u16 datalen=(recv[12]<<8)|recv[13];
	u8 data[58]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=collectId>>8;
	data[3]=collectId;
	data[4]=recv[4]|0x80;
	data[5]=(datalen+2)>>8;
	data[6]=datalen+2;
	data[7]=0;	//默认是成功
	data[8]=0;
	if (datalen>58-12) //重新调整数据长度
	{
		datalen=58-12;
	}
	addr=(recv[8]<<24)|(recv[9]<<16)|(recv[10]<<8)|recv[11];
	
	if (recv[7]==0)//读
	{
		memcpy (&data[9],(u8 *)addr,datalen);
		
		Get_Crc16(data,7+(datalen+2),crc);
		data[7+(datalen+2)]=crc[0];
		data[7+(datalen+2)+1]=crc[1];
		RF1_tx_bytes(data,7+(datalen+2)+2);//发送返回数据
	}
	else if (recv[7]==1)//写
	{
		u16 wdatalen=(recv[5]<<8)|recv[6];
		wdatalen-=5;
		memcpy ((u8 *)addr,&recv[12],datalen);
		Return_NEW(recv[4],ERR_SUCCESS);
	}
}













void cmd_0x06(u8 * buff)
{

	u8 data[30]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=collectId>>8;
	data[3]=collectId;
	data[4]=buff[4]|0x80;
	data[5]=0x00;
	data[6]=3;
	data[7]=0;		//错误类型
	data[8]=0;
	AllocationFlag=buff[7];
	data[9]=AllocationFlag;

	Get_Crc16(data,data[6]+7,crc);
	data[data[6]+7]=crc[0];
	data[data[6]+8]=crc[1];
	RF1_tx_bytes(data,data[6]+9);//发送返回数据
	
}


void cmd_0x07(u8 * buff)
{

	if (AllocationFlag)
	{
		
		if (buff[9]!=1)
		{
			Return_NEW(buff[4],ERR_DATAVALE);
			return;
		}
		
		collectId=(buff[7]<<8)|buff[8];
		flash_Write(FLASH_SAVE_ADDR,&collectId,1);
		Return_NEW(buff[4],ERR_SUCCESS);
	}
	else
	{
		Return_NEW(buff[4],ERR_CANNOTCFG);
	}
	
}


void cmd_0x09 (u8 *buff)
{
	if (buff[7]==2)
	{
		Return_NEW(buff[4],ERR_SUCCESS);
		delay_ms(100);
		NVIC_SystemReset();
	}
	else
	{
		Return_NEW(buff[4],ERR_DATAVALE);
	}
}




#include "data_map.h"


void cmd_0x01(u8 *recv)
{ 
	u8 data[50]={0};
	u8 velues[40]={0};
	u8 crc[2]={0};
	u8 i=0;
	u8 wantlenth=(recv[7]<<8)|recv[8];
	extern u8 PM2_5_value[4];
	extern u8 CO2Value[4];
	data[0]=0xff;
	data[1]=0xff;
	data[2]=collectId>>8;
	data[3]=collectId;
	data[4]=0x01|0x80;
	
	
	if (wantlenth>30)//超出最大长度
	{
		wantlenth=2;
		data[5]=0;
		data[6]=wantlenth;
		data[7]=0;
		data[8]=ERR_WANTLENGTH;
		
	}
	else
	{
		data[5]=wantlenth>>8;
		data[6]=wantlenth;	
			//错误类型:成功
		velues[0]=0;		
		velues[1]=0;				
					//数据位
		velues[2]=0x01;//本机类型
		velues[3]=1;//是否在工作
			velues[4]=DATA_MAP->sysRunTime>>24;
			velues[5]=DATA_MAP->sysRunTime>>16;
			velues[6]=DATA_MAP->sysRunTime>>8;
			velues[7]=DATA_MAP->sysRunTime>>0;
		velues[8]=3;//传感器个数
		
		
		
		velues[9]=0;//传感器故障
		//添加传感器故障个数
		if ((TempAndHumi[0]==0&&TempAndHumi[1]==0&&TempAndHumi[2]==0&&TempAndHumi[3]==0)&&
			(TempAndHumi[4]==0)&&(TempAndHumi[5]==0)&&(TempAndHumi[6]==0)&&(TempAndHumi[7]==0))
		{
			velues[9]++;
		}
		if ((TVOCValue[0]==0&&TVOCValue[1]==0)&&(TVOCValue[2]==0&&TVOCValue[3]==0))
		{
			velues[9]++;
		}
		
		if ((PM2_5_value[0]==0)&&(PM2_5_value[1]==0)&&(PM2_5_value[2]==0)&&(PM2_5_value[3]==0))
		{
			velues[9]++;
		}
		
		
		
		
		velues[10]=TempAndHumi[0];//温度
		velues[11]=TempAndHumi[1];//
		velues[12]=TempAndHumi[2];//
		velues[13]=TempAndHumi[3];//
		
		velues[14]=TempAndHumi[4];//湿度
		velues[15]=TempAndHumi[5];//
		velues[16]=TempAndHumi[6];//
		velues[17]=TempAndHumi[7];//

		velues[18]=TVOCValue[0];	//tvoc
		velues[19]=TVOCValue[1];
		velues[20]=TVOCValue[2];
		velues[21]=TVOCValue[3];

		velues[22]=CO2Value[0];			//co2
		velues[23]=CO2Value[1];			//
		velues[24]=CO2Value[2];			//
		velues[25]=CO2Value[3];			//
		
		velues[26]=PM2_5_value[0];
		velues[27]=PM2_5_value[1];
		velues[28]=PM2_5_value[2];
		velues[29]=PM2_5_value[3];
		
		for(i=0;i<wantlenth;i++)
		{
			data[7+i]=velues[i];
		}
	}
	
	Get_Crc16(data,7+wantlenth,crc);
	data[7+wantlenth]=crc[0];
	data[7+wantlenth+1]=crc[1];
	RF1_tx_bytes(data,7+wantlenth+2);//发送返回数据
	


	
}


void Return_NEW (u8 cmd,u16 err)
{
	u8 data[20];
	u8 crc[2];
	
	data[0]=0xff;
	data[1]=0xff;
	data[2]=collectId>>8;
	data[3]=collectId;
	data[4]=cmd|0x80;
	data[5]=0x00;
	data[6]=0x02;
	data[7]=err>>8;		//错误类型
	data[8]=err;
	
	Get_Crc16(data,9,crc);
	data[9]=crc[0];
	data[10]=crc[1];
	RF1_tx_bytes(data,data[6]+9);//发送返回数据
	
}


