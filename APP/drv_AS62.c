#include "drv_AS62.h"


//模块配置参数数组
//改变模块参数，只需改变参数数组值，然后初始化即可
uint8_t AS32_Param_Config[ 6 ] = { 0xC0, 0x04, 0xD2, 0x3A, 0x0A, 0xC0 };			//定点模式
uint8_t Cal=0;
uint8_t ADDRESS[8]					={0x5A,0xA5,0x05,0x82,0x00,0x20,0x00,0x00};//±¾»úµØÖ·

/**
  * @brief :ASxx模块初始化  摘要   模块只需要初始化一次
  * @param :无
  * @note  :按照默认参数初始化，修改默认参数表即可改变模块初始化参数
  * @retval:
  *        @ASxx_Write_OK 写入成功
  *        @ASxx_Write_ERROR 写入失败
  */
void AS32_Param_Init( uint8_t AS_P_Config[6] )//通过地址的奇偶选择相应的通道
{
//	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	M0=1;
	M1=1;
	Cal=ADDRESS[7]%2;
	if(Cal==1)
	{
		AS_P_Config[4]=0x01;
	}
	else
	{
		AS_P_Config[4]=0x02;
	}
	delay_ms(1000);
//	usart1_sendData((uint8_t *)AS_P_Config, 6 );
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	M1=0;
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
