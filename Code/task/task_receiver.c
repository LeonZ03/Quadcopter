#include "main.h"

/***
 * �������� :receiver_task(void *pdata);;
 *
 * �������� : ���ջ�����;
 *
 * ����ֵ	  : ��;
 *
 * ����ֵ   : ��;
 *
 **/


//���������ջ�ռ�
OS_STK RECEIVER_TASK_STK[RECEIVER_STK_SIZE];


void receiver_task(void *pdata){
   	while(1)
	{
		EventStartA(0);
 		OSTimeDly(20);//��������20ms,��ppm������ͬ
		EventStopA(0);		 

//		OLED_ShowNum(1, 1, ch[0], 10);//ͳһ�ŵ���ʾ������
//		OLED_ShowNum(2, 1, ch[1], 10);
//		OLED_ShowNum(3, 1, ch[2], 10);
//		OLED_ShowNum(4, 1, ch[3], 10);
		
		EventStartA(1);
		Motor_Run(ch[2]-500);
		EventStopA(1);
	}
}

