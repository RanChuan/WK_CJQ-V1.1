#include "usart3.h"	
#include "delay.h"
#include "data.h"


u8 PM2_5_value[4]={0};
//u8 TVOC_value[4];
unsigned char  usart3_rcv_buf[MAX_RCV_LEN];
volatile unsigned int   usart3_rcv_len = 0;
u8 buf_len3=0;						//屏幕usart1数据长度
u8 flag_rxusart3=0;
u8 usart3_buf[256]={0};		//屏幕usart1数据缓存
//u8 IsRecived3;
//u8 ReviceFlat3=0;
u8 MaxLenth3=0;
u8 Nowcount3=0;

u8 USART3_IDE=1;//串口空闲

void USART3_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* config USART2 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
		USART_ClearFlag(USART3,USART_FLAG_TC);	 //清除USARTx的待处理标志位


    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}




void usart2_sendData(unsigned char *dat,u8 len) 
{
	u8 i;
	//GPIO_SetBits(GPIOB,usart1_CONFIG);	//设置为发送模式
	for(i=0;i<len;i++)
	{
		USART_SendData(USART2,*(dat+i));
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
	buf_len3=0;
	//GPIO_ResetBits(GPIOB,usart1_CONFIG);	//设置为接收模式
}


	
void usart2_receiveData(u8 rxdat[],u8 *data_len)
{
	u8 i,rxlen_t;
	rxlen_t=buf_len3;
	*data_len=0;
	delay_ms(10);
	
	if((buf_len3==rxlen_t)&&buf_len3)		//判断10MS内有无数据增长
	{
			//GPIO_ResetBits(GPIOB,GPIO_Pin_5);		
		flag_rxusart3=1;
		for(i=0;i<rxlen_t;i++)
		{
			rxdat[i]=usart3_buf[i];
		}
		*data_len=buf_len3;
		buf_len3=0;
	}
}

void USART3_IRQHandler(void)                	
{
	int i ;
	u8 k;
	USART_ClearFlag(USART3,USART_FLAG_TC);
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//检查指定的USART中断发生与否	
	{
		USART3_IDE=0;
		k=USART_ReceiveData(USART3);   
		Nowcount3++;   
		usart3_rcv_buf[Nowcount3-1]=k;
	}
	if (USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET)
	{
		USART3_IDE=1;
		k=USART3->SR;
		k=USART3->DR;
	}
}	

#include "data_map.h"

//查询得到数据
void get_data(void)
{
	u8 b[5]={0x11,0x02,0x0B,0x01,0xE1};
	u8 r[]={0};
	static u8 err_time=0;
	int i;
	u8 len;
	f_to_u t={0};
	if(Nowcount3>=100)
	{
		if (USART3_IDE)
		{
			for(i=0;i<MAX_RCV_LEN;i++)
			{
				if(usart3_rcv_buf[i]==0x42)
				{
					if(usart3_rcv_buf[i+1]==0x4d)
					{
						if(usart3_rcv_buf[i+3]==0x1C)
						{
							PM2_5_value[0]=0;
							PM2_5_value[1]=0;
							PM2_5_value[2]=0;
							PM2_5_value[3]=0;
							t.f=((usart3_rcv_buf[i+6]<<8)|usart3_rcv_buf[i+7])/1000.0;
							//PM2_5_value|=usart3_rcv_buf[i+6]<<8;
							//PM2_5_value|=usart3_rcv_buf[i+7];//PM2.5浓度，单位ug/m3
														//转换为浮点数2019.3.30
							PM2_5_value	[0]=t.u[0];
							PM2_5_value	[1]=t.u[1];
							PM2_5_value	[2]=t.u[2];
							PM2_5_value	[3]=t.u[3];
							DATA_MAP->pm2_5=t.f;
							break;
						}
					}
				}
			}
			Nowcount3=0;
			err_time=0;
		}
	}
	else
	{
		err_time++;
		if (err_time>=5)
		{
			err_time--;
			PM2_5_value[0]=0;
			PM2_5_value[1]=0;
			PM2_5_value[2]=0;
			PM2_5_value[3]=0;
			DATA_MAP->pm2_5=0;
		}
	}
}



float PM_Ture()//PM2.5数值函数
{
	int k1,k2;
	u16 PM;
	float pm;
//	PM_value[0]=usart3_rcv_buf[6];
//	PM_value[1]=usart3_rcv_buf[7];
//	k1 = PM_value[0];
//	k2 = PM_value[1];

	PM=(k1<<8)|(k2<<0);
	
	pm=Myitoa_16_to_10(PM);
	
	return pm;

}


void something(void)
{
	float pm;
	pm= PM_Ture();
}

int	Myitoa_16_to_10(unsigned char num)  
{  
		char P_M[10];
		int m;
		sprintf(P_M,"%02x",num);
		sscanf(P_M,"%x",&m );  
		return m;	
}
