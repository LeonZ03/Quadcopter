#ifndef  TASK_RECEIVER_H
#define  TASK_RECEIVER_H


#include "main.h"





//receiver»ŒŒÒ
#define RECEIVER_TASK_PRIO       			8 
#define RECEIVER_STK_SIZE  		    		64

extern OS_STK RECEIVER_TASK_STK[RECEIVER_STK_SIZE];

void receiver_task(void *pdata);



#endif /* TASK_RECEIVER_H */
