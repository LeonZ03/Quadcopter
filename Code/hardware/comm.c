#include "main.h"

extern int16_t mpu6050[7];
extern int16_t hmc5883l[3];



//传递整个数组
void my_usart_transmit(USART_TypeDef* USARTx,int8_t *addr,int16_t len){
	
	USART_ClearFlag(USARTx,USART_FLAG_TC);		
	for(int i=1;i<=len;i++){
		USART_SendData(USARTx, *(addr++));
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);
		//USART_ClearFlag(USARTx,USART_FLAG_TC);	
	}
	
}

void USART_SendString(USART_TypeDef* USARTx, char *DataString)
{
	int i = 0;
	USART_ClearFlag(USARTx,USART_FLAG_TC);										//发送字符前清空标志位（否则缺失字符串的第一个字符）
	while(DataString[i] != '\0')												//字符串结束符
	{
		USART_SendData(USARTx,DataString[i]);									//每次发送字符串的一个字符
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//等待数据发送成功
		USART_ClearFlag(USARTx,USART_FLAG_TC);									//发送字符后清空标志位
		i++;
	}
}


//#define Agreement2 //协议
//#define Agreement1
//_st_ALL_flag ALL_flag;
void Send_ANOTC()
{
	//#ifdef Agreement2
//	uint8_t agreement=2;
//	
//	if(agreement==3){

//	uint8_t i;
//	uint8_t len=18;							
//	int16_t Anto[11];
//	int8_t* pt=(int8_t*)Anto;
//	
//	int16_t acc_x,acc_y,acc_z,gyr_x,gyr_y,gyr_z,mag_x,mag_y,mag_z;
//	acc_x=mpu6050[0];
//	acc_y=mpu6050[1];
//	acc_z=mpu6050[2];
//	gyr_x=mpu6050[4];
//	gyr_y=mpu6050[5];
//	gyr_z=mpu6050[6];
//	mag_x=hmc5883l[0];
//	mag_y=hmc5883l[1];
//	mag_z=hmc5883l[2];

//	Anto[0]=0xAAAA;
//	Anto[1]=0x02<<8 | 18;

//		
//	
//	Anto[2]=acc_x;
//	Anto[3]=acc_y;
//	Anto[4]=acc_z;
//	Anto[5]=gyr_x;
//	Anto[6]=gyr_y;
//	Anto[7]=gyr_z;
//	Anto[8]=mag_x;
//	Anto[9]=mag_y;
//	Anto[10]=mag_z;
//	
//	pt[len+4] = (int8_t)(0xAA+0xAA);//校验位
//	
//		for(i=2;i<len+4;i+=2)    //a swap with b;
//	{
//		pt[i] ^= pt[i+1];
//		pt[i+1] ^= pt[i];
//		pt[i] ^= pt[i+1];
//		pt[len+4] += pt[i] + pt[i+1];
//	}
//	
//	

//	my_usart_transmit(USART1,pt,len+5);
//	}
//	
	
	
	
	
	
	
	//agreement1
	
	uint8_t i;
	uint8_t len=0;							
	int16_t Anto[20];
	int8_t *pt = (int8_t*)(Anto);		
	
	Anto[2] = (int16_t)(Angle.roll*100);
	Anto[3] = (int16_t)(Angle.pitch*100);
	Anto[4] = (int16_t)(Angle.yaw*100);
	Anto[5] = 0;//没有高度数据
	Anto[6] = 0;
	//Anto[7] = ALL_flag.unlock<<8;//解锁信息
	Anto[7] = 1<<8;
	len = 12;						//数据长度
	
	Anto[0] = 0XAAAA;
	Anto[1] = len | 0x01<<8;
	pt[len+4] = (int8_t)(0xAA+0xAA);
	
	
	for(i=2;i<len+4;i+=2)    //a swap with b;
	{
		pt[i] ^= pt[i+1];
		pt[i+1] ^= pt[i];
		pt[i] ^= pt[i+1];
		pt[len+4] += pt[i] + pt[i+1];
	}
	
	my_usart_transmit(USART1,pt,len+5);
	
	
	
	
	
	
	//User_data
	Anto[0] = 0XAAAA;
	len=12;
	Anto[1]=len|0xf1<<8;
	Anto[2]=(int16_t)In_r.exp*100;
	Anto[3]=(int16_t)In_r.cur*100;
	
	Anto[4]=(int16_t)In_r.exp*100;
	Anto[5]=(int16_t)In_r.cur*100;
	
//	Anto[4]=(int16_t)In_p.exp*100;
//	Anto[5]=(int16_t)In_p.cur*100;
	
	Anto[6]=(int16_t)In_y.exp*100;
	Anto[7]=(int16_t)In_y.cur*100;
	
	pt[len+4] = (int8_t)(0xAA+0xAA);
	for(i=2;i<len+4;i+=2)    //a swap with b;
	{
		pt[i] ^= pt[i+1];
		pt[i+1] ^= pt[i];
		pt[i] ^= pt[i+1];
		pt[len+4] += pt[i] + pt[i+1];
	}
	
	//my_usart_transmit(USART1,pt,len+5);
	
}

