

#ifndef __comm_H
#define __comm_H	 

#include "stm32f4xx.h"

extern int circle;//1外环，2内环
extern int angle;//1:pitch ,2:roll , 3:yaw
extern float Kp_temp,Ki_temp,Kd_temp;


void Send_ANOTC(void);
void my_usart_transmit(USART_TypeDef* USARTx,int8_t *addr,int16_t len);
void USART_SendString(USART_TypeDef* USARTx, char *DataString);
void PID_DataHandle(void);


#endif

