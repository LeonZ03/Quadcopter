/*
*********************************************************************************************************
* Filename      : task_start.c
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
//#include "app_cfg.h"
//#include "sys_cfg.h"
//#include "ucos_ii.h"
//#include "stm32f4xx.h"
//#include "task_led.h"
//#include "task_start.h"
#include "main.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define  APP_TASK_USER_GREEN_PRIO								 6u
#define  APP_TASK_USER_RED_PRIO									 8u
#define  APP_TASK_USER_STK_SIZE									128u

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static OS_STK   App_TaskGreenStk[APP_TASK_USER_STK_SIZE];
static OS_STK   App_TaskRedStk[APP_TASK_USER_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/***
 * 函数名称 : APP_TASK_START();
 *
 * 函数描述 : ucos_ii 初始化任务;
 *
 * 传递值	  : 无;
 *
 * 返回值   : 无;
 *
 **/
void APP_TASK_START(void *p_arg)
{
		OS_CPU_SR cpu_sr;
    (void)p_arg;
		
		/* 配置滴答定时器，此函数调用位置为第一个创建任务开头 */
    OS_CPU_SysTickInit();
									
		/* 关中断 */
		OS_ENTER_CRITICAL();
				
		/* 创建绿灯任务 */
    OSTaskCreateExt(APP_TASK_GREEN,
                   (void    *)0,
                   (OS_STK *)&App_TaskGreenStk[APP_TASK_USER_STK_SIZE - 1],
                   (INT8U   )APP_TASK_USER_GREEN_PRIO,
                   (INT16U  )APP_TASK_USER_GREEN_PRIO,
                   (OS_STK *)&App_TaskGreenStk[0],
                   (INT32U  )APP_TASK_USER_STK_SIZE,
                   (void   *)0,
                   (INT16U  )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
                   
		/* 创建红灯任务 */
    OSTaskCreateExt(APP_TASK_RED,
                   (void   *)0,
                   (OS_STK *)&App_TaskRedStk[APP_TASK_USER_STK_SIZE - 1],
                   (INT8U   )APP_TASK_USER_RED_PRIO,
                   (INT16U  )APP_TASK_USER_RED_PRIO,
                   (OS_STK *)&App_TaskRedStk[0],
                   (INT32U  )APP_TASK_USER_STK_SIZE,
                   (void   *)0,
                   (INT16U  )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
                  
		/* 删除任务自身 */
    OSTaskDel(OS_PRIO_SELF);
		
		/* 关中断 */
		OS_EXIT_CRITICAL();
}



//START 任务

//创建任务堆栈空间	
OS_STK START_TASK_STK[START_STK_SIZE];

void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	pdata = pdata; 		  		 			  
	OS_CPU_SysTickInit();
	OSStatInit();					//初始化统计任务.这里会延时1秒钟左右	
 	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
	
	//开始创建用户任务
 	//OSTaskCreate(led_task,(void *)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);//闪灯任务，无用		
 	OSTaskCreateExt(receiver_task,(void *)0,(OS_STK*)&RECEIVER_TASK_STK[RECEIVER_STK_SIZE-1],RECEIVER_TASK_PRIO,0,&RECEIVER_TASK_STK[0],sizeof(RECEIVER_TASK_STK),(void*)0,OS_TASK_OPT_SAVE_FP|OS_TASK_OPT_STK_CHK);	
 	OSTaskCreateExt(attitude_task,(void *)0,(OS_STK*)&ATTITUDE_TASK_STK[ATTITUDE_STK_SIZE-1],ATTITUDE_TASK_PRIO,0,&ATTITUDE_TASK_STK[0],sizeof(ATTITUDE_TASK_STK),(void*)0,OS_TASK_OPT_SAVE_FP|OS_TASK_OPT_STK_CHK);	
	OSTaskCreateExt(scan_task,(void *)0,(OS_STK*)&SCAN_TASK_STK[SCAN_STK_SIZE-1],SCAN_TASK_PRIO,0,&SCAN_TASK_STK[0],sizeof(SCAN_TASK_STK),(void*)0,OS_TASK_OPT_SAVE_FP|OS_TASK_OPT_STK_CHK );	
	
 	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)

}	  
