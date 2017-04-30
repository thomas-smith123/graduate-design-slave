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
	TIME key;
	u8 status;	//用于判断接收/发送状态
	u8 rxbuf[4];			 //接收缓冲
//	u8 txbuf[4];			 //接收缓冲
//	u8 txbuf[]={'d','e','f','g'};	 //发送缓冲
	int i=0;
//	key=PCF8563_GetTime();
//txbuf[0]=key.second;txbuf[1]=key.mint;txbuf[2]=key.hour;txbuf[3]=key.day;
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //确保是否产生了EXTI Line中断
	{
		shakehand();
		  while(1)
			{
				NRF_TX_Mode();
			/*开始发送数据*/	
	//		for(i=0;i<16;i++)
				status = NRF_Tx_Dat(ciphertext);//每次只能发送前四个
		//		NRF_RX_Mode();		status = NRF_Rx_Dat(rxbuf);
				status = NRF_Tx_Dat(ciphertext+4);
				status = NRF_Tx_Dat(ciphertext+8);
				status = NRF_Tx_Dat(ciphertext+12);
		//		printf("code in interrupt \n");		
		//		for(i=0;i<16;i++)
		//			printf("%2x ",ciphertext[i]);
		//		printf("\n");
				break;		
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
