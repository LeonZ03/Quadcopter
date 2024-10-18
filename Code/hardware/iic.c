#include "main.h"






void I2C_Configuration(void){ //I2C初始化
//		//开启GPIOB时钟
//		RCC->AHB1ENR|=0x01<<1;
//		//开启IIC时钟
//		RCC->APB1ENR|=0x01<<21;
//		//GPIO的初始化配置
//		GPIOB->MODER&=~(0x02<<(8*2));
//		GPIOB->MODER|=0x02<<(8*2); //选择PB8为复用功能模式
//		GPIOB->MODER&=~(0x02<<(9*2));
//		GPIOB->MODER|=0x02<<(9*2);//选择PB9为复用功能模式
//	
//		GPIOB->OTYPER|=0x01<<8;
//		GPIOB->OTYPER|=0x01<<9;
//		
//		GPIOB->OSPEEDR&=~(0X03<<(8*2));
//		GPIOB->OSPEEDR|=0X03<<(8*2); //设置PB8的GPIO输出速率为100Mhz
//	
//		GPIOB->OSPEEDR&=~(0X03<<(9*2));
//		GPIOB->OSPEEDR|=0X03<<(9*2);//设置PB9的GPIO输出速率为100Mhz
//	
//		GPIOB->PUPDR&=~(0x0F<<(4*4));//无上拉下拉
//		//引脚复用功能设置，选择IIC外设
//		GPIOB->AFR[1]&=~(0xFF);
//		GPIOB->AFR[1]|=0x04;
//		GPIOB->AFR[1]|=(0x04<<(4));
//		
//		//IIC的初始化配置
//		I2C1->CR1&=~(0x01<<1); //选择IIC模式：主模式
//		I2C1->CR2|=0x2A;// 配置FREQ                                
////		I2C1->CCR&=~(0x01<<15);//配置为标准模式，即IIC速率为100khz
//		I2C1->CCR|=(0x01<<15);//
//		//I2C1->OAR1|=0x0A; //配置主设备地址，与从器件不一样即可
//		I2C1->CCR|=0xFFF;
//		I2C1->CCR&=0xD2; //配置CCR时钟频率                        
//		I2C1->TRISE=I2C1->CR2+1; //配置最大上升时间
//		I2C1->CR1|=0x01<<10; //开启应答
//		I2C1->CR1|=0x01; //开启IIC

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
	
	//管脚复用
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
 
	//配置IIC参数
	I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
	//I2C_InitStructure.I2C_OwnAddress1         = SlaveAdd;  //从设备地址
	I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed          = 400000;  //SCL最大400KHz
	
	I2C_Cmd(I2C1, ENABLE);
//  I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_ERR , ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_AcknowledgeConfig(I2C1, ENABLE); 

}

uint8_t MY_CHECK_EVENT(uint16_t SR1,uint16_t SR2)
{
		//先读取SR1，只有ADDR被置位 或者STOPF被清除才可以读取SR2的值
		if( (I2C1->SR1&0x0002 ) || !(I2C1->SR1&0x0010) )
		{
//比较传入值与实际值
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
//I2C发送一个字节（从地址，内部地址，内容）	
		//产生开始信号
		I2C1->CR1|=0x01<<8;
		while(!MY_CHECK_EVENT(0x0001,0x0003));//等待EV5结束
		//因为是写，所以不用给地址加一
		I2C1->DR=SlaveAddr;
		while(!MY_CHECK_EVENT(0x0082,0x0007));//等待EV6
		I2C1->DR = WriteAddr;
		while(!MY_CHECK_EVENT(0x0084,0x0007));
//等待EV8：移位寄存器非空，数据寄存器已空
		I2C1->DR = Data;
		while(!MY_CHECK_EVENT(0x0084,0x0007));
//等待EV8：移位寄存器非空，数据寄存器已空
		//产生结束信号
		I2C1->CR1|=0x01<<9;
}

uint8_t I2C_READ_BYTE(uint8_t SlaveAddr,uint8_t ReadAddr){ //I2C读取一个字节
	
	//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁

		
		uint8_t data;
		//产生开始信号
	
		
	
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		I2C1->CR1|=0x01<<8;
		while(!MY_CHECK_EVENT(0x0001,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
	
	
	
	
	
	
		
	
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		I2C1->DR=SlaveAddr;
		while(!MY_CHECK_EVENT(0x0082,0x0007));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		I2C1->DR= ReadAddr;
		while(!MY_CHECK_EVENT(0x0084,0x0007));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		//重新产生起始信号
		I2C1->CR1|=0x01<<8;
		while(!MY_CHECK_EVENT(0x0001,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		//读数据,最后一位要为1    
//		I2C1->DR=SlaveAddr+1;
			I2C1->DR=SlaveAddr|0x01;
			
		while(!MY_CHECK_EVENT(0x0002,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		//产生结束信号以及关闭应答
		//I2C1->CR1|=0x0001<<9;
		I2C1->CR1 &= ~((uint16_t)0x0001<<9);
		I2C1->CR1|=0x0001<<10;
		//I2C1->CR1&=0xFBFF;
		while(!MY_CHECK_EVENT(0x0040,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
	

		data=I2C1->DR;
		//开启应答
		//I2C1->CR1|=0x0001<<10;
		
		//I2C1->CR1&=0xFBFF;
		I2C1->CR1 &= ~((uint16_t)0x0001<<10);
		I2C1->CR1 |= (uint16_t)0x0001<<9;
		
//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁

		return data;
}

uint8_t I2C_READ_BYTE_LEN(uint8_t SlaveAddr,uint8_t ReadAddr,uint8_t* buf,uint8_t len){
	
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		I2C1->CR1|=0x01<<8;
		while(!MY_CHECK_EVENT(0x0001,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
	
	
	
	
	
	
		
	
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		I2C1->DR=SlaveAddr;
		while(!MY_CHECK_EVENT(0x0082,0x0007));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		I2C1->DR= ReadAddr;
		while(!MY_CHECK_EVENT(0x0084,0x0007));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		//重新产生起始信号
		I2C1->CR1|=0x01<<8;
		while(!MY_CHECK_EVENT(0x0001,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		//读数据,最后一位要为1    
//		I2C1->DR=SlaveAddr+1;
			I2C1->DR=SlaveAddr|0x01;
			
		while(!MY_CHECK_EVENT(0x0002,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
		
		
		
		
		
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
		//产生结束信号以及关闭应答
		//I2C1->CR1|=0x0001<<9;
		I2C1->CR1 &= ~((uint16_t)0x0001<<9);
		I2C1->CR1|=0x0001<<10;
		//I2C1->CR1&=0xFBFF;
		
		for(int i=0;i<len;i++){
		while(!MY_CHECK_EVENT(0x0040,0x0003));
		//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
		buf[i]=I2C1->DR;
		}
		//开启应答
		//I2C1->CR1|=0x0001<<10;
		
		//I2C1->CR1&=0xFBFF;
		I2C1->CR1 &= ~((uint16_t)0x0001<<10);
		I2C1->CR1 |= (uint16_t)0x0001<<9;
}

