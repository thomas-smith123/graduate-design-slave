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
TIME now;
uint8_t ciphertext[AES_BLOCK_SIZE];
//uint8_t key[16];
int main(void)
{
			uint8_t i, r;
			delay_init();	
	uart_init(9600);	 //串口初始化为9600
//	uart3_init(9600);
			NVIC_Configuration_gps(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//		PCF8563_Init();
		delay_init();	
		SPI_NRF_Init();
//显示信息，可用
		/* TIM2 定时配置 */	
  TIM2_Configuration();
	Key2_GPIO_Config();
	keyarry_GPIO_init();
	/* 实战定时器的中断优先级 */
	TIM2_NVIC_Configuration();
	
  /*检测NRF模块与MCU的连接*/
		status1 = NRF_Check(); 
  if(status1 == SUCCESS)	   
	{
		printf("\r\n   NRF与MCU连接成功\r\n"); 
//		LCD_DispStr(10, 10, (uint8_t *)"SUCCESSFULLY CONNECTED!", RED);	
	} 
  else	  
	{
		printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");
//		LCD_DispStr(10, 10, (uint8_t *)"CHECKING CONNECTED SITUATION...", RED);	
	}

	printf("Welcome to use!\r\n");
	while(1)
	{
		printGpsBuffer();
		parseGpsBuffer();
		now=PCF8563_GetTime();
		if(now.second%10==0)
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
			break;
		}
	}
	while(1)	
		{	
		CLI();
		SEI();	

			if(Key2_Scan(GPIOE,GPIO_Pin_4)==KEY_ON)
			{
				if(shakehand())
						printf("\n unlock！！ \n");
			}
		};
	
/*********************************************END OF FILE**********************/
}
