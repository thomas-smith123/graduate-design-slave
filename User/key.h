#include "stm32f10x.h"
#ifndef __key_H
#define __key_H	
#define KEY_ON	0
#define KEY_OFF	1
void Key2_GPIO_Config(void);
u8 Key2_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);
u8 Key3_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);
void keyarry_GPIO_init(void);
u8 keyarray_Read(void);
u8 keyarray_Scan(void);
u8 keydown(void);
void verify_in(void);
#endif

