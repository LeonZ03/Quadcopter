#include "main.h"






void I2C_Configuration(void){ //I2C��ʼ��
//		//����GPIOBʱ��
//		RCC->AHB1ENR|=0x01<<1;
//		//����IICʱ��
//		RCC->APB1ENR|=0x01<<21;
//		//GPIO�ĳ�ʼ������
//		GPIOB->MODER&=~(0x02<<(8*2));
//		GPIOB->MODER|=0x02<<(8*2); //ѡ��PB8Ϊ���ù���ģʽ
//		GPIOB->MODER&=~(0x02<<(9*2));
//		GPIOB->MODER|=0x02<<(9*2);//ѡ��PB9Ϊ���ù���ģʽ
//	
//		GPIOB->OTYPER|=0x01<<8;
//		GPIOB->OTYPER|=0x01<<9;
//		
//		GPIOB->OSPEEDR&=~(0X03<<(8*2));
//		GPIOB->OSPEEDR|=0X03<<(8*2); //����PB8��GPIO�������Ϊ100Mhz
//	
//		GPIOB->OSPEEDR&=~(0X03<<(9*2));
//		GPIOB->OSPEEDR|=0X03<<(9*2);//����PB9��GPIO�������Ϊ100Mhz
//	
//		GPIOB->PUPDR&=~(0x0F<<(4*4));//����������
//		//���Ÿ��ù������ã�ѡ��IIC����
//		GPIOB->AFR[1]&=~(0xFF);
//		GPIOB->AFR[1]|=0x04;
//		GPIOB->AFR[1]|=(0x04<<(4));
//		
//		//IIC�ĳ�ʼ������
//		I2C1->CR1&=~(0x01<<1); //ѡ��IICģʽ����ģʽ
//		I2C1->CR2|=0x2A;// ����FREQ                                
////		I2C1->CCR&=~(0x01<<15);//����Ϊ��׼ģʽ����IIC����Ϊ100khz
//		I2C1->CCR|=(0x01<<15);//
//		//I2C1->OAR1|=0x0A; //�������豸��ַ�����������һ������
//		I2C1->CCR|=0xFFF;
//		I2C1->CCR&=0xD2; //����CCRʱ��Ƶ��                        
//		I2C1->TRISE=I2C1->CR2+1; //�����������ʱ��
//		I2C1->CR1|=0x01<<10; //����Ӧ��
//		I2C1->CR1|=0x01; //����IIC

	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	I2C_DeInit(I2C1);	
  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//�ܽŸ���
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
 
	//����IIC����
	I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
	//I2C_InitStructure.I2C_OwnAddress1         = SlaveAdd;  //���豸��ַ
	I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed          = 400000;  //SCL���400KHz
	
	I2C_Cmd(I2C1, ENABLE);
//  I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_ERR , ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_AcknowledgeConfig(I2C1, ENABLE); 

}

uint8_t MY_CHECK_EVENT(uint16_t SR1,uint16_t SR2)
{
		//�ȶ�ȡSR1��ֻ��ADDR����λ ����STOPF������ſ��Զ�ȡSR2��ֵ
		if( (I2C1->SR1&0x0002 ) || !(I2C1->SR1&0x0010) )
		{
//�Ƚϴ���ֵ��ʵ��ֵ
			if( ( (I2C1->SR1 & SR1)==SR1 ) && ( (I2C1->SR2 & SR2)==SR2 ) )
				return 1;
			else
				return 0;
		}
		else
		{
			return 0;
		}
}

