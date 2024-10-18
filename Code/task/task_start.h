#ifndef  TASK_START_H
#define  TASK_START_H

#include "main.h"

void APP_TASK_START(void *p_arg);


#define START_TASK_PRIO      			0 //开始任务的优先级设置为最高
#define START_STK_SIZE  				64

extern OS_STK START_TASK_STK[START_STK_SIZE];

void start_task(void *pdata);	


#endif /* TASK_SATRT_H */
