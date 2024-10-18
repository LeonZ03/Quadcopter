#include "main.h"


void I2C_Config(){

		GPIO_InitTypeDef GPIO_InitStructure;
 
  // 配置I2C引脚的GPIO和复用功能
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  // 打开GPIOB的时钟
  RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	
	
}