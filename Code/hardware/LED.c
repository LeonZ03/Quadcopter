#include "stm32f4xx.h"

void led_init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	

	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;


	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

//void led_up(void){
//	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
//}

//void led_down(void){
//	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
//}



void my_delay_ms(int time)
{    
   u32 i=0;  
   while(time--)   {
      i=11996;  //自己定义6246
      while(i--) ;    
   }
}

