

#include"main.h"

//TIM3������APB1�£���APB��Ƶ����Ϊ1ʱ��TIM3ʱ��Ƶ��ΪAPB1������������Ҳ����30M*2=60M
void OC_PWM_Init(u16 arr,u16 psc){


//ʹ��GPIOC��
//���ø����������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//pc6
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//����ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
	
	//pc7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//����ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);
	
	//pc8
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//����ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);
	
	//pc9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//����ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);

	
	
//��ʼ����ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//�����ʼ���ṹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ�ܶ�ʱ��3ʱ��
	TIM_InternalClockConfig(TIM3);

		//��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //�Զ���װ�ؼĴ�����ֵ(ARR)
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //TIMXԤ��Ƶ��ֵ(PSC)  12
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//���ã�������һ��ֵ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //�������Ϲ��ܶԶ�ʱ�����г�ʼ��
	
	
//����TIM3_CH1��PWMģʽ��ʹ��TIM3��CH1���
	TIM_OCInitTypeDef TIM_OCInitStructure;//����ṹ��
	TIM_OCStructInit(&TIM_OCInitStructure);//���ṹ�����г�Աһ��Ĭ��ֵ������������Ҫ���޸�
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//ѡ��ʱ��ģʽ��PWM1���ã���ʾ��Ч��ƽΪ�ߵ�ƽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse=0;//CCR
	
	
	//ch1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);//���ݽṹ����Ϣ���г�ʼ��
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//ch2
	TIM_OCInitStructure.TIM_Pulse=0;//CCR
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);//���ݽṹ����Ϣ���г�ʼ��
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//ch3
	TIM_OCInitStructure.TIM_Pulse=0;//CCR
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);//���ݽṹ����Ϣ���г�ʼ��
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//ch4
	TIM_OCInitStructure.TIM_Pulse=0;//CCR
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);//���ݽṹ����Ϣ���г�ʼ��
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3,ENABLE);
	

}


//void PWM_SetCCR(uint16_t CCR){//��������CCR
//	TIM_SetCompare1(TIM3,CCR);//��������TIM3ͨ��1��CCR
//}





//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM1��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
void TIM1_CH1_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA8

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //PA8����λ��ʱ��1
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	//��ʼ��TIM1���벶�����
	TIM_ICInitTypeDef  TIM1_ICInitStructure;
	
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM1_ICInitStructure.TIM_ICFilter = 0xF;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM1, &TIM1_ICInitStructure);
		
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//��������ж� ,����CC1IE�����ж�	

	TIM_Cmd(TIM1,ENABLE ); 	//ʹ�ܶ�ʱ��1

 
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;//TIM1 Capture Compare Interrupt
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	
}
//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM1CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM1CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM1��32λ)
//��ʱ��1�жϷ������	 
 
void TIM1_CC_IRQHandler(void)
{ 		    
	EventStartD(2);
	
	EventStopD(2);
	
 	if((TIM1CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM1CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM1CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
			  TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);//��ȡ��ǰ�Ĳ���ֵ.
	 			TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
				
				PPM_Handle();//PPM���һ·
				//if(ch_flag==3)Motor_Run(ch[2]-500);
				
				TIM1CH1_CAPTURE_STA=0;
			}else  								//��δ��ʼ,��һ�β���������
			{
				cap_1_ch=1;
				
				
				TIM1CH1_CAPTURE_STA=0;			//���
				TIM1CH1_CAPTURE_VAL=0;
				TIM1CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM1,DISABLE ); 	//�رն�ʱ��1
	 			TIM_SetCounter(TIM1,0);
	 			TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM1,ENABLE ); 	//ʹ�ܶ�ʱ��1
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
	
	
}


u32 ch[8],ch_temp;
u8 ch_flag;
u8 ElcContrller_last=0, ElcContrller_cur=0;
u8 cap_1_ch=0;
void PPM_Handle(){
	ch_temp=TIM1CH1_CAPTURE_VAL;
	if(ch_temp>2000){//һ��PPM���ڿ�ʼ
		ch_flag=0;
	}
	else{
		ch[ch_flag++]=TIM1CH1_CAPTURE_VAL+10;
		
	}
	if(ch[6]>1000){//���������²�����
		
	PWM_SetCCR_1(1000-1);
	PWM_SetCCR_2(1000-1);
	PWM_SetCCR_3(1000-1);
	PWM_SetCCR_4(1000-1);
	}
}










