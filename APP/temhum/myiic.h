#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//IO 方向设置
#define SDA_IN() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}
//IO 操作函数
#define IIC_SCL PBout(6) //SCL
#define IIC_SDA PBout(7) //SDA
#define READ_SDA PBin(7) //SDA
//IIC 所有操作函数
void IIC_Init(void); //初始化 IIC 的 IO 口
void IIC_Start(void); //发送 IIC 开始信号
void IIC_Stop(void); //发送 IIC 停止信号
void IIC_Send_Byte(u8 txd); //IIC 发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC 读取一个字节
u8 IIC_Wait_Ack(void); //IIC 等待 ACK 信号
void IIC_Ack(void); //IIC 发送 ACK 信号
void IIC_NAck(void); //IIC 不发送 ACK 信号
u8 IIC_CheckDevice(u8 adress); //判断设备是否存在， 响应成功
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);
#endif

