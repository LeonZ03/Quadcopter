#include "main.h"


void I2C_Config(){

		GPIO_InitTypeDef GPIO_InitStructure;
 
  // ����I2C���ŵ�GPIO�͸��ù���
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  // ��GPIOB��ʱ��
  RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	
	
}