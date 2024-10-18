#include "main.h"



/***
 * 函数名称 :attitude_task(void *pdata);
 *
 * 函数描述 : 姿态解算，通过蓝牙传递数据到手机
 *
 * 传递值	  : 无;
 *
 * 返回值   : 无;
 *
 **/


//创建任务堆栈空间
OS_STK ATTITUDE_TASK_STK[ATTITUDE_STK_SIZE];



void attitude_task(void *pdata){
//	
//		while(1)
//			{
//				EventStartB(0);
//				OSTimeDly(5);
//				EventStopB(0);
//				
//				EventStartB(1);
//				MPU6050_Read(mpu6050);//读出MPU6050数据
//				HMC5883L_Read(hmc5883l);//读出hmc5883l数据
//				BlueTooth_SendString("mpu6050:");
//				for(int i=0;i<6;i++)
//				{
//					if(i!=3)
//					{
//					BlueTooth_SendData(mpu6050[i]);
//					BlueTooth_SendByte(' ');
//					}
//				}
//				BlueTooth_SendString("                                                        ");
//				BlueTooth_SendString("hmc5883l:");
//				for(int i=0;i<3;i++)
//				{
//					BlueTooth_SendData(hmc5883l[i]);
//					BlueTooth_SendByte(' ');
//					
//				}
//				BlueTooth_SendString("                                                        ");
//				
//				EventStopB(1);
//			}
}

