#include "main.h"




void Motor_Init(void){
	
	if(ch[6]>1000){//油门锁，下拨上锁
	PWM_SetCCR_1(1000-1);
	PWM_SetCCR_2(1000-1);
	PWM_SetCCR_3(1000-1);
	PWM_SetCCR_4(1000-1);
		
		return ;
	}
	
	OC_PWM_Init(10000-1,60-1);//分辨率：ccr范围1000-2000
	
	PWM_SetCCR_1(2000-1);
	PWM_SetCCR_2(2000-1);
	PWM_SetCCR_3(2000-1);
	PWM_SetCCR_4(2000-1);
	delay_ms(2600);
	
	PWM_SetCCR_1(1000-1);
	PWM_SetCCR_2(1000-1);
	PWM_SetCCR_3(1000-1);
	PWM_SetCCR_4(1000-1);
	
	delay_ms(2600);
	
	
	//OS_CPU_SysTickInit();//由于使用了delay_ms,影响了systick的配置，因此需要重新初始化系统时钟

}

int Power1,Power2,Power3,Power4;
void Motor_Run(int speed){//speed=0-1000
	
	
	if(ch[6]>1000){//油门锁，下拨上锁
		
	PWM_SetCCR_1(1000-1);
	PWM_SetCCR_2(1000-1);
	PWM_SetCCR_3(1000-1);
	PWM_SetCCR_4(1000-1);
		
		return ;
	}
	
	if(speed<0)speed=0;//防止越界
	if(speed>1000)speed=1000;
	
//	Power1=speed/50+20-1+Pitch_Out-Roll_Out+Yaw_Out;
//	Power2=speed/50+20-1+Pitch_Out+Roll_Out-Yaw_Out;
//	Power3=speed/50+20-1-Pitch_Out-Roll_Out-Yaw_Out;
//	Power4=speed/50+20-1-Pitch_Out+Roll_Out+Yaw_Out;
	
	Power1=speed+1000-1+Pitch_Out-Roll_Out+Yaw_Out;
	Power2=speed+1000-1+Pitch_Out+Roll_Out-Yaw_Out;
	Power3=speed+1000-1-Pitch_Out-Roll_Out-Yaw_Out;
	Power4=speed+1000-1-Pitch_Out+Roll_Out+Yaw_Out;
	
	PWM_SetCCR_1(Power1);
	PWM_SetCCR_2(Power2);
	PWM_SetCCR_3(Power3);
	PWM_SetCCR_4(Power4);
	
//	PWM_SetCCR_1(speed/50+20-1);
//	PWM_SetCCR_2(speed/50+20-1);
//	PWM_SetCCR_3(speed/50+20-1);
//	PWM_SetCCR_4(speed/50+20-1);
	
}

void MotorInit_Handle(){
	if(ch[4]<800)ElcContrller_cur=1;
	else ElcContrller_cur=0;
	
	if(ElcContrller_cur==1&&ElcContrller_last==0){//必须上一次长脉宽，下一次短脉宽才解锁
		led_up;
		Motor_Init();//电调初始化
		led_down;
		}
			
		ElcContrller_last=ElcContrller_cur;
		
}

int ch3_tmp;
void MovementHandle(){
	pitch_exp=((int)ch[1]-1000-4)*0.04;
	roll_exp=((int)ch[0]-1000-2)*0.04;
	
	ch3_tmp=ch[3]+3;
	if(ch3_tmp>=998 && ch3_tmp<=1002)ch3_tmp=1000;
	yaw_exp+=(ch3_tmp-1000)*dt*0.025;
}
