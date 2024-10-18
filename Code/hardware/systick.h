#ifndef SYSTICK_H
#define SYSTICK_H
#include "stm32f4xx.h"


void delay_ms(u32 time);
void delay_us(u32 time);
void SysTick_Init_FORms(void);
void Delay_ms_ByInt(uint32_t ms);

#endif

