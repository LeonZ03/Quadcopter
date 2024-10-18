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
 * �������� : APP_TASK_START();
 *
 * �������� : ucos_ii ��ʼ������;
 *
 * ����ֵ	  : ��;
 *
 * ����ֵ   : ��;
 *
 **/
void APP_TASK_START(void *p_arg)
{
		OS_CPU_SR cpu_sr;
    (void)p_arg;
		
		/* ���õδ�ʱ�����˺�������λ��Ϊ��һ����������ͷ */
    OS_CPU_SysTickInit();
									
		/* ���ж� */
		OS_ENTER_CRITICAL();
				
		/* �����̵����� */
    OSTaskCreateExt(APP_TASK_GREEN,
                   (void    *)0,
                   (OS_STK *)&App_TaskGreenStk[APP_TASK_USER_STK_SIZE - 1],
                   (INT8U   )APP_TASK_USER_GREEN_PRIO,
                   (INT16U  )APP_TASK_USER_GREEN_PRIO,
                   (OS_STK *)&App_TaskGreenStk[0],
                   (INT32U  )APP_TASK_USER_STK_SIZE,
                   (void   *)0,
                   (INT16U  )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
                   
		/* ����������� */
    OSTaskCreateExt(APP_TASK_RED,
                   (void   *)0,
                   (OS_STK *)&App_TaskRedStk[APP_TASK_USER_STK_SIZE - 1],
                   (INT8U   )APP_TASK_USER_RED_PRIO,
                   (INT16U  )APP_TASK_USER_RED_PRIO,
                   (OS_STK *)&App_TaskRedStk[0],
                   (INT32U  )APP_TASK_USER_STK_SIZE,
                   (void   *)0,
                   (INT16U  )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
                  
		/* ɾ���������� */
    OSTaskDel(OS_PRIO_SELF);
		
		/* ���ж� */
		OS_EXIT_CRITICAL();
}



//START ����

//���������ջ�ռ�	
OS_STK START_TASK_STK[START_STK_SIZE];

void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	pdata = pdata; 		  		 			  
	OS_CPU_SysTickInit();
	OSStatInit();					//��ʼ��ͳ������.�������ʱ1��������	
 	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
	
	//��ʼ�����û�����
 	//OSTaskCreate(led_task,(void *)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);//������������		
 	OSTaskCreateExt(receiver_task,(void *)0,(OS_STK*)&RECEIVER_TASK_STK[RECEIVER_STK_SIZE-1],RECEIVER_TASK_PRIO,0,&RECEIVER_TASK_STK[0],sizeof(RECEIVER_TASK_STK),(void*)0,OS_TASK_OPT_SAVE_FP|OS_TASK_OPT_STK_CHK);	
 	OSTaskCreateExt(attitude_task,(void *)0,(OS_STK*)&ATTITUDE_TASK_STK[ATTITUDE_STK_SIZE-1],ATTITUDE_TASK_PRIO,0,&ATTITUDE_TASK_STK[0],sizeof(ATTITUDE_TASK_STK),(void*)0,OS_TASK_OPT_SAVE_FP|OS_TASK_OPT_STK_CHK);	
	OSTaskCreateExt(scan_task,(void *)0,(OS_STK*)&SCAN_TASK_STK[SCAN_STK_SIZE-1],SCAN_TASK_PRIO,0,&SCAN_TASK_STK[0],sizeof(SCAN_TASK_STK),(void*)0,OS_TASK_OPT_SAVE_FP|OS_TASK_OPT_STK_CHK );	
	
 	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)

}	  
