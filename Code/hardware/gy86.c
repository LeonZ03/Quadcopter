#include "main.h"

_st_Mpu MPU6050;   //MPU6050原始数据
_st_AngE Angle;    //当前角度姿态值

static volatile int16_t *pMpu = (int16_t *)&MPU6050;
int16_t MpuOffset[6] = {0};		//MPU6050补偿数值

int16_t mpu6050[7];
int16_t hmc5883l[3];


void MPU6050_Init(void){  //初始化MPU6050
		I2C_SEND_BYTE(0xD0,0x6B,0x00);//解除休眠状态
		I2C_SEND_BYTE(0xD0,0x19,0x01);//陀螺仪采样率
		I2C_SEND_BYTE(0xD0,0x1A,0x06);//中断和低通滤波器
				
		//I2C_SEND_BYTE(0xD0,0x1A,0x00);//中断和低通滤波器

		I2C_SEND_BYTE(0xD0,0x1B,0x18);//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)	
		I2C_SEND_BYTE(0xD0,0x1C,0x00);//配置加速度传感器工作在2G模式
}

void MPU6050_Read(int16_t* data){ //读出X、Y、Z三轴加速度/陀螺仪原始数据 
		uint8_t i;
    	uint8_t t[14];
		uint8_t addr=0x3B;     
		I2C_READ_BYTE_LEN(0xd0,addr,t,14);
		
	for(i=0;i<7;i++)
		{		
						data [i]=((t[2*i]<<8)+t[2*i+1]);
		}	
//		for(i=0;i<7;i++)
//		{
//			//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁
//			t[2*i]=I2C_READ_BYTE(0xD0,addr);
//			t[2*i+1]=I2C_READ_BYTE(0xD0,addr+1);
//			//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//中断锁
//			
//			
//			data [i]=((t[2*i]<<8)+t[2*i+1]);

//			addr+=2;//因为一次读取了两个寄存器，所以+2
//			
//		}	
		
//		MpuGetData();
}

//extern int16_t mpu6050[7];

void MpuGetData(void)
{
//	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//中断锁

	MPU6050_Read(mpu6050);
	HMC5883L_Read(hmc5883l);
	
	//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	

	
	uint8_t i;
	
	MPU6050.accX=mpu6050[0];
	MPU6050.accY=mpu6050[1];
	MPU6050.accZ=mpu6050[2];
	
	MPU6050.gyroX=mpu6050[4];
	MPU6050.gyroY=mpu6050[5];
	MPU6050.gyroZ=mpu6050[6];
	
	MPU6050.hmcX=hmc5883l[0];
	MPU6050.hmcY=hmc5883l[2];
	MPU6050.hmcZ=hmc5883l[1];

	
	for(i=0;i<6;i++)
	{
		pMpu[i] = pMpu[i] - MpuOffset[i];							/* 将数据整为16bit，并减去水平校准值 */
		if(i < 3)		/* 加速度卡尔曼滤波 */
		{
			continue;//不做滤波
			{
				static struct KalmanFilter EKF[3] = {{0.02,0,0,0,0.001,0.543},{0.02,0,0,0,0.001,0.543},{0.02,0,0,0,0.001,0.543}};	
				kalmanfiter(&EKF[i],(float)pMpu[i]);  
				pMpu[i] = (int16_t)EKF[i].Out;
//				printf("EKF:%f\r\n",EKF[i].Out);
			}
		}
		if(i > 2)		/* 角速度一阶互补滤波 */
		{	
			uint8_t k=i-3;
			const float factor = 0.15f;  	
			static float tBuff[3];		

			pMpu[i] = tBuff[k] = tBuff[k] * (1 - factor) + pMpu[i] * factor;                
		}
	}	
}
void HMC5883L_Init(void)
{
		//先配置MPU6050寄存器将辅助IIC设备挂载到总线上
		I2C_SEND_BYTE(0xD0,0x6A,0x00);
		I2C_SEND_BYTE(0xD0,0x37,0x02);
		//配置HMC5883L的相关寄存器
		I2C_SEND_BYTE(0x3C,0x00,0x70);
		I2C_SEND_BYTE(0x3C,0x01,0x20);
		I2C_SEND_BYTE(0x3C,0x02,0x00);
}

void HMC5883L_Read(int16_t *data)
{
		uint8_t i;
  	 	uint8_t t[6]; 
		uint8_t addr=0x03;
		//读取状态寄存器，数据准备就绪再进行读取
		//while(!(I2C_READ_BYTE(0x3C,0x09)&0x01));
		I2C_READ_BYTE_LEN(0x3c,addr,t,6);
//		for(i=0; i<3; i++) 	
//		{
//			t[2*i]=I2C_READ_BYTE(0x3C,addr);
//			t[2*i+1]=I2C_READ_BYTE(0x3C,addr+1);
//			data[i]=((t[2*i] << 8)+t[2*i+1]);
//			addr+=2;
//		}
	for(i=0; i<3; i++) 	
		{
			data[i]=((t[2*i] << 8)+t[2*i+1]);
		}
}


//一维卡尔曼滤波
void kalmanfiter(struct KalmanFilter *EKF,float input)
{
	EKF->NewP = EKF->LastP + EKF->Q;
	EKF->Kg = EKF->NewP / (EKF->NewP + EKF->R);
	EKF->Out = EKF->Out + EKF->Kg * (input - EKF->Out);
	EKF->LastP = (1 - EKF->Kg) * EKF->NewP;
}


float Hmc_X0,Hmc_Y0,Hmc_Z0;//地磁初始数据

void calibrate(void){
	delay_ms(1000);
	led_up;
	delay_ms(100);
	led_down;
	
	int32_t acc_x_t=0,acc_y_t=0,acc_z_t=0,gyr_x_t=0,gyr_y_t=0,gyr_z_t=0,hmc_x_t=0,hmc_y_t=0,hmc_z_t=0, hmc_LenXYZ_t;
	
	for(int i=0;i<100;i++){//采100次样，取平均
		MPU6050_Read(mpu6050);
		acc_x_t+=mpu6050[0];
		acc_y_t+=mpu6050[1];
		acc_z_t+=mpu6050[2];
		
		gyr_x_t+=mpu6050[4];
		gyr_y_t+=mpu6050[5];
		gyr_z_t+=mpu6050[6];
		
		hmc_x_t+=hmc5883l[0];
		hmc_y_t+=hmc5883l[2];
		hmc_z_t+=hmc5883l[1];
				
	}
		acc_x_t /= 100;
		acc_y_t /= 100;
		acc_z_t /= 100;
		gyr_x_t /= 100;
		gyr_y_t /= 100;
		gyr_z_t /= 100;
		hmc_x_t /= 100;
		hmc_y_t /= 100;
		hmc_z_t /= 100;
	
		MpuOffset[0]=acc_x_t;
		MpuOffset[1]=acc_y_t;
		MpuOffset[2]=acc_z_t - 16384;
	
		MpuOffset[3]=gyr_x_t;
		MpuOffset[4]=gyr_y_t;
		MpuOffset[5]=gyr_z_t;
	
		
		//地磁归一化
		hmc_LenXYZ_t=sqrt(squa(hmc_x_t)+squa(hmc_y_t)+squa(hmc_z_t));
		Hmc_X0=hmc_x_t/hmc_LenXYZ_t;
		Hmc_Y0=hmc_y_t/hmc_LenXYZ_t;
		Hmc_Z0=hmc_z_t/hmc_LenXYZ_t;
		
}
