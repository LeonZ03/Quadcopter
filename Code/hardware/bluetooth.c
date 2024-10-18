#include "main.h"

void BlueTooth_Init()
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);
//开启USART外设时钟
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
//开启GPIO时钟
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);
//配置复用功能为USART1接口
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);
//配置复用功能为USART1接口
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  
//选择引脚为PB6与PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//无上拉下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate=115200;//选择波特率
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//字长为8bit
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//停止位长度为1
	USART_InitStruct.USART_Parity=USART_Parity_No;//无奇偶校验
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
//无硬件流控
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//| USART_Mode_Rx;
//接收与输出模式
	USART_Init(USART1,&USART_InitStruct);//初始化
	
	
NVIC_InitTypeDef NVIC_InitStructure;


USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断，接到数据
//USART_ITConfig(USART1, USART_IT_TC, ENABLE);//开启中断，发送完成


//Usart1 NVIC 配置
NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级 2
NVIC_InitStructure.NVIC_IRQChannelSubPriority =2; //响应优先级 2
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化 NVIC 寄存器、



	USART_Cmd(USART1,ENABLE);//上电使能
}



#define USART_REC_LEN 50
u16 USART_RX_STA=0;
uint8_t USART_RX_BUF[USART_REC_LEN];



void USART1_IRQHandler(void){
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_TC)==SET){
		USART_ClearFlag(USART1, USART_FLAG_TC);
	}
	
	
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET){//接收中断
		//ReceiveData_Handle();
		//USART_ClearFlag(USART1, USART_FLAG_RXNE);
		u8	Res =USART_ReceiveData(USART1);//(USART1->DR); //读取接收到的数据
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了 0x0d
			{
			if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
			else {
				USART_RX_STA|=0x8000; //接收完成了
				//USART_RX_STA=0;//重新接收

				}
			}
			else //还没收到 0X0D
			{
			if(Res==0x0d)USART_RX_STA|=0x4000;
			else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA > (USART_REC_LEN-1))USART_RX_STA=0;
					//接收数据错误,重新开始接收
				}
			}		
		}
	
	}


	return;
}

void BlueTooth_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);//发送字节数据
	while(USART_GetFlagStatus( USART1, USART_FLAG_TXE)==RESET);
//等待标志位清除          
}

uint32_t BlueTooth_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void BlueTooth_SendData(uint32_t Number)
{
	uint32_t Temp=Number;
	uint8_t Len=0;
	while(Temp!=0)
	{
		Temp/=10;
		Len++;
	}//该循环用于计算该数据有几位
	uint8_t i;
	for (i = 0; i < Len; i ++)
	{
	BlueTooth_SendByte(Number / BlueTooth_Pow(10, Len - i - 1) % 10 + '0'); 
	}
}

// 发送字符串到蓝牙
void BlueTooth_SendString(char* str) {
    while (*str) {
        USART_SendData(USART1, (uint16_t) *str);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        str++;
    }
}


