#ifndef __GY86_H
#define __GY86_H	 




#include "main.h"



struct KalmanFilter{
	float LastP;		//��һ��Э����
	float NewP;		//���µ�Э����
	float Out;			//���������
	float Kg;				//����������
	float Q;				//����������Э����
	float R;				//�۲�������Э����
};


extern _st_Mpu MPU6050;   //MPU6050ԭʼ����
extern _st_AngE Angle;    //��ǰ�Ƕ���ֵ̬
extern float Hmc_X0,Hmc_Y0,Hmc_Z0;//�شų�ʼ����

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


