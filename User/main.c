/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   用3.5.0版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */  
#include "stm32f10x.h"
#include "bsp_spi_nrf.h"
#include "pcf8563.h"
#include "oled.h"
#include "KeyExti.h"
#include "key.h"
#include "delay.h"
#include "AES.h"
#include "usart.h"
#include "GPS.h"
#include "bsp_Timebase.h"
#include "sys.h"
#define CLI() __set_PRIMASK(1)		/* 关闭总中断 */  
#define SEI() __set_PRIMASK(0)				/* 开放总中断 */ 
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
u8 status1;	//用于判断接收/发送状态

extern TIME today;
uint8_t ciphertext[AES_BLOCK_SIZE];
//uint8_t key[16];
int main(void)
{
			uint8_t i, r;
//		KeyExti_Config();
			delay_init();	
	uart_init(9600);	 //串口初始化为9600
			NVIC_Configuration_gps(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//		PCF8563_Init();
		delay_init();	
		SPI_NRF_Init();
		OLED_Init();
		today.day=9;		today.hour=11;		
		today.mint=20;		today.month=1;	
		today.second=0;	
		today.year=17;
	PCF_SetTime(today.year,today.month,today.day,today.hour,today.mint,today.second);
//	KeyExti_Config();
//显示信息，可用
		OLED_ShowNum(0,0,2,1,8);
		OLED_ShowCHinese(0,0,13);
		OLED_ShowCHinese(18,0,14);
		OLED_ShowCHinese(36,0,15);
		OLED_ShowCHinese(54,0,16);
		OLED_ShowCHinese(72,0,17);
		OLED_ShowCHinese(90,0,18);
		/* TIM2 定时配置 */	
  TIM2_Configuration();
	
	/* 实战定时器的中断优先级 */
	TIM2_NVIC_Configuration();

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
  /*检测NRF模块与MCU的连接*/
		status1 = NRF_Check(); 

	printf("Welcome to use!\r\n");
//	for(i=0;i<16;i++)
//		printf("%2x ",ciphertext[i]);
//	printf("\n");
	while(1)	
		{	
		CLI();
		SEI();	
	//	GPIO_SetBits(GPIOE,GPIO_Pin_4);	
			parseGpsBuffer();
			printGpsBuffer();
			if(!Key2_Scan(GPIOE,4))
			{
				if(shakehand())
						printf("\n unlock！！ \n");
			}
//			if(time==5000)
//			{printf("123\n");time=0;}
		};
	
/*********************************************END OF FILE**********************/
}
