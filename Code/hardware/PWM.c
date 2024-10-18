

#include"main.h"

//TIM3挂载在APB1下，当APB分频数不为1时，TIM3时钟频率为APB1的两倍，这里也就是30M*2=60M
void OC_PWM_Init(u16 arr,u16 psc){


//使能GPIOC，
//设置复用推挽输出
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//pc6
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//复用模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
	
	//pc7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//复用模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);
	
	//pc8
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//复用模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);
	
	//pc9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//复用模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);

	
	
//初始化定时器
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义初始化结构体
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能定时器3时钟
	TIM_InternalClockConfig(TIM3);

		//初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //自动重装载寄存器的值(ARR)
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //TIMX预分频的值(PSC)  12
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//无用，但给他一个值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据以上功能对定时器进行初始化
	
	
//设置TIM3_CH1的PWM模式，使能TIM3的CH1输出
	TIM_OCInitTypeDef TIM_OCInitStructure;//定义结构体
	TIM_OCStructInit(&TIM_OCInitStructure);//给结构体所有成员一个默认值，后续根据需要再修改
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//选择定时器模式，PWM1常用，表示有效电平为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出比较极性高
	TIM_OCInitStructure.TIM_Pulse=0;//CCR
	
	
	//ch1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);//根据结构体信息进行初始化
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//ch2
	TIM_OCInitStructure.TIM_Pulse=0;//CCR
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);//根据结构体信息进行初始化
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//ch3
	TIM_OCInitStructure.TIM_Pulse=0;//CCR
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);//根据结构体信息进行初始化
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//ch4
	TIM_OCInitStructure.TIM_Pulse=0;//CCR
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);//根据结构体信息进行初始化
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3,ENABLE);
	

}


//void PWM_SetCCR(uint16_t CCR){//单独更改CCR
//	TIM_SetCompare1(TIM3,CCR);//单独更改TIM3通道1的CCR
//}





//定时器5通道1输入捕获配置
//arr：自动重装值(TIM2,TIM1是32位的!!)
//psc：时钟预分频数
void TIM1_CH1_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA8

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //PA8复用位定时器1
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	//初始化TIM1输入捕获参数
	TIM_ICInitTypeDef  TIM1_ICInitStructure;
	
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM1_ICInitStructure.TIM_ICFilter = 0xF;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM1, &TIM1_ICInitStructure);
		
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//允许更新中断 ,允许CC1IE捕获中断	

	TIM_Cmd(TIM1,ENABLE ); 	//使能定时器1

 
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;//TIM1 Capture Compare Interrupt
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	
}
//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM1CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM1CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM1是32位)
//定时器1中断服务程序	 
 
void TIM1_CC_IRQHandler(void)
{ 		    
	EventStartD(2);
	
	EventStopD(2);
	
 	if((TIM1CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM1CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM1CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
			  TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);//获取当前的捕获值.
	 			TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
				
				PPM_Handle();//PPM添加一路
				//if(ch_flag==3)Motor_Run(ch[2]-500);
				
				TIM1CH1_CAPTURE_STA=0;
			}else  								//还未开始,第一次捕获上升沿
			{
				cap_1_ch=1;
				
				
				TIM1CH1_CAPTURE_STA=0;			//清空
				TIM1CH1_CAPTURE_VAL=0;
				TIM1CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM1,DISABLE ); 	//关闭定时器1
	 			TIM_SetCounter(TIM1,0);
	 			TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM1,ENABLE ); 	//使能定时器1
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
	
	
}


u32 ch[8],ch_temp;
u8 ch_flag;
u8 ElcContrller_last=0, ElcContrller_cur=0;
u8 cap_1_ch=0;
void PPM_Handle(){
	ch_temp=TIM1CH1_CAPTURE_VAL;
	if(ch_temp>2000){//一个PPM周期开始
		ch_flag=0;
	}
	else{
		ch[ch_flag++]=TIM1CH1_CAPTURE_VAL+10;
		
	}
	if(ch[6]>1000){//油门锁，下拨上锁
		
	PWM_SetCCR_1(1000-1);
	PWM_SetCCR_2(1000-1);
	PWM_SetCCR_3(1000-1);
	PWM_SetCCR_4(1000-1);
	}
}










