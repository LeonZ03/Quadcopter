#ifndef _KEY_H_
#define _KEY_H_

//#include "main.h"
//typedef unsigned short int   u16;


extern  unsigned short int scan_flag;

void KEY_Init(void);
void EXTIX_Init(void);
void TIM2_Init(u32 arr,u16 psc);

extern EXTI_InitTypeDef EXTI_InitStructure;
extern unsigned char key_flag;


#endif

