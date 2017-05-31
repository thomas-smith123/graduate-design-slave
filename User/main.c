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
uint8_t ciphertext[AES_BLOCK_SIZE];
//uint8_t key[16];
int main(void)
{
			uint8_t i, r;
//		KeyExti_Config();
			delay_init();	
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
			NVIC_Configuration_gps(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
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
//��ʾ��Ϣ������
		OLED_ShowNum(0,0,2,1,8);
		OLED_ShowCHinese(0,0,13);
		OLED_ShowCHinese(18,0,14);
		OLED_ShowCHinese(36,0,15);
		OLED_ShowCHinese(54,0,16);
		OLED_ShowCHinese(72,0,17);
		OLED_ShowCHinese(90,0,18);
		/* TIM2 ��ʱ���� */	
  TIM2_Configuration();
	
	/* ʵս��ʱ�����ж����ȼ� */
	TIM2_NVIC_Configuration();

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
  /*���NRFģ����MCU������*/
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
						printf("\n unlock���� \n");
			}
//			if(time==5000)
//			{printf("123\n");time=0;}
		};
	
/*********************************************END OF FILE**********************/
}
