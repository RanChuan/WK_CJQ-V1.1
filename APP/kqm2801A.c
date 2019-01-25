#include "kqm2801A.h"
#include "myiic.h"
#include "delay.h"
#include "sgp30.h"

u8 TvocValue[4]={0};   //���淵�ص��ĸ��ֽ�
u8 TVOCValue[2]={0};   //����TVOC����

//u8 a[4]={0};
float KQM2801_Value(char whatdo) //0x5F:TVOC
{
	float tvoc;
	u8 tmp1,tmp2;
	u16 ST;
	IIC_Init();
	delay_us(50);
	IIC_Start();  //������ʼ�ź�
	delay_us(50);
	IIC_Send_Byte(0x5F); //��������ַ   
	if(IIC_Wait_Ack()==0) //�ȴ�Ӧ���źŵ��� ����ֵ�� 1������Ӧ��ʧ�� 0�� ����Ӧ��ɹ�
	{
		delay_us(50);
	//IIC_Wait_Ack();
	TvocValue[0]= IIC_Read_Byte(1);    //��ȡ��λ����
	delay_us(100);
	TvocValue[1]= IIC_Read_Byte(1);   //��ȡ��λ����
	delay_us(100);
	TvocValue[2]=	IIC_Read_Byte(1);
	delay_us(100);
	TvocValue[3]= IIC_Read_Byte(1);
	delay_us(100);		
	IIC_Read_Byte(0);           
	delay_us(50);
	IIC_Stop();
	delay_us(50);
	if(TvocValue[1]==0xFF&&TvocValue[2]==0xFF)   //Ԥ��ʱ������ݸ�λ��λ����0xff
		return 0;
	
	ST = (TvocValue[1] << 8) | (TvocValue[2] << 0);
	
	if(whatdo==((char)0x5F))
	{
		tvoc = ST * 0.1;
		return (tvoc);
	} 
}
}
//SHT20 �����λ=========�ɲ���======================================
void SoftReset1(void) //������
{
	IIC_Init();
	IIC_Start();
	IIC_Send_Byte(0x80); //SHT20 ������ַ+write
	IIC_Send_Byte(0xfe);
	IIC_Stop();
} //���÷ֱ��ʣ� ���Բ��ã� Ĭ�Ͼ���
void Set_Resolution1(void)
{
	IIC_Start();
	IIC_Send_Byte(0x80); //����д����
	if(IIC_Wait_Ack()==0)
	{
		IIC_Send_Byte(0xE6); //д�û��Ĵ���
		if(IIC_Wait_Ack()==0)
		{
			if(IIC_Send_Byte(0x83),IIC_Wait_Ack()==0)
				; //11bit RH%; 11bit temp
		}
	}
}

/*********��ȡTVOC**********/

void Get_TVOC()
{
	float t=0;
	u16 ret=0;
	u16 tvoc=0;
	u16 co2=0;
	t=KQM2801_Value(0x5F);
	
	if(t==0)
	{
					//�ɰ�TVOC������û������|û�д�����
		ret=0;
		ret=sgp_measure_iaq_blocking_read(&tvoc,&co2);
		if (ret==0)
		{
			TVOCValue[0]=tvoc;
			TVOCValue[1]=0;
		}
	}
	else {
		TVOCValue[0]=(u8)t;            //TVOC����ֵ
		TVOCValue[1]=((u8)(t*10))%10;  //TVOCС��ֵ
	}


}

