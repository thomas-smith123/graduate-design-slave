/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ��3.5.0�汾�⽨�Ĺ���ģ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
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
#define CLI() __set_PRIMASK(1)		/* �ر����ж� */  
#define SEI() __set_PRIMASK(0)				/* �������ж� */ 
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
u8 status1;	//�����жϽ���/����״̬

extern TIME today;
TIME now;
uint8_t ciphertext[AES_BLOCK_SIZE];
//uint8_t key[16];
int main(void)
{
			uint8_t i, r;
			delay_init();	
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
//	uart3_init(9600);
			NVIC_Configuration_gps(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//		PCF8563_Init();
		delay_init();	
		SPI_NRF_Init();
//��ʾ��Ϣ������
		/* TIM2 ��ʱ���� */	
  TIM2_Configuration();
	Key2_GPIO_Config();
	keyarry_GPIO_init();
	/* ʵս��ʱ�����ж����ȼ� */
	TIM2_NVIC_Configuration();
	
  /*���NRFģ����MCU������*/
		status1 = NRF_Check(); 
  if(status1 == SUCCESS)	   
	{
		printf("\r\n   NRF��MCU���ӳɹ�\r\n"); 
//		LCD_DispStr(10, 10, (uint8_t *)"SUCCESSFULLY CONNECTED!", RED);	
	} 
  else	  
	{
		printf("\r\n   ���ڼ��NRF��MCU�Ƿ��������ӡ�����\r\n");
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
						printf("\n unlock���� \n");
			}
		};
	
/*********************************************END OF FILE**********************/
}
