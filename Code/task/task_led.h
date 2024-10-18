#ifndef  TASK_LED_H
#define  TASK_LED_H


#include "main.h"



void APP_TASK_GREEN (void *p_arg);
void APP_TASK_RED   (void *p_arg);



//LEDÈÎÎñ
#define LED_TASK_PRIO       			7 
#define LED_STK_SIZE  		    		64

extern OS_STK LED_TASK_STK[LED_STK_SIZE];

void led_task(void *pdata);



#endif /* TASH_LED_H */