//extern pid_struct In_r;
int circle;//1外环，2内环
int angle;//1:pitch ,2:roll , 3:yaw
float Kp_temp,Ki_temp,Kd_temp;

char repeat_buf[30];
void PID_DataHandle(){
	
//	int circle;//1外环，2内环
//	int angle;//1:pitch ,2:roll , 3:yaw
//	float Kp_temp,Ki_temp,Kd_temp;
	
	sscanf((char*)USART_RX_BUF,"PID:%d:%d:Kp=%f,Ki=%f,Kd=%f",&circle,&angle,&Kp_temp,&Ki_temp,&Kd_temp);
	
	if(circle==1){//外环
		if(angle==1){
			PID_StructInit(&Out_p);
			
			Out_p.Kp = Kp_temp;
			Out_p.Ki = Ki_temp;
			Out_p.Kd = Kd_temp;
			
			sprintf(repeat_buf,"Successful!\n");
			BlueTooth_SendString(repeat_buf);
		}
		else if(angle==2){
			PID_StructInit(&Out_r);
			
			Out_r.Kp = Kp_temp;
			Out_r.Ki = Ki_temp;
			Out_r.Kd = Kd_temp;
					
			sprintf(repeat_buf,"Successful!\n");
			BlueTooth_SendString(repeat_buf);
			
//			sprintf(repeat_buf,"OutRoll_Kp=%f,OutRoll_Ki=%f,OutRoll_Kd=%f\n",Out_r.Kp,Out_r.Ki,Out_r.Kd);
//			BlueTooth_SendString(repeat_buf);
		}
		else if(angle==3){
			PID_StructInit(&Out_y);
			
			Out_y.Kp = Kp_temp;
			Out_y.Ki = Ki_temp;
			Out_y.Kd = Kd_temp;
					
			sprintf(repeat_buf,"Successful!\n");
			BlueTooth_SendString(repeat_buf);
			
//			sprintf(repeat_buf,"OutYaw_Kp=%f,OutYaw_Ki=%f,OutYaw_Kd=%f\n",Out_y.Kp,Out_y.Ki,Out_y.Kd);
//			BlueTooth_SendString(repeat_buf);
		}
	
	}
	
	else if(circle==2){
		if(angle==1){
			PID_StructInit(&In_p);

			In_p.Kp = Kp_temp;
			In_p.Ki = Ki_temp;
			In_p.Kd = Kd_temp;
					
			sprintf(repeat_buf,"Successful!\n");
			BlueTooth_SendString(repeat_buf);
			
//			sprintf(repeat_buf,"InPitch_Kp=%f,InPitch_Ki=%f,InPitch_Kd=%f\n",In_p.Kp,In_p.Ki,In_p.Kd);
//			BlueTooth_SendString(repeat_buf);
		}
		else if(angle==2){
			PID_StructInit(&In_r);
			In_r.Kp = Kp_temp;
			In_r.Ki = Ki_temp;
			In_r.Kd = Kd_temp;
			
					
			sprintf(repeat_buf,"Successful!\n");
			BlueTooth_SendString(repeat_buf);

//			
//			sprintf(repeat_buf,"InRoll_Kp=%f,InRoll_Ki=%f,InRoll_Kd=%f\n",In_r.Kp,In_r.Ki,In_r.Kd);
//			BlueTooth_SendString(repeat_buf);
		}
		else if(angle==3){
			PID_StructInit(&In_y);

			In_y.Kp = Kp_temp;
			In_y.Ki = Ki_temp;
			In_y.Kd = Kd_temp;
					
			sprintf(repeat_buf,"Successful!\n");
			BlueTooth_SendString(repeat_buf);
			
//			sprintf(repeat_buf,"InYaw_Kp=%f,InYaw_Ki=%f,InYaw_Kd=%f\n",In_y.Kp,In_y.Ki,In_y.Kd);
//			BlueTooth_SendString(repeat_buf);
		}
	}
	
	
	
}
