#ifndef __GY86_H
#define __GY86_H	 




#include "main.h"



struct KalmanFilter{
	float LastP;		//上一次协方差
	float NewP;		//最新的协方差
	float Out;			//卡尔曼输出
	float Kg;				//卡尔曼增益
	float Q;				//过程噪声的协方差
	float R;				//观测噪声的协方差
};


extern _st_Mpu MPU6050;   //MPU6050原始数据
extern _st_AngE Angle;    //当前角度姿态值
extern float Hmc_X0,Hmc_Y0,Hmc_Z0;//地磁初始数据

extern int16_t mpu6050[7];
extern int16_t hmc5883l[3];



void MPU6050_Init(void);
void MPU6050_Read(int16_t* data);
void HMC5883L_Init(void);
void HMC5883L_Read(int16_t *data);

void calibrate(void);
void MpuGetData(void);
void kalmanfiter(struct KalmanFilter *EKF,float input);


#endif


