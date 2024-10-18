#include "main.h"

unsigned short int scan_flag=0;


void KEY_Init(void)
{
		 GPIO_InitTypeDef GPIO_InitStructure;
		 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能 GPIOC时钟
	
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //按键B1 对应引脚
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
		 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		 GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化 GPIOB13
		 
}



EXTI_InitTypeDef EXTI_InitStructure;//设成全局变量，因为多个函数要同时访问
void EXTIX_Init(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		
	
		KEY_Init(); //按键对应的 IO 口初始化
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能 SYSCFG 时钟
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);//PB13 连接线 13
		
		 /* 配置 EXTI_Line13 */
		 EXTI_InitStructure.EXTI_Line = EXTI_Line13;//LINE13
		 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
		 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
		 EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能 LINE13
		 EXTI_Init(&EXTI_InitStructure);
		
		 NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断 13
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级 0
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
		 NVIC_Init(&NVIC_InitStructure);//配置 NVIC
	
		 key_flag=0;//触发标志位
		 
}

unsigned char key_flag;//触发标志位

//extern uint8_t clear_flag;
void EXTI15_10_IRQHandler(){
		
		if(EXTI_GetITStatus(EXTI_Line13)!=RESET)//判断某个线上的中断是否发生 
		{ 
			
			
			if(key_flag==0){//刚按下
				
				TIM_SetCounter(TIM2, 0);
				TIM_Cmd(TIM2,ENABLE);
				EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//设置上升沿触发
				EXTI_Init(&EXTI_InitStructure);
				
				key_flag=1;
			}
			else{
				int time=TIM2->CNT;
				TIM_Cmd(TIM2,DISABLE);
				//TIM_SetCounter(TIM2, 0);
				
				EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//设置下降沿触发
				EXTI_Init(&EXTI_InitStructure);
				
				key_flag=0;
				
				//OLED_ShowNum(2,1,time,14);//显示按压时间
				if(time>1000000){//长按
					EventStartD(0);
					
					led_up;
					Motor_Init();//电调初始化
					led_down;
					
					EventStopD(0);
				}
				else{//短按
					EventStartD(1);
					
					scan_flag=(scan_flag+1)%4;//切换屏幕
					clear_flag=1;
//					OLED_MyClear();
					//printf("the press time is %d\n",time);
					
					EventStopD(1);
				}
			
			}
			TIM_Cmd(TIM2,ENABLE);
			
			EXTI_ClearITPendingBit(EXTI_Line13); //清除 LINE 上的中断标志位 
		}
		
}




//1. 如果 APB 预分频器为 1，定时器时钟频率等于 APB 域的频率。
//2. 否则，等于 APB 域的频率的两倍 (×2)。
void TIM2_Init(u32 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //使能 TIM2 时钟
		TIM_TimeBaseInitStructure.TIM_Period = arr; //自动重装载值
		TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //定时器分频
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);// 初始化定时器 TIM2
	
		//TIM_Cmd(TIM2,ENABLE); //使能定时器 2
}

