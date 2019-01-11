#include "usart1.h"	
#include "delay.h"

u8 usart1_buf[256]={0};		//屏幕usart1数据缓存
u8 buf_len=0;						//屏幕usart1数据长度
u8 flag_rxusart1=0;
u8 IsRecived;
u8 ReviceFlat=0;
u8 MaxLenth=0;
u8 Nowcount=0;


void usart1_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* config USART2 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
		GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;   //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //数据长8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;   //1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No ;   //无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //失能硬件流
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //开启接收和发送模式
    USART_Init(USART1, &USART_InitStructure);  //初始化串口1
    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_ClearFlag(USART1,USART_FLAG_TC );
		
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  //打开串口1的全局中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //响应优先级是3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //失能不用串口中断，直接用串口发送模式
    NVIC_Init(&NVIC_InitStructure);
}


void usart1_sendData(unsigned char *dat,u8 len) 
{
	u8 i;
	//GPIO_SetBits(GPIOB,usart1_CONFIG);	//设置为发送模式
	for(i=0;i<len;i++)
	{
		USART_SendData(USART1,*(dat+i));
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);  //USART_FLAG_TXE发送数据寄存器标志位  
	}
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		//USART_FLAG_TC发送完成标志位
	buf_len=0;
	//GPIO_ResetBits(GPIOB,usart1_CONFIG);	//设置为接收模式
}


//串口屏用
void usart1_receiveData(u8 rxdat[],u8 *data_len)
{
	u8 i,rxlen_t;
	rxlen_t=buf_len;
	*data_len=0;
	delay_ms(10);
	
	if((buf_len==rxlen_t)&&buf_len)		//判断10MS内有无数据增长
	{
			//GPIO_ResetBits(GPIOB,GPIO_Pin_5);		
		flag_rxusart1=1;
		for(i=0;i<rxlen_t;i++)
		{
			rxdat[i]=usart1_buf[i];
		}
		*data_len=buf_len;
		buf_len=0;
	}
}

void USART1_IRQHandler(void)                	
{
		u8 k;
		USART_ClearFlag(USART1,USART_FLAG_TC );
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//检查指定的USART中断发生与否	
	{
	//	 USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		k=USART_ReceiveData(USART1);  //把接收回来的数据保存到变量中
		if(k==0xf0){	//有效数据的头  
			usart1_buf[0]=k;   //把接收回来的数据放到缓存区
			Nowcount=1; 
			return;
		}
		
		if(k==0xfe)
		{
			Nowcount=2;
			usart1_buf[1]=k;
			IsRecived=1;
			ReviceFlat=0;
			return ;
		}
		
		if(IsRecived){   //是否是有用的数据   0:无用  1：有用  如果是，就放到接收缓存区
			Nowcount++;   //接收回来的数据会放到缓存区，缓存区的大小
			usart1_buf[Nowcount-1]=k;
			
		if(Nowcount==4)    //判断buf里的数据是不是有效数据
				{
					if(usart1_buf[3]==0x03)
					{
							MaxLenth=10;
					}
					else if(usart1_buf[2]==0xff)
					{
							MaxLenth=10;
					}
				}
			if(Nowcount==MaxLenth){   //如果数据接收完
				IsRecived=0;   
				ReviceFlat=1;   //数据接收是否完成标志位   1：数据接收完
				Nowcount=0;
			}
		}

	}
}
