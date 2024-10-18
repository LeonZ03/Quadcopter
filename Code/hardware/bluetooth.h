#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H	 

//#include "stm32f4xx.h"
//#include "app_cfg.h"
//#include "sys_cfg.h"
//#include "ucos_ii.h"
//#include "task_start.h"
//#include "led.h"
//#include "systick.h"
//#include "task_led.h"
//#include "task_start.h"
//#include "gy86.h"
//#include "bluetooth.h"
#include "main.h"



extern u8 USART_RX_BUF[];
extern u16 USART_RX_STA;


void BlueTooth_Init(void);
void BlueTooth_SendByte(uint8_t Byte);
uint32_t BlueTooth_Pow(uint32_t X, uint32_t Y);
void BlueTooth_SendData(uint32_t Number);
void BlueTooth_SendString(char* str);


#endif

