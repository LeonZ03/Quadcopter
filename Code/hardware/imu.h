//#include "main.h"

#ifndef __IMU_H
#define	__IMU_H

//#include "alldata.h"
#include "stdint.h"

#define squa( Sq )        (((float)Sq)*((float)Sq))
//	
extern const float M_PI ;
extern const float RtA ;
extern const float AtR ;
extern const float Gyro_G ;	  	//陀螺仪初始化量程+-2000度每秒于1 / (65536 / 4000) = 0.03051756*2		
extern const float Gyro_Gr ;     //面计算度每秒,转换弧度每秒则 2*0.03051756	 * 0.0174533f = 0.0005326*2




typedef struct{
	int16_t accX;
	int16_t accY;
	int16_t accZ;
	int16_t gyroX;
	int16_t gyroY;
	int16_t gyroZ;
	int16_t hmcX;
	int16_t hmcY;
	int16_t hmcZ;
}_st_Mpu;

typedef struct{
	float roll;
	float pitch;
	float yaw;
}_st_AngE;



/* 四元数系数 */
typedef volatile struct {
  float q0;
  float q1;
  float q2;
  float q3;
} Quaternion;


/* 陀螺仪积分误差 */
struct V{
	float x;
	float y;
	float z;
};	

struct V1{
	int32_t x;
	int32_t y;
	int32_t z;
};	



extern _st_Mpu MPU6050;
extern _st_AngE Angle;
extern  float dt;


static float NormAcc;
static float NormHmc;

extern volatile struct V Gravity,Acc,Gyro,ex_acc_hmc,Mag_tran,Mag_hmc;
extern volatile struct V1 HmcMag_W,HmcMag_B;

extern void GetAngle(const _st_Mpu *pMpu,_st_AngE *pAngE);
extern void imu_rest(void);

#endif