void I2C_SEND_BYTE(uint8_t SlaveAddr,uint8_t WriteAddr,uint8_t Data){
//I2C����һ���ֽڣ��ӵ�ַ���ڲ���ַ�����ݣ�	
		//������ʼ�ź�
		I2C1->CR1|=0x01<<8;
		while(!MY_CHECK_EVENT(0x0001,0x0003));//�ȴ�EV5����
		//��Ϊ��д�����Բ��ø���ַ��һ
		I2C1->DR=SlaveAddr;
		while(!MY_CHECK_EVENT(0x0082,0x0007));//�ȴ�EV6
		I2C1->DR = WriteAddr;
		while(!MY_CHECK_EVENT(0x0084,0x0007));
//�ȴ�EV8����λ�Ĵ����ǿգ����ݼĴ����ѿ�
		I2C1->DR = Data;
		while(!MY_CHECK_EVENT(0x0084,0x0007));
//�ȴ�EV8����λ�Ĵ����ǿգ����ݼĴ����ѿ�
		//���������ź�
		I2C1->CR1|=0x01<<9;
}

uint8_t I2C_READ_BYTE(uint8_t SlaveAddr,uint8_t ReadAddr){ //I2C��ȡһ���ֽ�
	
	//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���

		
		uint8_t data;
		//������ʼ�ź�
	
		
	
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		I2C1->CR1|=0x01<<8;
		while(!MY_CHECK_EVENT(0x0001,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
	
	
	
	
	
	
		
	
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		I2C1->DR=SlaveAddr;
		while(!MY_CHECK_EVENT(0x0082,0x0007));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		I2C1->DR= ReadAddr;
		while(!MY_CHECK_EVENT(0x0084,0x0007));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		//���²�����ʼ�ź�
		I2C1->CR1|=0x01<<8;
		while(!MY_CHECK_EVENT(0x0001,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		//������,���һλҪΪ1    
//		I2C1->DR=SlaveAddr+1;
			I2C1->DR=SlaveAddr|0x01;
			
		while(!MY_CHECK_EVENT(0x0002,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		//���������ź��Լ��ر�Ӧ��
		//I2C1->CR1|=0x0001<<9;
		I2C1->CR1 &= ~((uint16_t)0x0001<<9);
		I2C1->CR1|=0x0001<<10;
		//I2C1->CR1&=0xFBFF;
		while(!MY_CHECK_EVENT(0x0040,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
	

		data=I2C1->DR;
		//����Ӧ��
		//I2C1->CR1|=0x0001<<10;
		
		//I2C1->CR1&=0xFBFF;
		I2C1->CR1 &= ~((uint16_t)0x0001<<10);
		I2C1->CR1 |= (uint16_t)0x0001<<9;
		
//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���

		return data;
}

uint8_t I2C_READ_BYTE_LEN(uint8_t SlaveAddr,uint8_t ReadAddr,uint8_t* buf,uint8_t len){
	
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		I2C1->CR1|=0x01<<8;
		while(!MY_CHECK_EVENT(0x0001,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
	
	
	
	
	
	
		
	
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		I2C1->DR=SlaveAddr;
		while(!MY_CHECK_EVENT(0x0082,0x0007));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		I2C1->DR= ReadAddr;
		while(!MY_CHECK_EVENT(0x0084,0x0007));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		//���²�����ʼ�ź�
		I2C1->CR1|=0x01<<8;
		while(!MY_CHECK_EVENT(0x0001,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		//������,���һλҪΪ1    
//		I2C1->DR=SlaveAddr+1;
			I2C1->DR=SlaveAddr|0x01;
			
		while(!MY_CHECK_EVENT(0x0002,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
		//���������ź��Լ��ر�Ӧ��
		//I2C1->CR1|=0x0001<<9;
		I2C1->CR1 &= ~((uint16_t)0x0001<<9);
		I2C1->CR1|=0x0001<<10;
		//I2C1->CR1&=0xFBFF;
		
		for(int i=0;i<len;i++){
		while(!MY_CHECK_EVENT(0x0040,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
		buf[i]=I2C1->DR;
		}
		//����Ӧ��
		//I2C1->CR1|=0x0001<<10;
		
		//I2C1->CR1&=0xFBFF;
		I2C1->CR1 &= ~((uint16_t)0x0001<<10);
		I2C1->CR1 |= (uint16_t)0x0001<<9;
}

