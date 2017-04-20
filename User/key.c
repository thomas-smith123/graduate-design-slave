#include "key.h"
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
void Key2_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//…œ¿≠ ‰»Î
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}
u8 Key2_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON)
	{
		delay_ms(1);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON)
			return KEY_ON;
		else return KEY_OFF;
	}
	else return KEY_OFF;
}
u8 Key3_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON)
	{
		delay_ms(100);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON)
			return KEY_ON;
		else return KEY_OFF;
	}
	else return KEY_OFF;
}
