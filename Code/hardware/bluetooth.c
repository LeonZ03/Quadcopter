#include "main.h"

void BlueTooth_Init()
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);
//����USART����ʱ��
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
//����GPIOʱ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);
//���ø��ù���ΪUSART1�ӿ�
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);
//���ø��ù���ΪUSART1�ӿ�
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  
//ѡ������ΪPB6��PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate=115200;//ѡ������
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//�ֳ�Ϊ8bit
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//ֹͣλ����Ϊ1
	USART_InitStruct.USART_Parity=USART_Parity_No;//����żУ��
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
//��Ӳ������
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//| USART_Mode_Rx;
//���������ģʽ
	USART_Init(USART1,&USART_InitStruct);//��ʼ��
	
	
NVIC_InitTypeDef NVIC_InitStructure;


USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����жϣ��ӵ�����
//USART_ITConfig(USART1, USART_IT_TC, ENABLE);//�����жϣ��������


//Usart1 NVIC ����
NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ� 2
NVIC_InitStructure.NVIC_IRQChannelSubPriority =2; //��Ӧ���ȼ� 2
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ��ʹ��
NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ�� NVIC �Ĵ�����



	USART_Cmd(USART1,ENABLE);//�ϵ�ʹ��
}



#define USART_REC_LEN 50
u16 USART_RX_STA=0;
uint8_t USART_RX_BUF[USART_REC_LEN];



void USART1_IRQHandler(void){
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_TC)==SET){
		USART_ClearFlag(USART1, USART_FLAG_TC);
	}
	
	
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET){//�����ж�
		//ReceiveData_Handle();
		//USART_ClearFlag(USART1, USART_FLAG_RXNE);
		u8	Res =USART_ReceiveData(USART1);//(USART1->DR); //��ȡ���յ�������
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ��� 0x0d
			{
			if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
			else {
				USART_RX_STA|=0x8000; //���������
				//USART_RX_STA=0;//���½���

				}
			}
			else //��û�յ� 0X0D
			{
			if(Res==0x0d)USART_RX_STA|=0x4000;
			else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA > (USART_REC_LEN-1))USART_RX_STA=0;
					//�������ݴ���,���¿�ʼ����
				}
			}		
		}
	
	}


	return;
}

void BlueTooth_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);//�����ֽ�����
	while(USART_GetFlagStatus( USART1, USART_FLAG_TXE)==RESET);
//�ȴ���־λ���          
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
	}//��ѭ�����ڼ���������м�λ
	uint8_t i;
	for (i = 0; i < Len; i ++)
	{
	BlueTooth_SendByte(Number / BlueTooth_Pow(10, Len - i - 1) % 10 + '0'); 
	}
}

// �����ַ���������
void BlueTooth_SendString(char* str) {
    while (*str) {
        USART_SendData(USART1, (uint16_t) *str);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        str++;
    }
}


