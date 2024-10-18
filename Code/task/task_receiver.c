#include "main.h"

/***
 * 函数名称 :receiver_task(void *pdata);;
 *
 * 函数描述 : 接收机任务;
 *
 * 传递值	  : 无;
 *
 * 返回值   : 无;
 *
 **/


//创建任务堆栈空间
OS_STK RECEIVER_TASK_STK[RECEIVER_STK_SIZE];


void receiver_task(void *pdata){
   	while(1)
	{
		EventStartA(0);
 		OSTimeDly(20);//采样周期20ms,与ppm周期相同
		EventStopA(0);		 

//		OLED_ShowNum(1, 1, ch[0], 10);//统一放到显示任务中
//		OLED_ShowNum(2, 1, ch[1], 10);
//		OLED_ShowNum(3, 1, ch[2], 10);
//		OLED_ShowNum(4, 1, ch[3], 10);
		
		EventStartA(1);
		Motor_Run(ch[2]-500);
		EventStopA(1);
	}
}

