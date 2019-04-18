#ifndef __DEAL_H
#define __DEAL_H

#include "stdio.h"
#include "sys.h"



//模式2中的错误类型定义

#define ERR_SUCCESS         0x0000			//成功
#define ERR_FAIL						0x0001			//未知错误
#define ERR_DATAVALE				0x0002			//数据错误
#define ERR_DATALENGTH			0x0003			//数据长度错误
#define ERR_WANTLENGTH			0x0004			//想要读取的数据长度错误
#define ERR_NULLCMD					0x0005			//不支持的命令类型，
#define ERR_NOCONNECT				0x0006			//没有连接，一切操作都要在通信开始之后
#define ERR_NOCJADDR				0x0007			//采集器的地址不是本机绑定的采集器地址
#define ERR_CANNOTCFG				0x0008			//在不是可配置状态下收到了配置命令
#define ERR_NOTMYTYPE				0x0009			//所要控制的设备与本机不符
#define ERR_TIMEOUT					0x000a   //超时




extern u16 dataBuffer[20];
#define FLASH_SAVE_ADDR  (0x08000000+62*1024) 				//设置FLASH 保存地址(必须为偶数)
//#define FLASH_SAVE_ALLOCTIONARR  (0x08000000+62*1024) 				//设置FLASH 保存地址(必须为偶数)

#define SIZE sizeof(dataBuffer/2)	 			  	//数组长度

void cmd_0x01 (u8 *recv);
void cmd_0x04 (u8 *recv);
void cmd_0x06 (u8 *buff);
void cmd_0x07 (u8 *buff);
void cmd_0x09 (u8 *buff);
void deal(void);
void Return_NEW (u8 cmd,u16 err);


#endif
