#include "main.h"

#define LED_RED_OFF 		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
#define LED_RED_ON			GPIO_SetBits(GPIOD,GPIO_Pin_7);

#define LED_GREEN_OFF 	GPIO_ResetBits(GPIOG,GPIO_Pin_9);
#define LED_GREEN_ON 		GPIO_SetBits(GPIOG,GPIO_Pin_9);



/***
 * 函数名称 : APP_TASK_GREEN();
 *
 * 函数描述 : 绿灯任务;
 *
 * 传递值	  : 无;
 *
 * 返回值   : 无;
 *
 **/
void APP_TASK_GREEN(void *p_arg)
{
		for (;;)
		{
				/* 红灯亮 */
				LED_GREEN_ON;
				
				/* 延时0.5s，并任务切换 */
				OSTimeDly(500);
				
				/* 红灯灭 */
				LED_GREEN_OFF;
				
				/* 延时0.5s，并任务切换 */
				OSTimeDly(500);
		}
}

/***
 * 函数名称 : APP_TASK_RED();
 *
 * 函数描述 : 红灯任务;
 *
 * 传递值	  : 无;
 *
 * 返回值   : 无;
 *
 **/
void APP_TASK_RED(void *p_arg)
{
		for (;;)
		{
				/* 红灯亮 */
				LED_RED_ON;
				
				/* 延时0.25s，并任务切换 */
				OSTimeDly(250);
				
				/* 红灯灭 */
				LED_RED_OFF;
				
				/* 延时0.25s，并任务切换 */
				OSTimeDly(250);
		}
}


/***
 * 函数名称 :led_task(void *pdata);
 *
 * 函数描述 : 板载led任务;
 *
 * 传递值	  : 无;
 *
 * 返回值   : 无;
 *
 **/


//创建任务堆栈空间
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

