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

			//������������
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
	data[7]=0;		//��������
	data[8]=0;
	AllocationFlag=buff[7];
	data[9]=AllocationFlag;

	Get_Crc16(data,data[6]+7,crc);
	data[data[6]+7]=crc[0];
	data[data[6]+8]=crc[1];
	RF1_tx_bytes(data,data[6]+9);//���ͷ�������
	
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



void cmd_0x01(u8 *recv)
{ 
	u8 data[40]={0};
	u8 velues[25]={0};
	u8 crc[2]={0};
	u8 i=0;
	u8 wantlenth=(recv[7]<<8)|recv[8];
	extern u16 PM2_5_value;
	extern u8 CO2Value[2];
	data[0]=0xff;
	data[1]=0xff;
	data[2]=collectId>>8;
	data[3]=collectId;
	data[4]=0x01|0x80;
	
	
	if (wantlenth>20)//������󳤶�
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
			//��������:�ɹ�
		velues[0]=0;		
		velues[1]=0;				
					//����λ
		velues[2]=0x01;//��������
		velues[3]=1;//�Ƿ��ڹ���
		//	velues[4]=RUN_TIME_S>>24;
		//	velues[5]=RUN_TIME_S>>16;
		//	velues[6]=RUN_TIME_S>>8;
		//	velues[7]=RUN_TIME_S;
		velues[8]=3;//����������
		velues[9]=0;//����������
		
		
		
		//��Ӵ��������ϸ���
		if ((TempAndHumi[0]==0&&TempAndHumi[1]==0)&&(TempAndHumi[2]==0)&&(TempAndHumi[3]==0))
		{
			velues[9]++;
		}
		if (TVOCValue[0]==0&&TVOCValue[1]==0)
		{
			velues[9]++;
		}
		if (PM2_5_value==0)
		{
			velues[9]++;
		}
		
		
		
		
		velues[10]=TempAndHumi[0];//�¶�����
		velues[11]=TempAndHumi[1];//�¶�С��
		velues[12]=TempAndHumi[2];//ʪ������
		velues[13]=TempAndHumi[3];//ʪ��С��
		velues[14]=TVOCValue[0];
		velues[15]=TVOCValue[1];
		velues[16]=PM2_5_value>>8;	//pm2.5��8λ
		velues[17]=PM2_5_value;			//pm2.5��8λ
		velues[18]=CO2Value[0];			//co2
		velues[19]=CO2Value[1];			//co2��8λ
		for(i=0;i<wantlenth;i++)
		{
			data[7+i]=velues[i];
		}
	}
	
	Get_Crc16(data,7+wantlenth,crc);
	data[7+wantlenth]=crc[0];
	data[7+wantlenth+1]=crc[1];
	RF1_tx_bytes(data,7+wantlenth+2);//���ͷ�������
	


	
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
	data[7]=err>>8;		//��������
	data[8]=err;
	
	Get_Crc16(data,9,crc);
	data[9]=crc[0];
	data[10]=crc[1];
	RF1_tx_bytes(data,data[6]+9);//���ͷ�������
	
}


