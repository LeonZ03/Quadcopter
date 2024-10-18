

#include "main.h"

/***
 * 函数名称 : OS_CPU_SysTickInit();
 *
 * 函数描述 : 滴答定时器设定;
 *
 * 传递值	  : 无;
 *
 * 返回值   : 无;
 *
 **/
void OS_CPU_SysTickInit(void)
{
		RCC_ClocksTypeDef rcc_clocks;
		
		/* 获取系统时钟 */
		RCC_GetClocksFreq(&rcc_clocks);
		
		/* 设置滴答定时器溢出计数值 */
		SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);//时基为1ms
}



/***
 * 函数名称 : Sys_Config();
 *
 * 函数描述 : 系统初始化配置;
 *
 * 传递值	  : 无;
 *
 * 返回值   : 无;
 *
 **/
void Sys_Config(void)
{
	  SystemCoreClockUpdate();
		OLED_Init();//OLED初始化
		led_init();
		//Motor_Init();//电调解锁初始化//统一放到按键中断，长按键触发
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2		
	//	TIM1_CH1_Cap_Init(0XFFFF,120-1); //输入捕获初始化，以1Mhz的频率计数 
		EXTIX_Init(); //初始化外部中断输入
		TIM2_Init(0xffffffff,60-1);//初始化定时器TIM2，用于给按键计时
	
		I2C_Configuration();//IIC初始化配置
		BlueTooth_Init();//蓝牙初始化
		MPU6050_Init();//MPU6050初始化
		HMC5883L_Init();//HMC5883L初始化
		calibrate();//imu校正
		PID_Init();
	
		TIM1_CH1_Cap_Init(0XFFFF,120-1); //输入捕获初始化，以1Mhz的频率计数 


		
		EventRecorderInitialize(EventRecordAll, 1U);//初始化EventRecorder
		EventRecorderStart();

}
