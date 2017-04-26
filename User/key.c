#include "key.h"
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
void Key2_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
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
void keyarry_GPIO_init(void)
{
	/*****PE9,PE10,PE11****/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	/***********PE12,PE13,PE14,PE15*******/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入
	GPIO_Init(GPIOE,&GPIO_InitStructure);	
	GPIO_SetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	GPIO_ResetBits(GPIOE,GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_8
	|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0);
}
u8 keyarray_Read()
{
	unsigned int KeyValue=13;
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
	GPIO_ResetBits(GPIOE,GPIO_Pin_10|GPIO_Pin_11);
	switch(GPIO_ReadInputData(GPIOE)&0xf000)// 9,10,11,12,13,14,15
	{
		case 0x1000:KeyValue=0;break;
		case 0x2000:KeyValue=7;break;
		case 0x4000:KeyValue=4;break;
		case 0x8000:KeyValue=1;break;
		default:break;
	}
	GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_11);
	switch(GPIO_ReadInputData(GPIOE)&0xf000)// 9,10,11,12,13,14,15
	{
		case 0x2000:KeyValue=8;break;
		case 0x4000:KeyValue=5;break;
		case 0x8000:KeyValue=2;break;
		default:break;
	}	
		GPIO_SetBits(GPIOE,GPIO_Pin_11);
	GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_10);
	switch(GPIO_ReadInputData(GPIOE)&0xf000)// 9,10,11,12,13,14,15
	{
		case 0x1000:KeyValue=11;break;
		case 0x2000:KeyValue=9;break;
		case 0x4000:KeyValue=6;break;
		case 0x8000:KeyValue=3;break;
		default:break;
	}	
//	GPIO_SetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	while(!keydown());//按键松开
	return KeyValue;
}
u8 keydown(void)
{
	//PB9到PB11全为高
	GPIO_Write(GPIOE,0x0e00);
	if((GPIO_ReadInputData(GPIOE)&0xf000)!=0x0000)
		return 1;
	else 
		return 0;
}
u8 keyarray_Scan()
{
	u8 u;
	keyarry_GPIO_init();
	if(keydown())
	{
		delay_ms(20);
		if(keydown())
			u=keyarray_Read();
			return  u;
	}
	return 0;	
}
