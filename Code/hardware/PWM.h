#ifndef _PWM_H
#define _PWM_H

#include "main.h"

extern u32 ch[];
extern u8 ch_flag;
extern u8 ElcContrller_last, ElcContrller_cur;
extern u8 cap_1_ch;//相当于互斥锁，确保捕获一路ch的过程不被关中断打断


#define PWM_SetCCR_1(ccr) (TIM_SetCompare1(TIM3,ccr))
#define PWM_SetCCR_2(ccr) (TIM_SetCompare2(TIM3,ccr))
#define PWM_SetCCR_3(ccr) (TIM_SetCompare3(TIM3,ccr))
#define PWM_SetCCR_4(ccr) (TIM_SetCompare4(TIM3,ccr))


void OC_PWM_Init(u16 arr,u16 psc);
//void PWM_SetCCR(uint16_t CCR);
void IC_PWM_Init(void);
void TIM1_CH1_Cap_Init(u16 arr,u16 psc);
void PPM_Handle(void);






#endif

