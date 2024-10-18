#ifndef __iic_H
#define __iic_H	


#include "main.h"



void I2C_Configuration(void);
uint8_t MY_CHECK_EVENT(uint16_t SR1,uint16_t SR2);
void I2C_SEND_BYTE(uint8_t SlaveAddr,uint8_t WriteAddr,uint8_t Data);
uint8_t I2C_READ_BYTE(uint8_t SlaveAddr,uint8_t ReadAddr);
uint8_t I2C_READ_BYTE_LEN(uint8_t SlaveAddr,uint8_t ReadAddr,uint8_t* buf,uint8_t len);

#endif

