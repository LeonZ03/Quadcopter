#include "main.h"

_st_Mpu MPU6050;   //MPU6050ԭʼ����
_st_AngE Angle;    //��ǰ�Ƕ���ֵ̬

static volatile int16_t *pMpu = (int16_t *)&MPU6050;
int16_t MpuOffset[6] = {0};		//MPU6050������ֵ

int16_t mpu6050[7];
int16_t hmc5883l[3];


void MPU6050_Init(void){  //��ʼ��MPU6050
		I2C_SEND_BYTE(0xD0,0x6B,0x00);//�������״̬
		I2C_SEND_BYTE(0xD0,0x19,0x01);//�����ǲ�����
		I2C_SEND_BYTE(0xD0,0x1A,0x06);//�жϺ͵�ͨ�˲���
				
		//I2C_SEND_BYTE(0xD0,0x1A,0x00);//�жϺ͵�ͨ�˲���

		I2C_SEND_BYTE(0xD0,0x1B,0x18);//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)	
		I2C_SEND_BYTE(0xD0,0x1C,0x00);//���ü��ٶȴ�����������2Gģʽ
}

void MPU6050_Read(int16_t* data){ //����X��Y��Z������ٶ�/������ԭʼ���� 
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
//			//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���
//			t[2*i]=I2C_READ_BYTE(0xD0,addr);
//			t[2*i+1]=I2C_READ_BYTE(0xD0,addr+1);
//			//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//�ж���
//			
//			
//			data [i]=((t[2*i]<<8)+t[2*i+1]);

//			addr+=2;//��Ϊһ�ζ�ȡ�������Ĵ���������+2
//			
//		}	
		
//		MpuGetData();
}

//extern int16_t mpu6050[7];

void MpuGetData(void)
{
//	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,DISABLE);//�ж���

	MPU6050_Read(mpu6050);
	HMC5883L_Read(hmc5883l);
	
	//TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	

	
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
		pMpu[i] = pMpu[i] - MpuOffset[i];							/* ��������Ϊ16bit������ȥˮƽУ׼ֵ */
		if(i < 3)		/* ���ٶȿ������˲� */
		{
			continue;//�����˲�
			{
				static struct KalmanFilter EKF[3] = {{0.02,0,0,0,0.001,0.543},{0.02,0,0,0,0.001,0.543},{0.02,0,0,0,0.001,0.543}};	
				kalmanfiter(&EKF[i],(float)pMpu[i]);  
				pMpu[i] = (int16_t)EKF[i].Out;
//				printf("EKF:%f\r\n",EKF[i].Out);
			}
		}
		if(i > 2)		/* ���ٶ�һ�׻����˲� */
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
		//������MPU6050�Ĵ���������IIC�豸���ص�������
		I2C_SEND_BYTE(0xD0,0x6A,0x00);
		I2C_SEND_BYTE(0xD0,0x37,0x02);
		//����HMC5883L����ؼĴ���
		I2C_SEND_BYTE(0x3C,0x00,0x70);
		I2C_SEND_BYTE(0x3C,0x01,0x20);
		I2C_SEND_BYTE(0x3C,0x02,0x00);
}

void HMC5883L_Read(int16_t *data)
{
		uint8_t i;
  	 	uint8_t t[6]; 
		uint8_t addr=0x03;
		//��ȡ״̬�Ĵ���������׼�������ٽ��ж�ȡ
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


//һά�������˲�
void kalmanfiter(struct KalmanFilter *EKF,float input)
{
	EKF->NewP = EKF->LastP + EKF->Q;
	EKF->Kg = EKF->NewP / (EKF->NewP + EKF->R);
	EKF->Out = EKF->Out + EKF->Kg * (input - EKF->Out);
	EKF->LastP = (1 - EKF->Kg) * EKF->NewP;
}


float Hmc_X0,Hmc_Y0,Hmc_Z0;//�شų�ʼ����

void calibrate(void){
	delay_ms(1000);
	led_up;
	delay_ms(100);
	led_down;
	
	int32_t acc_x_t=0,acc_y_t=0,acc_z_t=0,gyr_x_t=0,gyr_y_t=0,gyr_z_t=0,hmc_x_t=0,hmc_y_t=0,hmc_z_t=0, hmc_LenXYZ_t;
	
	for(int i=0;i<100;i++){//��100������ȡƽ��
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
	
		
		//�شŹ�һ��
		hmc_LenXYZ_t=sqrt(squa(hmc_x_t)+squa(hmc_y_t)+squa(hmc_z_t));
		Hmc_X0=hmc_x_t/hmc_LenXYZ_t;
		Hmc_Y0=hmc_y_t/hmc_LenXYZ_t;
		Hmc_Z0=hmc_z_t/hmc_LenXYZ_t;
		
}
