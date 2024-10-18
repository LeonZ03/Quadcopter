#ifndef __PID_H
#define __PID_H



//extern float Kp,Ki,Kd;


typedef struct _pid_struct
{
	float Kp;
	float Ki;
	float Kd;
	float cur;
	float LastErr;
	float exp;
//	float err;
	float err_sum;

}pid_struct;



extern pid_struct In_p,In_r,In_y;//�ڻ������ٶ�
extern pid_struct Out_p,Out_r,Out_y;//�⻷���Ƕ�



extern int Pitch_Out,Roll_Out,Yaw_Out;
extern float w_pitch_out,w_roll_out,w_yaw_out;
extern float pitch_exp,roll_exp,yaw_exp;


#define CIRCLE 2		//pid����


void PID_In_Control(void);
void PID_Out_Control(void);
void PID_StructInit(pid_struct* p);
void PID_Init(void);

void PID_SingleCir_Control(void);




#endif

