#include "main.h"
#include "OLED_Font.h"

/*引脚配置*/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(x))

/*引脚初始化*/
void OLED_I2C_Init(void)
{
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
 	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}



void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}



void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}


void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}



void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}



void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}



void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置低4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置高4位
}



void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
		
	}
}


void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}



void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}



uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}


void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}



void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}



void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}



void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}



void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置？？此步存疑
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别？？此步存疑
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//清屏
}

uint8_t clear_flag=0;
void OLED_MyClear(void){
	char s[20];
	sprintf(s,"                 ");
	OLED_ShowString(1,1,s);
	OLED_ShowString(2,1,s);
	OLED_ShowString(3,1,s);
	OLED_ShowString(4,1,s);
}

void OLED_Show(){
	
		
		if(clear_flag==1){
			OLED_MyClear();
			clear_flag=0;
		}
		
		char temp[20];//临时存储字符串
		

				if(scan_flag==0){//显示接收机
//					
//					OLED_ShowNum(1,1,ch[0],8);
//					OLED_ShowNum(2,1,ch[1],8);
//					OLED_ShowNum(3,1,ch[2],8);
//					OLED_ShowNum(4,1,ch[3],8);
					
//					sprintf(temp,"%d            ",ch[0]);
//					OLED_ShowString(1,1,temp);
//					
//					sprintf(temp,"%d            ",ch[1]);
//					OLED_ShowString(2,1,temp);
//					
//					sprintf(temp,"%d            ",ch[2]);
//					OLED_ShowString(3,1,temp);
//					
//					sprintf(temp,"%d            ",ch[3]);
//					OLED_ShowString(4,1,temp);
//					
				
					
					
					
					
					sprintf(temp,"pitch roll yaw ");
					OLED_ShowString(1,1,temp);
					
					sprintf(temp,"%f",Angle.pitch);
					OLED_ShowString(2,1,temp);
					
					sprintf(temp,"%f",Angle.roll);
					OLED_ShowString(3,1,temp);
					
					sprintf(temp,"%f",Angle.yaw);
					OLED_ShowString(4,1,temp);

//			
//					sprintf(temp,"exp:p r y");
//					OLED_ShowString(1,1,temp);
//					
//					sprintf(temp,"%f      ",pitch_exp);
//					OLED_ShowString(2,1,temp);
//					
//					sprintf(temp,"%f",roll_exp);
//					OLED_ShowString(3,1,temp);
//					
//					sprintf(temp,"%f",yaw_exp);
//					OLED_ShowString(4,1,temp);

				}
				else if(scan_flag==1){//显示MPU6050
//					sprintf(temp,"ACC      GYR  ");
//					OLED_ShowString(1,1,temp);
//					
//					sprintf(temp,"%d   ",mpu6050[0]);
//					OLED_ShowString(2,1,temp);
//					sprintf(temp,"%d   ",mpu6050[4]);
//					OLED_ShowString(2,10,temp);
//					
//					sprintf(temp,"%d   ",mpu6050[1]);
//					OLED_ShowString(3,1,temp);
//					sprintf(temp,"%d   ",mpu6050[5]);
//					OLED_ShowString(3,10,temp);
//					
//					sprintf(temp,"%d   ",mpu6050[2]-28000+16384-50);
//					OLED_ShowString(4,1,temp);
//					sprintf(temp,"%d   ",mpu6050[6]);
//					OLED_ShowString(4,10,temp);
					
					sprintf(temp,"ACC(g)  GYR(Rad)");
					OLED_ShowString(1,1,temp);
					
					sprintf(temp,"%.4f   ",Acc.x);
					OLED_ShowString(2,1,temp);
					sprintf(temp," %.4f ",Gyro.x);
					OLED_ShowString(2,8,temp);
					
					sprintf(temp,"%.4f   ",Acc.y);
					OLED_ShowString(3,1,temp);
					sprintf(temp," %.4f ",Gyro.y);
					OLED_ShowString(3,8,temp);
					
					sprintf(temp,"%.4f   ",Acc.z);
					OLED_ShowString(4,1,temp);
					sprintf(temp," %.4f ",Gyro.z);
					OLED_ShowString(4,8,temp);
					
//					
//					
//					OLED_ShowNum(1,1,ch[4],8);
//					OLED_ShowNum(2,1,ch[5],8);
//					OLED_ShowNum(3,1,ch[6],8);
//					OLED_ShowNum(4,1,ch[7],8);
		

				}
				else if(scan_flag==2) {//显示HMC5883L
//					OLED_ShowString(1,1,"HMC5883L   temp");
//					
//					double tem=mpu6050[3]/340.0+36.53;


//				
//					sprintf(temp,"%d       %.2f",hmc5883l[0]+500,tem);
//					OLED_ShowString(2,1,temp);
//					
//					sprintf(temp,"%d          ",hmc5883l[1]);
//					OLED_ShowString(3,1,temp);
//					
//					sprintf(temp,"%d          ",hmc5883l[2]);
//					OLED_ShowString(4,1,temp);
//					
//					
					
					
						
					sprintf(temp,"PID_In_roll ");
					OLED_ShowString(1,1,temp);
					
					sprintf(temp,"Kp=%g   ",In_r.Kp);
					OLED_ShowString(2,1,temp);
					
					sprintf(temp,"Ki=%g   ",In_r.Ki);
					OLED_ShowString(3,1,temp);
					
					sprintf(temp,"Kd=%g   ",In_r.Kd);
					OLED_ShowString(4,1,temp);
					

				}
				else{
						
					sprintf(temp,"PID_Out_Roll ");
					OLED_ShowString(1,1,temp);
					
					sprintf(temp,"Kp=%g   ",Out_r.Kp);
					OLED_ShowString(2,1,temp);
					
					sprintf(temp,"Ki=%g   ",Out_r.Ki);
					OLED_ShowString(3,1,temp);
					
					sprintf(temp,"Kd=%g   ",Out_r.Kd);
					OLED_ShowString(4,1,temp);
					
				}
				
				
}
