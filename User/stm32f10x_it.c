/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_spi_nrf.h"
#include "pcf8563.h"
#include "AES.h"
#include "usart.h"
#include "key.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
		if (CoreDebug->DHCSR & 1)
		{  //check C_DEBUGEN == 1 -> Debugger Connected  
      __breakpoint(0);  // halt program execution here     
		}

  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void EXTI4_IRQHandler(void)
{
	extern	uint8_t ciphertext[AES_BLOCK_SIZE];
//	TIME key;
	u8 status;	//用于判断接收/发送状态
//	u8 txbuf[16];			 //接收缓冲
	u8 keyvalue=13;
		uint8_t ID_Num[10],mingwen1[16],mingwen2[16],miwen1[16],miwen2[16];
//	uint8_t key[10];
		uint8_t *syn,*seq,*ack,*fin,*time,*data;
uint8_t roundkeys[AES_ROUND_KEY_SIZE];
	u8 i;
		uint8_t frame[42]=
		{1,3,0,5,0,5,4,1,4,5,
									0,123,0,0,
									2,0,1,7,0,5,0,1,1,3,4,5,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		};
	struct tm *now_GPS,*now_pcf;
	TIME now;
	uint8_t key[] = {
		1, 3, 0, 5, 0, 5, 4, 1, 
		4, 5, 1, 2, 3, 4};
	syn=frame+10;seq=frame+11;
	ack=frame+12;fin=frame+13;
	time=frame+14;data=frame+26;

//	u8 rxbuf[4];			 //接收缓冲
//	u8 txbuf[]={'d','e','f','g'};	 //发送缓冲
//	key=PCF8563_GetTime();
//txbuf[0]=key.second;txbuf[1]=key.mint;txbuf[2]=key.hour;txbuf[3]=key.day;
	//			keyvalue=keyarray_Scan();
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //确保是否产生了EXTI Line中断
	{
		if(shakehand())
		{
			printf("输入验证码：\n");
			//	u8 i,keyvalue;
	//		printf("输入密码\n");
		for(i=0;i!=14;i++)
		{
			while(!keydown());
				keyvalue=keyarray_Scan();
			switch(keyvalue)
			{
				case 1:key[i]=keyvalue;printf("1");break;
				case 2:key[i]=keyvalue;printf("2");break;
				case 3:key[i]=keyvalue;printf("3");break;
				case 4:key[i]=keyvalue;printf("4");break;
				case 5:key[i]=keyvalue;printf("5");break;
				case 6:key[i]=keyvalue;printf("6");break;
				case 7:key[i]=keyvalue;printf("7");break;
				case 8:key[i]=keyvalue;printf("8");break;
				case 9:key[i]=keyvalue;printf("9");break;
				case 0:key[i]=0;printf("0");break;
				case 10:i-=2;break;
				default:break;
			}
		} 
		printf("key:");
		for(i=0;i<14;i++)
		{
			printf("%d",key[i]);
			frame[i+26]=key[i];
		}		
			now=PCF8563_GetTime();		
		/*****************year******************/
		*time=2;//2
		*(time+1)=now.year%1000/100;//0
		*(time+2)=now.year%100/10;//1
		*(time+3)=now.year%10;
		/********************month**************/
		*(time+4)=now.month/10;
		*(time+5)=now.month%10;
		/*******************day*****************/
		*(time+6)=now.day/10;
		*(time+7)=now.day%10;
		/******************hour****************/
		*(time+8)=now.hour/10;
		*(time+9)=now.hour%10;
		/*****************minute**************/
		*(time+10)=now.mint/10;
		*(time+11)=now.mint%10;
		/*****************second***************/
		*(time+12)=now.second/10;
		*(time+13)=now.second%10;
		for(i=0;i<16;i++)
		{
			mingwen1[i]=frame[i+10];
			mingwen2[i]=frame[i+26];
		}
			// encryption
		aes_encrypt_128(roundkeys, mingwen1, miwen1);//明文、密文、轮密钥
		aes_encrypt_128(roundkeys, mingwen2, miwen2);
		NRF_TX_Mode();
		status = NRF_Tx_Dat(ID_Num);//ID
		status = NRF_Tx_Dat(ID_Num+4);
		status = NRF_Tx_Dat(ID_Num+8);
		status = NRF_Tx_Dat(ID_Num+12);
		status = NRF_Tx_Dat(miwen1);
		status = NRF_Tx_Dat(miwen1+4);
		status = NRF_Tx_Dat(miwen1+8);
		status = NRF_Tx_Dat(miwen1+12);
		status = NRF_Tx_Dat(miwen2);
		status = NRF_Tx_Dat(miwen2+4);
		status = NRF_Tx_Dat(miwen2+8);
		status = NRF_Tx_Dat(miwen2+12);
		}
//		  while(1)
//			{
//				NRF_TX_Mode();
//			/*开始发送数据*/	
//	//		for(i=0;i<16;i++)
//				status = NRF_Tx_Dat(ciphertext);//每次只能发送前四个
//		//		NRF_RX_Mode();		status = NRF_Rx_Dat(rxbuf);
//				status = NRF_Tx_Dat(ciphertext+4);
//				status = NRF_Tx_Dat(ciphertext+8);
//				status = NRF_Tx_Dat(ciphertext+12);
//		//		printf("code in interrupt \n");		
//		//		for(i=0;i<16;i++)
//		//			printf("%2x ",ciphertext[i]);
//		//		printf("\n");
//				break;		
		//    NRF_RX_Mode();
		//    /*等待接收数据*/
		//    status = NRF_Rx_Dat(rxbuf);
		//    /*判断接收状态*/
		//    switch(status)
		//    {
		//      case RX_DR:
		//      for(i=0;i<4;i++)
		//      {					
		//        txbuf[i] =rxbuf[i];
		//      }
		//      break;
		//      case ERROR:
		// //       printf("\r\n 主机端 接收出错。   \r\n");
		//      break;  		
		//    }
  }// while(1)}

		EXTI_ClearITPendingBit(EXTI_Line4);     //清除中断标志位
} 


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
