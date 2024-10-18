#ifndef  TASK_SCAN_H
#define  TASK_SCAN_H


#include "main.h"





//scan»ŒŒÒ
#define SCAN_TASK_PRIO       			10 
#define SCAN_STK_SIZE  		    		64

extern OS_STK SCAN_TASK_STK[SCAN_STK_SIZE];
//extern u8 scan_flag;

void scan_task(void *pdata);



#endif /* TASK_SCAN_H */
