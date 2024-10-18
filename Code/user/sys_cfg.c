

#include "main.h"

/***
 * �������� : OS_CPU_SysTickInit();
 *
 * �������� : �δ�ʱ���趨;
 *
 * ����ֵ	  : ��;
 *
 * ����ֵ   : ��;
 *
 **/
void OS_CPU_SysTickInit(void)
{
		RCC_ClocksTypeDef rcc_clocks;
		
		/* ��ȡϵͳʱ�� */
		RCC_GetClocksFreq(&rcc_clocks);
		
		/* ���õδ�ʱ���������ֵ */
		SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);//ʱ��Ϊ1ms
}



/***
 * �������� : Sys_Config();
 *
 * �������� : ϵͳ��ʼ������;
 *
 * ����ֵ	  : ��;
 *
 * ����ֵ   : ��;
 *
 **/
void Sys_Config(void)
{
	  SystemCoreClockUpdate();
		OLED_Init();//OLED��ʼ��
		led_init();
		//Motor_Init();//���������ʼ��//ͳһ�ŵ������жϣ�����������
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2		
	//	TIM1_CH1_Cap_Init(0XFFFF,120-1); //���벶���ʼ������1Mhz��Ƶ�ʼ��� 
		EXTIX_Init(); //��ʼ���ⲿ�ж�����
		TIM2_Init(0xffffffff,60-1);//��ʼ����ʱ��TIM2�����ڸ�������ʱ
	
		I2C_Configuration();//IIC��ʼ������
		BlueTooth_Init();//������ʼ��
		MPU6050_Init();//MPU6050��ʼ��
		HMC5883L_Init();//HMC5883L��ʼ��
		calibrate();//imuУ��
		PID_Init();
	
		TIM1_CH1_Cap_Init(0XFFFF,120-1); //���벶���ʼ������1Mhz��Ƶ�ʼ��� 


		
		EventRecorderInitialize(EventRecordAll, 1U);//��ʼ��EventRecorder
		EventRecorderStart();

}
