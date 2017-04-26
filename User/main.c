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
int main(void)
{
			uint8_t i, r;

	/* 128 bit key */
/*	uint8_t key[] = {
		0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59, 
		0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98};
//	uint8_t plaintext[16] = {
//		0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
//		0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
	uint8_t plaintext[16] = "1305054145jrd";
	const uint8_t const_cipher[AES_BLOCK_SIZE] = {
		0xff, 0x0b, 0x84, 0x4a, 0x08, 0x53, 0xbf, 0x7c,
		0x69, 0x34, 0xab, 0x43, 0x64, 0x14, 0x8f, 0xb9};	
	uint8_t roundkeys[AES_ROUND_KEY_SIZE];
*/
		KeyExti_Config();
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
		
		CLI();
		SEI();		
  /*���NRFģ����MCU������*/
		status1 = NRF_Check(); 
	// key schedule
/*	aes_key_schedule_128(key, roundkeys);//��Կ����
	// encryption
	aes_encrypt_128(roundkeys, plaintext, ciphertext);//���ġ����ġ�����Կ
	for (i = 0; i < AES_BLOCK_SIZE; i++) {
		if ( ciphertext[i] != const_cipher[i] ) { break; }
	}
	// decryption
//	aes_decrypt_128(roundkeys, ciphertext, ciphertext);

//	for (i = 0; i < AES_BLOCK_SIZE; i++) {
//		if ( ciphertext[i] != plaintext[i] ) { break; }
//	}
*/
	printf("Welcome to use!\r\n");
//	for(i=0;i<16;i++)
//		printf("%2x ",ciphertext[i]);
//	printf("\n");
	while(1)	
		{		
			parseGpsBuffer();
			printGpsBuffer();
		};
	
  /*�ж�����״̬*/  
//  while(1)
//  {
//    NRF_TX_Mode();
//    /*��ʼ��������*/	
//    status = NRF_Tx_Dat(txbuf);	  
//    NRF_RX_Mode();
//    /*�ȴ���������*/
//    status = NRF_Rx_Dat(rxbuf);
//    /*�жϽ���״̬*/
//    switch(status)
//    {
//      case RX_DR:
//      for(i=0;i<4;i++)
//      {					
//        txbuf[i] =rxbuf[i];
//      }
//      break;
//      case ERROR:
// //       printf("\r\n ������ ���ճ���   \r\n");
//      break;  		
//    }
 // }// while(1)}
/*********************************************END OF FILE**********************/
}
