#ifndef _COLLECT_H
#define _COLLECT_H
#include "stm32f10x.h"

#define COLLECTSIZE 128

extern u16 collectId; //采集器编号

//typedef struct 
//{
//	u8 numId;
//	u8 id1;  //集中器
//	u8 id2;  //采集器编号
//	u8 id3;  //控制器
//	u8 tempNumH;  //温度整数位
//	u8 tempNumL;  //温度小数位
//	u8 humiNumH;   //湿度整数
//	u8 humiNumL;
//	u8 tvocH;     //tvoc整数
//	u8 tvocL;
//	u8 pm2_5_bai;  //pm2.5百位
//	u8 pm2_5_h;    //pm2.5十位和个位
//	u8 pm2_5_l;    //pm2.5小数
//	
//}CollectData;

void collectdeal(void);

void returnEnvironmentData(u8 address);

void addressIdinit(void);

void Nothing(void);
//获取TVOC
void get_tvoc(void);


#endif
