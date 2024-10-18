#include "main.h"

unsigned short int scan_flag=0;


void KEY_Init(void)
{
		 GPIO_InitTypeDef GPIO_InitStructure;
		 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ�� GPIOCʱ��
	
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //����B1 ��Ӧ����
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
		 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		 GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ�� GPIOB13
		 
}



EXTI_InitTypeDef EXTI_InitStructure;//���ȫ�ֱ�������Ϊ�������Ҫͬʱ����
void EXTIX_Init(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		
	
		KEY_Init(); //������Ӧ�� IO �ڳ�ʼ��
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ�� SYSCFG ʱ��
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);//PB13 ������ 13
		
		 /* ���� EXTI_Line13 */
		 EXTI_InitStructure.EXTI_Line = EXTI_Line13;//LINE13
		 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
		 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
		 EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ�� LINE13
		 EXTI_Init(&EXTI_InitStructure);
		
		 NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж� 13
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ� 0
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//��Ӧ���ȼ� 2
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
		 NVIC_Init(&NVIC_InitStructure);//���� NVIC
	
		 key_flag=0;//������־λ
		 
}

unsigned char key_flag;//������־λ

//extern uint8_t clear_flag;
void EXTI15_10_IRQHandler(){
		
		if(EXTI_GetITStatus(EXTI_Line13)!=RESET)//�ж�ĳ�����ϵ��ж��Ƿ��� 
		{ 
			
			
			if(key_flag==0){//�հ���
				
				TIM_SetCounter(TIM2, 0);
				TIM_Cmd(TIM2,ENABLE);
				EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//���������ش���
				EXTI_Init(&EXTI_InitStructure);
				
				key_flag=1;
			}
			else{
				int time=TIM2->CNT;
				TIM_Cmd(TIM2,DISABLE);
				//TIM_SetCounter(TIM2, 0);
				
				EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�����½��ش���
				EXTI_Init(&EXTI_InitStructure);
				
				key_flag=0;
				
				//OLED_ShowNum(2,1,time,14);//��ʾ��ѹʱ��
				if(time>1000000){//����
					EventStartD(0);
					
					led_up;
					Motor_Init();//�����ʼ��
					led_down;
					
					EventStopD(0);
				}
				else{//�̰�
					EventStartD(1);
					
					scan_flag=(scan_flag+1)%4;//�л���Ļ
					clear_flag=1;
//					OLED_MyClear();
					//printf("the press time is %d\n",time);
					
					EventStopD(1);
				}
			
			}
			TIM_Cmd(TIM2,ENABLE);
			
			EXTI_ClearITPendingBit(EXTI_Line13); //��� LINE �ϵ��жϱ�־λ 
		}
		
}




//1. ��� APB Ԥ��Ƶ��Ϊ 1����ʱ��ʱ��Ƶ�ʵ��� APB ���Ƶ�ʡ�
//2. ���򣬵��� APB ���Ƶ�ʵ����� (��2)��
void TIM2_Init(u32 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //ʹ�� TIM2 ʱ��
		TIM_TimeBaseInitStructure.TIM_Period = arr; //�Զ���װ��ֵ
		TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��ʱ����Ƶ
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);// ��ʼ����ʱ�� TIM2
	
		//TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ�� 2
}

