#include "main.h"

/***
 * �������� :scan_task(void *pdata);
 *
 * �������� : ɨ��������OLED����ʾ���ջ���ͨ����GY-86����;
 *
 * ����ֵ	  : ��;
 *
 * ����ֵ   : ��;
 *
 **/


//���������ջ�ռ�
OS_STK SCAN_TASK_STK[SCAN_STK_SIZE];

//u8 scan_flag;
void scan_task(void *pdata){
//	scan_flag=0;
//	
//	char temp[20];//��ʱ�洢gy-86������
//	while(1){
//		

//		EventStartC(0);
//		OSTimeDly(10);
//		EventStopC(0);
//		
//		EventStartC(1);
//		//OLED_Clear();
//		if(scan_flag==0){//��ʾ���ջ�
//			
////			OLED_ShowNum(1,1,ch[0],8);
////			OLED_ShowNum(2,1,ch[1],8);
////			OLED_ShowNum(3,1,ch[2],8);
////			OLED_ShowNum(4,1,ch[3],8);
//			
//			sprintf(temp,"%d               ",ch[0]);
//			OLED_ShowString(1,1,temp);
//			
//			sprintf(temp,"%d           ",ch[1]);
//			OLED_ShowString(2,1,temp);
//			
//			sprintf(temp,"%d           ",ch[2]);
//			OLED_ShowString(3,1,temp);
//			
//			sprintf(temp,"%d           ",ch[3]);
//			OLED_ShowString(4,1,temp);

//		}
//		else if(scan_flag==1){//��ʾMPU6050
//			OLED_ShowString(1,1,"MPU6050        ");
//			
//			sprintf(temp,"%d  %d    ",mpu6050[0],mpu6050[4]);
//			OLED_ShowString(2,1,temp);
//			
//			sprintf(temp,"%d  %d    ",mpu6050[1],mpu6050[5]);
//			OLED_ShowString(3,1,temp);
//			
//			sprintf(temp,"%d  %d    ",mpu6050[2],mpu6050[6]);
//			OLED_ShowString(4,1,temp);

//		}
//		else {//��ʾHMC5883L
//			OLED_ShowString(1,1,"HMC5883L   temp");
//			
//			//double tem=mpu6050[3]/340+36.53;
//			//double tem=0.0000;
//			float a=1.0f;
//			
//			sprintf(temp,"%d      %d    ",hmc5883l[0],mpu6050[3]);
//			OLED_ShowString(2,1,temp);
//			
//			sprintf(temp,"%d         ",hmc5883l[1]);
//			OLED_ShowString(3,1,temp);
//			
//			sprintf(temp,"%d         ",hmc5883l[2]);
//			OLED_ShowString(4,1,temp);

//		}
//		
//		EventStopC(1);
//	}
}


