#include "myiic.h"
#include "delay.h" //延时函数可以参照自己芯片编写， 这个是 STM32F103 系列编写的//初始化ＩＩＣ
void IIC_Init(void)
{
RCC->APB2ENR|=1<<3; //使能 PORTB 时钟
GPIOB->CRL&=0X00FFFFFF; //PB6/7 推挽输出
GPIOB->CRL|=0X33000000;
GPIOB->ODR|=3<<6; //PB6,7 输出高
} //产生 IIC 起始信号
void IIC_Start(void)
{
SDA_OUT(); //sda 输出
IIC_SDA=1;
IIC_SCL=1;
delay_us(10);
IIC_SDA=0;//START:when CLK is high,DATA change form high to low
delay_us(10);
IIC_SCL=0;//锁住 IIC 总线， 准备发送或接收数据
} //产生 IIC 停止信号
void IIC_Stop(void)
{
SDA_OUT();//sda 输出
IIC_SCL=0;
IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
delay_us(10);
IIC_SCL=1;
IIC_SDA=1;//发送 IIC 总线结束信号
delay_us(10);
} //等待应答信号到来
//返回值： 1， 接收应答失败
// 0， 接收应答成功
u8 IIC_Wait_Ack(void)
{
u8 ucErrTime=0;
SDA_IN(); //SDA 设置为输入
IIC_SDA=1;delay_us(10);
IIC_SCL=1;delay_us(10);
while(READ_SDA)
{
ucErrTime++;
if(ucErrTime>250){
IIC_Stop();
return 1;
}
}
IIC_SCL=0;//时钟输出为 0
return 0;
} //产生 ACK 应答
void IIC_Ack(void)
{
IIC_SCL=0;
SDA_OUT();
IIC_SDA=0;
delay_us(10);
IIC_SCL=1;
delay_us(10);
IIC_SCL=0;
} //不产生 ACK 应答
void IIC_NAck(void)
{
IIC_SCL=0;
SDA_OUT();
IIC_SDA=1;
delay_us(10);
IIC_SCL=1;
delay_us(10);
IIC_SCL=0;
} //IIC 发送一个字节
//返回从机有无应答
void IIC_Send_Byte(u8 txd)
{
u8 t;
SDA_OUT();
IIC_SCL=0;//拉低时钟开始数据传输
for(t=0;t<8;t++)
{
IIC_SDA=(txd&0x80)>>7;
txd<<=1;
delay_us(10);
IIC_SCL=1;delay_us(10);
IIC_SCL=0;
delay_us(10);
}
} //读一个字节， ack=1 时， 发送 ACK， ack=0 时， 发送 nACK；
u8 IIC_Read_Byte(unsigned char ack)
{
unsigned char i,receive=0;
SDA_IN();//SDA 设置为输入
for(i=0;i<8;i++ )
{
IIC_SCL=0;
delay_us(10);
IIC_SCL=1;
receive<<=1;
if(READ_SDA)
	receive++;
delay_us(10);
} if
(!ack)
IIC_NAck();//发送 nACK
else
IIC_Ack(); //发送 ACK
return receive;
} // 检查 IIC 器件是否响应成功
u8 IIC_CheckDevice(u8 adress)
{
u8 CdAck;
IIC_Init();
IIC_Start();
IIC_Send_Byte(adress);
CdAck=IIC_Wait_Ack();
return CdAck;
}
