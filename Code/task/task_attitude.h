#ifndef  TASK_ATTITUDE_H
#define  TASK_ATTITUDE_H


#include "main.h"





//attitude»ŒŒÒ
#define ATTITUDE_TASK_PRIO       			9 
#define ATTITUDE_STK_SIZE  		    		64

extern OS_STK ATTITUDE_TASK_STK[ATTITUDE_STK_SIZE];
extern int16_t mpu6050[7];
extern int16_t hmc5883l[3];

void attitude_task(void *pdata);



#endif /* TASK_ATTITUDE_H */

