#include "systick.h"
#include "stm32f4xx.h"



void delay_ms(u32 time){//
    SysTick_Config(SystemCoreClock/1000);
	
		for(int i = 0; i < time; i++)
		{
			while( !((SysTick -> CTRL) & (1 << 16)) );
		}
		SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}



void delay_us(u32 time){//
    SysTick_Config(SystemCoreClock/1000000);
	
		for(int i = 0; i < time; i++)
		{
			while( !((SysTick -> CTRL) & (1 << 16)) );
		}
		SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}




/* use interrupt */
__IO uint32_t TimingDelay;//全局变量，用于在中断服务程序中计数

void SysTick_Init_FORms(void)
{
	SysTick_Config(SystemCoreClock/1000);//设置时基为1ms
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delay_ms_ByInt(uint32_t ms)
{
	SysTick_Init_FORms();
	TimingDelay = ms;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(TimingDelay != 0);
	
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

