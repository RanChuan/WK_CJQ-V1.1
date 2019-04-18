
/***************************************************

		数据的内存分配
		单片机的前1KB内存用作公用内存，主机可以通过内存存取指令来修改和读取
		每个cpu字长作为单位存储


**************************************************/

#ifndef DATA_MAP_H__
#define DATA_MAP_H__


#define DATA_MAP ((data_map*)0x20000000)



typedef struct
	
{
	char devType[4];			//设备类型
	char softVersion[4];		//软件版本
	char extType[4];			//扩展类型
	u32 devAddr;
	u32 sysRunTime;
	float temperature;
	float humidity;
	float tvoc;
	float co2;
	float pm2_5;
}data_map;



void data_map_init(void);





#endif





