#include "main.h"


//float Kp,Ki,Kd;
int Pitch_Out,Roll_Out,Yaw_Out;
float w_pitch_out,w_roll_out,w_yaw_out;
float pitch_exp,roll_exp,yaw_exp=0;





//pid_struct In_p={0},In_r={0},In_y={0};//内环，角速度
//pid_struct Out_p={0},Out_r={0},Out_y={0};//外环，角度


pid_struct In_p,In_r,In_y;//内环，角速度
pid_struct Out_p,Out_r,Out_y;//外环，角度

void PID_Out_Control(){
	
	
	//pitch
	Out_p.exp=pitch_exp;
	Out_p.cur=Angle.pitch;//
	Out_p.err_sum+=(Out_p.exp-Out_p.cur);
	w_pitch_out=-1*(Out_p.Kp*(Out_p.exp-Out_p.cur) + Out_p.Ki*Out_p.err_sum*dt + Out_p.Kd*Gyro.y*RtA);//方向适配
	//Out_p.LastErr=Out_p.exp-Out_p.cur;
	
	//roll
	Out_r.exp=roll_exp;
	Out_r.cur=Angle.roll;//转弧度制
	Out_r.err_sum+=(Out_r.exp-Out_r.cur);
	w_roll_out=Out_r.Kp*(Out_r.exp-Out_r.cur) + Out_r.Ki*Out_r.err_sum*dt - Out_r.Kd*Gyro.x*RtA;//方向适配
	//Out_r.LastErr=Out_r.exp-Out_r.cur;
	
	//yaw
	Out_y.exp=yaw_exp;
	Out_y.cur=Angle.yaw;//转弧度制
	Out_y.err_sum+=(Out_y.exp-Out_y.cur);
	w_yaw_out=-1*(Out_y.Kp*(Out_y.exp-Out_y.cur) + Out_y.Ki*Out_y.err_sum*dt + Out_y.Kd*Gyro.z*RtA);//方向适配
	//Out_y.LastErr=Out_y.exp-Out_y.cur;
	
	
}


void PID_In_Control(){
	
	//pitch
	In_p.exp=w_pitch_out;//输入w期望值
	In_p.cur=Gyro.y*RtA;//从陀螺仪获取当前值
	In_p.err_sum+=(In_p.exp-In_p.cur);//累计误差err
	Pitch_Out=In_p.Kp*(In_p.exp-In_p.cur) + In_p.Ki*In_p.err_sum*dt + In_p.Kd*(In_p.exp-In_p.cur-In_p.LastErr);//pid表达式，给出输出
	In_p.LastErr=In_p.exp-In_p.cur;//更新上一次的误差
	
	//roll
	In_r.exp=w_roll_out;
	In_r.cur=Gyro.x*RtA;
	In_r.err_sum+=(In_r.exp-In_r.cur);
	Roll_Out=In_r.Kp*(In_r.exp-In_r.cur) + In_r.Ki*In_r.err_sum*dt + In_r.Kd*(In_r.exp-In_r.cur-In_r.LastErr);
	
	
	//yaw
	In_y.exp=w_yaw_out;
	In_y.cur=Gyro.z*RtA;
	In_y.err_sum+=(In_y.exp-In_y.cur);
	Yaw_Out=In_y.Kp*(In_y.exp-In_y.cur) + In_y.Ki*In_y.err_sum*dt + In_y.Kd*(In_y.exp-In_y.cur-In_y.LastErr);
	In_y.LastErr=In_y.exp-In_y.cur;

}





void PID_SingleCir_Control(){
	
	//pitch
	Out_p.exp=pitch_exp;
	Out_p.cur=Angle.pitch;//转弧度制
	Out_p.err_sum+=(Out_p.exp-Out_p.cur);
	Pitch_Out=-1*(Out_p.Kp*(Out_p.exp-Out_p.cur) + Out_p.Ki*Out_p.err_sum*dt + Out_p.Kd*Gyro.y*RtA);//方向适配
	//Out_p.LastErr=Out_p.exp-Out_p.cur;
	
	//roll
	Out_r.exp=roll_exp;
	Out_r.cur=Angle.roll;//转弧度制
	Out_r.err_sum+=(Out_r.exp-Out_r.cur);
	Roll_Out=Out_r.Kp*(Out_r.exp-Out_r.cur) + Out_r.Ki*Out_r.err_sum*dt - Out_r.Kd*Gyro.x*RtA;//方向适配
	//Out_r.LastErr=Out_r.exp-Out_r.cur;
	
	//yaw
	Out_y.exp=yaw_exp;
	Out_y.cur=Angle.yaw;//转弧度制
	Out_y.err_sum+=(Out_y.exp-Out_y.cur);
	Yaw_Out=-1*(Out_y.Kp*(Out_y.exp-Out_y.cur) + Out_y.Ki*Out_y.err_sum*dt + Out_y.Kd*Gyro.z*RtA);//方向适配
	//Out_y.LastErr=Out_y.exp-Out_y.cur;
	
	
}


void PID_Init(){
	PID_StructInit(&In_p);
	PID_StructInit(&In_r);
	PID_StructInit(&In_y);
	PID_StructInit(&Out_p);
	PID_StructInit(&Out_r);
	PID_StructInit(&Out_y);
	
	
	if(CIRCLE==1){
		Out_p.Kp=8.2;
		Out_p.Ki=0;
		Out_p.Kd=2;
		
		Out_r.Kp=8;
		Out_r.Ki=0;
		Out_r.Kd=2.5;
		
		Out_y.Kp=0.5;
		Out_y.Ki=0;
		Out_y.Kd=0;
	}
	
	else if(CIRCLE==2){
		//pitch
		Out_p.Kp=3.6;
		Out_p.Ki=0;
		Out_p.Kd=0;
		
		In_p.Kp=0.8;
		In_p.Ki=0.05;
		In_p.Kd=0.4;
		
		
		
		//roll
		Out_r.Kp=3.6;
		Out_r.Ki=0;
		Out_r.Kd=0;
		
		In_r.Kp=0.6;
		In_r.Ki=0.2;
		In_r.Kd=0.3;
		
		
	
		//yaw
		Out_y.Kp=2;
		Out_y.Ki=0;
		Out_y.Kd=0;
		
		In_y.Kp=1;
		In_y.Ki=0;
		In_y.Kd=0;
	}
	
}

void PID_StructInit(pid_struct* p){
	
	p->cur=0;
	p->err_sum=0;
	p->exp=0;
	p->Kd=0;
	p->Ki=0;
	p->Kp=0;
	p->LastErr=0;
}

