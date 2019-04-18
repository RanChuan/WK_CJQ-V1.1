#include "sht21.h"
#include "myiic.h"
#include "delay.h"
#include "data.h"

u8 TempAndHumi[8]={0};
float SHT2X_MeasureNHM(char whatdo) //0xF3:温度测量， 0xF5:湿度测量， 都是非主机
{
	float Humidity,Temperature;
	u8 tmp1,tmp2;
	u16 ST;
	IIC_Init();
	delay_us(50);
	IIC_Start();
	IIC_Send_Byte(0x80); //·器件地址   
	if(IIC_Wait_Ack()==0)
	{
		IIC_Send_Byte(whatdo);
		if(IIC_Wait_Ack()==0)
		{
			do
			{
			delay_us(50);
			IIC_Start();
			}while(IIC_Send_Byte(0x81),IIC_Wait_Ack()==1); //中间是逗号运算符    输出结果是最后一个运算的结果
		}
	}//此处必须等待应答， 不可用延时代替， 否则无法读出， 读数 128 都是数据错误
	delay_us(50);
	tmp1= IIC_Read_Byte(0);
	delay_us(50);
	tmp2= IIC_Read_Byte(0);
	IIC_Read_Byte(0);
	IIC_Stop();
	delay_us(50);
	if(tmp1==0xFF&&tmp2==0xFF)
		return 0;
	ST = (tmp1 << 8) | (tmp2 << 0);
	ST &= ~0x0003;	
	if(whatdo==((char)0xF3))
	{
		Temperature = ((float)ST * 0.00268127) - 46.85;
		return (Temperature);
	} 
	else //这里默认另一种就是非主机湿度测量， 如需要主机测量模式请自己添加
	{
		Humidity = ((float)ST * 0.00190735) - 6;
		return (Humidity);
	}
}
//SHT20 软件复位=========可不用======================================
void SoftReset(void) //软重启
{
	IIC_Init();
	IIC_Start();
	IIC_Send_Byte(0x80); //SHT20 器件地址+write
	IIC_Send_Byte(0xfe);
	IIC_Stop();
} //设置分辨率， 可以不用， 默认就行
void Set_Resolution(void)
{
	IIC_Start();
	IIC_Send_Byte(0x80); //发送写命令
	if(IIC_Wait_Ack()==0)
	{
		IIC_Send_Byte(0xE6); //写用户寄存器
		if(IIC_Wait_Ack()==0)
		{
			if(IIC_Send_Byte(0x83),IIC_Wait_Ack()==0)
				; //11bit RH%; 11bit temp
		}
	}
}




#include "data_map.h"
/*********获取温湿度
  * 高两个字节放温度
  * 低两个字节放湿度
  */
void Get_TempAmdHumi()
{
	f_to_u t={0};
	
	t.f=SHT2X_MeasureNHM(0xF3);		//上传浮点数2019.3.30
	TempAndHumi[0]=t.u[0];
	TempAndHumi[1]=t.u[1];
	TempAndHumi[2]=t.u[2];
	TempAndHumi[3]=t.u[3];
	DATA_MAP->temperature=t.f;
	
	t.f=SHT2X_MeasureNHM(0xF5);
	TempAndHumi[4]=t.u[0];
	TempAndHumi[5]=t.u[1];
	TempAndHumi[6]=t.u[2];
	TempAndHumi[7]=t.u[3];
	DATA_MAP->humidity=t.f;
}

