#ifndef _LED_H
#define _LED_H

#define led_up (GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET))
#define led_down (GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET))


void led_init(void);
//void led_up(void);
//void led_down(void);
void led_breath(void);
void my_delay_ms(int time);

#endif

