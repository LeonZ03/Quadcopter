#include "main.h"

const float M_PI = 3.1415926535;
const float RtA = 57.2957795f;				//����ת�Ƕ�
const float AtR = 0.0174532925f;			//�Ƕ�ת����
const float Gyro_G = 0.03051756f*2;	  	//������ADCתdegree/s,�����ǳ�ʼ������+-2000��ÿ����1 / (65536 / 4000) = 0.03051756*2		
const float Gyro_Gr = 0.0005326f*2;     //������ADCתrad/s,������ÿ��,ת������ÿ���� 2*0.03051756	 * 0.0174533f = 0.0005326*2

static float NormAcc; 						//ģ������һ����


Quaternion NumQ = {1, 0, 0, 0};


volatile struct V exInt = {0};


//#define	YAW_GYRO
static float k_acc=1;
static float k_hmc=0;

volatile struct V Gravity,Acc,Gyro,ex_acc_hmc,Mag_tran,Mag_hmc;
volatile struct V1 HmcMag_W,HmcMag_B;


float dt=0;
	
void GetAngle(const _st_Mpu *pMpu,_st_AngE *pAngE) 
{		
//	volatile struct V Gravity,Acc,Gyro,ex_acc_hmc,Mag_tran,Mag_hmc;
//	volatile struct V1 HmcMag_W,HmcMag_B;
		
	static  float KpDef = 0.5f ;
	static  float KiDef = 0.0001f;
//static  float KiDef = 0.00001f;
	
	float q0_t,q1_t,q2_t,q3_t;
  //float NormAcc;	
	float NormQuat; 
	float HalfTime = dt * 0.5f;
//	
//	//����������ϵ�µĵش����������������ݣ�ת��Ϊ�������ϵ
//	HmcMag_W.x = 2*pMpu->hmcX*(0.5-NumQ.q2*NumQ.q2-NumQ.q3*NumQ.q3)+2*pMpu->hmcY*(NumQ.q1*NumQ.q2-NumQ.q0*NumQ.q3)+2*pMpu->hmcZ*(NumQ.q1*NumQ.q3+NumQ.q0*NumQ.q2);
//	HmcMag_W.y = 2*pMpu->hmcX*(NumQ.q1*NumQ.q2+NumQ.q0*NumQ.q3)+2*pMpu->hmcY*(0.5-NumQ.q1*NumQ.q1-NumQ.q3*NumQ.q3)+2*pMpu->hmcZ*(NumQ.q2*NumQ.q3-NumQ.q0*NumQ.q1);
//	HmcMag_W.z = 2*pMpu->hmcX*(NumQ.q1*NumQ.q3-NumQ.q0*NumQ.q2)+2*pMpu->hmcY*(NumQ.q2*NumQ.q3+NumQ.q0*NumQ.q1)+2*pMpu->hmcZ*(0.5-NumQ.q1*NumQ.q1-NumQ.q2*NumQ.q2);
//	
//	int32_t LenXY_t = sqrt(HmcMag_W.x*HmcMag_W.x+HmcMag_W.y*HmcMag_W.y);//������ء�����ϵ�µĵش�����xyת�ɳ�ʼʱ(�ϵ�ʱ)�ķ���
//	HmcMag_W.x = LenXY_t*(Hmc_X0/Hmc_LenXY);
//	HmcMag_W.y = LenXY_t*(Hmc_Y0/Hmc_LenXY);
//	
//	//���������ϵ�µĵش�����ת�ػ�������ϵ
//	HmcMag_B.x = 2*HmcMag_W.x*(0.5-NumQ.q2*NumQ.q2-NumQ.q3*NumQ.q3)+2*HmcMag_W.y*(NumQ.q1*NumQ.q2+NumQ.q0*NumQ.q3)+2*HmcMag_W.z*(NumQ.q1*NumQ.q3-NumQ.q0*NumQ.q2);
//	HmcMag_B.y = 2*HmcMag_W.x*(NumQ.q1*NumQ.q2-NumQ.q0*NumQ.q3)+2*HmcMag_W.y*(0.5-NumQ.q1*NumQ.q1-NumQ.q3*NumQ.q3)+2*HmcMag_W.z*(NumQ.q2*NumQ.q3+NumQ.q0*NumQ.q1);
//	HmcMag_B.z = 2*HmcMag_W.x*(NumQ.q0*NumQ.q2+NumQ.q1*NumQ.q3)+2*HmcMag_W.y*(NumQ.q2*NumQ.q3-NumQ.q0*NumQ.q1)+2*HmcMag_W.z*(0.5-NumQ.q1*NumQ.q1-NumQ.q2*NumQ.q2);
//	
//	//�����ƹ�һ��
//	NormHmc = 1/sqrt(squa(HmcMag_B.x)+squa(HmcMag_B.y)+squa(HmcMag_B.z));
//	Mag_tran.x=HmcMag_B.x*NormHmc;
//	Mag_tran.y=HmcMag_B.y*NormHmc;
//	Mag_tran.z=HmcMag_B.z*NormHmc;
//	
//	NormHmc = 1/sqrt(squa(pMpu->hmcX)+squa(pMpu->hmcY)+squa(pMpu->hmcZ));
//	Mag_hmc.x=pMpu->hmcX*NormHmc;
//	Mag_hmc.y=pMpu->hmcY*NormHmc;
//	Mag_hmc.z=pMpu->hmcZ*NormHmc;



	//���������ϵ�µĳ�ʼ�ش�����ת�ػ�������ϵ
	Mag_tran.x = 2.0f*Hmc_X0*(0.5f-NumQ.q2*NumQ.q2-NumQ.q3*NumQ.q3)+2.0f*Hmc_Y0*(NumQ.q1*NumQ.q2+NumQ.q0*NumQ.q3)+2.0f*Hmc_Z0*(NumQ.q1*NumQ.q3-NumQ.q0*NumQ.q2);
	Mag_tran.y = 2.0f*Hmc_X0*(NumQ.q1*NumQ.q2-NumQ.q0*NumQ.q3)+2.0f*Hmc_Y0*(0.5f-NumQ.q1*NumQ.q1-NumQ.q3*NumQ.q3)+2.0f*Hmc_Z0*(NumQ.q2*NumQ.q3+NumQ.q0*NumQ.q1);
	Mag_tran.z = 2.0f*Hmc_X0*(NumQ.q0*NumQ.q2+NumQ.q1*NumQ.q3)+2.0f*Hmc_Y0*(NumQ.q2*NumQ.q3-NumQ.q0*NumQ.q1)+2.0f*Hmc_Z0*(0.5f-NumQ.q1*NumQ.q1-NumQ.q2*NumQ.q2);
	
	
	//����ǰ���hmc������һ��
	NormHmc = 1/sqrt(squa(pMpu->hmcX)+squa(pMpu->hmcY)+squa(pMpu->hmcZ));
	Mag_hmc.x=pMpu->hmcX*NormHmc;
	Mag_hmc.y=pMpu->hmcY*NormHmc;
	Mag_hmc.z=pMpu->hmcZ*NormHmc;

	
	//��ȡ��Ч��ת�����е��������� 
	Gravity.x = 2*(NumQ.q1 * NumQ.q3 - NumQ.q0 * NumQ.q2);								
	Gravity.y = 2*(NumQ.q0 * NumQ.q1 + NumQ.q2 * NumQ.q3);						  
	Gravity.z = 1-2*(NumQ.q1 * NumQ.q1 + NumQ.q2 * NumQ.q2);	
	// ���ٶȹ�һ��
	//printf("accX:%d\r\n",MPU6050.accX);
	NormAcc = 1/sqrt(squa(MPU6050.accX)+ squa(MPU6050.accY) +squa(MPU6050.accZ));
	//printf("NorAcc%f\r\n",NormAcc);
	//	NormAcc = Q_rsqrt(squa(MPU6050.accX)+ squa(MPU6050.accY) +squa(MPU6050.accZ));
	
  Acc.x = pMpu->accX * NormAcc;
  Acc.y = pMpu->accY * NormAcc;
  Acc.z = pMpu->accZ * NormAcc;
	
 	//������˵ó���ֵ
		ex_acc_hmc.x = k_acc*(Acc.y * Gravity.z - Acc.z * Gravity.y)+k_hmc*(Mag_hmc.y*Mag_tran.z - Mag_hmc.z*Mag_tran.y);
		ex_acc_hmc.y = k_acc*(Acc.z * Gravity.x - Acc.x * Gravity.z)+k_hmc*(Mag_hmc.z*Mag_tran.x - Mag_hmc.x*Mag_tran.z);
		ex_acc_hmc.z = k_acc*(Acc.x * Gravity.y - Acc.y * Gravity.x)+k_hmc*(Mag_hmc.x*Mag_tran.y - Mag_hmc.y*Mag_tran.x);
//	
//	ex_acc_hmc.x = (Mag_hmc.y*Mag_tran.z - Mag_hmc.z*Mag_tran.y);//ֻ�õشž���
//	ex_acc_hmc.y = k_acc*(Acc.z * Gravity.x - Acc.x * Gravity.z)+k_hmc*(Mag_hmc.z*Mag_tran.x - Mag_hmc.x*Mag_tran.z);
//	ex_acc_hmc.z = k_acc*(Acc.x * Gravity.y - Acc.y * Gravity.x)+k_hmc*(Mag_hmc.x*Mag_tran.y - Mag_hmc.y*Mag_tran.x);
	
	//�������ٶȻ��ֲ������ٶȵĲ���ֵ
  exInt.x += ex_acc_hmc.x * KiDef;
  exInt.y += ex_acc_hmc.y * KiDef;
  exInt.z += ex_acc_hmc.z * KiDef;
	
	//���ٶ��ںϼ��ٶȻ��ֲ���ֵ
  Gyro.x = pMpu->gyroX * Gyro_Gr + KpDef * ex_acc_hmc.x  +  exInt.x;//������
  Gyro.y = pMpu->gyroY * Gyro_Gr + KpDef * ex_acc_hmc.y  +  exInt.y;
  Gyro.z = pMpu->gyroZ * Gyro_Gr + KpDef * ex_acc_hmc.z  +  exInt.z;
	
	// һ�����������, ������Ԫ��
	q0_t = (-NumQ.q1*Gyro.x - NumQ.q2*Gyro.y - NumQ.q3*Gyro.z) * HalfTime;
	q1_t = ( NumQ.q0*Gyro.x - NumQ.q3*Gyro.y + NumQ.q2*Gyro.z) * HalfTime;
	q2_t = ( NumQ.q3*Gyro.x + NumQ.q0*Gyro.y - NumQ.q1*Gyro.z) * HalfTime;
	q3_t = (-NumQ.q2*Gyro.x + NumQ.q1*Gyro.y + NumQ.q0*Gyro.z) * HalfTime;
	
	NumQ.q0 += q0_t;
	NumQ.q1 += q1_t;
	NumQ.q2 += q2_t;
	NumQ.q3 += q3_t;
	// ��Ԫ����һ��
	NormQuat = 1/sqrt(squa(NumQ.q0) + squa(NumQ.q1) + squa(NumQ.q2) + squa(NumQ.q3));
	NumQ.q0 *= NormQuat;
	NumQ.q1 *= NormQuat;
	NumQ.q2 *= NormQuat;
	NumQ.q3 *= NormQuat;	
	
	// ��Ԫ��תŷ����
	{
		 
			#ifdef	YAW_GYRO
			pAngE->yaw= -atan2f(2 * NumQ.q1 *NumQ.q2 + 2 * NumQ.q0 * NumQ.q3, 1 - 2 * NumQ.q2 *NumQ.q2 - 2 * NumQ.q3 * NumQ.q3) * RtA;  //yaw����-��Ϊ�˸�������λ��ͳһ
			#else
				float yaw_G = pMpu->gyroZ * Gyro_G;
				if((yaw_G > 1.0f) || (yaw_G < -1.0f)) //����̫С������Ϊ�Ǹ��ţ�����ƫ������
				{
					pAngE->yaw  -= yaw_G * dt;
					//printf("Yaw:%f\r\n",pAngE->yaw);
				}
			#endif
			pAngE->pitch  =  -asin(2 * NumQ.q0 *NumQ.q2 - 2 * NumQ.q1 * NumQ.q3) * RtA;						
		
			pAngE->roll	= atan2(2 * NumQ.q2 *NumQ.q3 + 2 * NumQ.q0 * NumQ.q1, 1 - 2 * NumQ.q1 *NumQ.q1 - 2 * NumQ.q2 * NumQ.q2) * RtA;	//PITCH 			
			//printf("Pitch:%f;\r\n",pAngE->pitch);
			//printf("Roll:%f;\r\n",pAngE->roll);
	}
}

