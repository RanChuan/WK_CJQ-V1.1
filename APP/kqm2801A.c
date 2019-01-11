#include "kqm2801A.h"
#include "myiic.h"
#include "delay.h"

u8 TvocValue[4]={0};   //保存返回的四个字节
u8 TVOCValue[2]={0};   //保存TVOC数据

//u8 a[4]={0};
float KQM2801_Value(char whatdo) //0x5F:TVOC
{
	float tvoc;
	u8 tmp1,tmp2;
	u16 ST;
	IIC_Init();
	delay_us(50);
	IIC_Start();  //产生起始信号
	delay_us(50);
	IIC_Send_Byte(0x5F); //·器件地址   
	if(IIC_Wait_Ack()==0) //等待应答信号到来 返回值： 1，接收应答失败 0， 接收应答成功
	{
		delay_us(50);
	//IIC_Wait_Ack();
	TvocValue[0]= IIC_Read_Byte(1);    //读取高位数据
	delay_us(100);
	TvocValue[1]= IIC_Read_Byte(1);   //读取低位数据
	delay_us(100);
	TvocValue[2]=	IIC_Read_Byte(1);
	delay_us(100);
	TvocValue[3]= IIC_Read_Byte(1);
	delay_us(100);		
	IIC_Read_Byte(0);           
	delay_us(50);
	IIC_Stop();
	delay_us(50);
	if(TvocValue[1]==0xFF&&TvocValue[2]==0xFF)   //预热时候的数据高位低位都是0xff
		return 0;
	
	ST = (TvocValue[1] << 8) | (TvocValue[2] << 0);
	
	if(whatdo==((char)0x5F))
	{
		tvoc = ST * 0.1;
		return (tvoc);
	} 
}
}
//SHT20 软件复位=========可不用======================================
void SoftReset1(void) //软重启
{
	IIC_Init();
	IIC_Start();
	IIC_Send_Byte(0x80); //SHT20 器件地址+write
	IIC_Send_Byte(0xfe);
	IIC_Stop();
} //设置分辨率， 可以不用， 默认就行
void Set_Resolution1(void)
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

/*********获取TVOC**********/

void Get_TVOC()
{
	float t=0;
	
	t=KQM2801_Value(0x5F);
	
	if(t==0)
	{
		
	}
	else {
		TVOCValue[0]=(u8)t;            //TVOC整数值
		TVOCValue[1]=((u8)(t*10))%10;  //TVOC小数值
	}


}

