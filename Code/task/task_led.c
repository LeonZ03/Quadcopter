#include "main.h"

#define LED_RED_OFF 		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
#define LED_RED_ON			GPIO_SetBits(GPIOD,GPIO_Pin_7);

#define LED_GREEN_OFF 	GPIO_ResetBits(GPIOG,GPIO_Pin_9);
#define LED_GREEN_ON 		GPIO_SetBits(GPIOG,GPIO_Pin_9);



/***
 * �������� : APP_TASK_GREEN();
 *
 * �������� : �̵�����;
 *
 * ����ֵ	  : ��;
 *
 * ����ֵ   : ��;
 *
 **/
void APP_TASK_GREEN(void *p_arg)
{
		for (;;)
		{
				/* ����� */
				LED_GREEN_ON;
				
				/* ��ʱ0.5s���������л� */
				OSTimeDly(500);
				
				/* ����� */
				LED_GREEN_OFF;
				
				/* ��ʱ0.5s���������л� */
				OSTimeDly(500);
		}
}

/***
 * �������� : APP_TASK_RED();
 *
 * �������� : �������;
 *
 * ����ֵ	  : ��;
 *
 * ����ֵ   : ��;
 *
 **/
void APP_TASK_RED(void *p_arg)
{
		for (;;)
		{
				/* ����� */
				LED_RED_ON;
				
				/* ��ʱ0.25s���������л� */
				OSTimeDly(250);
				
				/* ����� */
				LED_RED_OFF;
				
				/* ��ʱ0.25s���������л� */
				OSTimeDly(250);
		}
}


/***
 * �������� :led_task(void *pdata);
 *
 * �������� : ����led����;
 *
 * ����ֵ	  : ��;
 *
 * ����ֵ   : ��;
 *
 **/


//���������ջ�ռ�
OS_STK LED_TASK_STK[LED_STK_SIZE];

void led_task(void *pdata)
{   
	while(1)
	{  
		led_up;
		OSTimeDly(15000);
		
		led_down;
		OSTimeDly(15000);

	}									 
}	   

