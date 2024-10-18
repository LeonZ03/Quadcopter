#include "main.h"



int time_t;
int main(void)

{
		
	Sys_Config();
	
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	


	while(1){
		
	

		
		
		//EventStartA(1);//A1
		MotorInit_Handle();
		MovementHandle();
		//EventStopA(1);
		

	
			//EventStartA(2);//A2
		if((USART_RX_STA&0x8000)!=0){
			if(USART_RX_BUF[0]=='P'&&USART_RX_BUF[1]=='I'&&USART_RX_BUF[2]=='D')PID_DataHandle();

//			PID_DataHandle();
			//my_usart_transmit(USART1,(int8_t*)USART_RX_BUF,USART_RX_STA&0x3fff);
			USART_RX_STA=0;
		}
			//EventStopA(2);
		
		
		
		EventStartA(3);//A3
		
		MpuGetData();//采集GY-86数据
		
		EventStopA(3);
		
		
		
		
		
		time_t=TIM2->CNT;//定时器计算时钟周期
		dt=(float)time_t/1000000;
		TIM_Cmd(TIM2,DISABLE);
		
		
		TIM_SetCounter(TIM2, 0);
		TIM_Cmd(TIM2,ENABLE);
		
		
		
		//EventStartB(2);
		GetAngle(&MPU6050,&Angle);//姿态解算
		//EventStopB(2);
		
		
		
		
		//EventStopA(0);//测试周期时长
		//EventStartA(0);
		
		//In_p.Kp=0;
		
		//	PID_Out_Control();
		//	PID_In_Control();

		//EventStartB(0);       				//B0
			
		//PID_SingleCir_Control();
		
		PID_Out_Control();
		PID_In_Control();
		
		//EventStopB(0);
		
		
		Motor_Run(ch[2]-500);
		
		
		
		//EventStartB(1);
		
		Send_ANOTC();//向上位机发送数据
		
		//EventStopB(1);
		
		
		
		
		
		
		//OLED_Show();
		
		
	}
		
	

}




//int main(){
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2		
//	BlueTooth_Init();
//	
//	
//	while(1){
//		//USART_SendData(USART1,'A');
//		
//		int16_t str[8]={0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007};

//		my_usart_transmit(USART1,str,8);
//		
////		char* str="vghxwe";
////		USART_SendString(USART1,"vghxwe\n");
//		//printf("hhhhh\r\n");
//		//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//		
//		
//		//Send_ANOTC();
//	
//	}


//}






























//#include "stm32f4xx.h"
//#include "LED.h"
//#include "PWM.h"
//#include "MOTOR.h"
//#include "OLED.h"
//#include "main.h"
//#include "systick.h"





//extern u8  TIM1CH1_CAPTURE_STA;		//输入捕获状态		    				
//extern u32	TIM1CH1_CAPTURE_VAL;	//输入捕获值  
//extern u32 ch[];
//	
//int main(void)
//{ 
//	OLED_Init();//OLED初始化
//	Motor_Init();//电调解锁初始化
//	
//	
//	
//	
//	
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
//	
//	
// 
//	
// 	TIM1_CH1_Cap_Init(0XFFFFFFF,42-1); //输入捕获初始化，以1Mhz的频率计数 
//   	while(1)
//	{
//		
// 		delay_ms(10);//消抖
//				 

//		
//			OLED_ShowNum(1, 1, ch[0], 10);
//			OLED_ShowNum(2, 1, ch[1], 10);
//			OLED_ShowNum(3, 1, ch[2], 10);
//			OLED_ShowNum(4, 1, ch[3], 10);
//			
//			Motor_Run(ch[2]-500);


//	}
//	
//	
//}



