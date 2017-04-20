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
	u8 status;	//�����жϽ���/����״̬
	u8 rxbuf[4];			 //���ջ���
//	u8 txbuf[4];			 //���ջ���
//	u8 txbuf[]={'d','e','f','g'};	 //���ͻ���
	int i=0;
//	key=PCF8563_GetTime();
//txbuf[0]=key.second;txbuf[1]=key.mint;txbuf[2]=key.hour;txbuf[3]=key.day;
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		  while(1)
  {
    NRF_TX_Mode();
    /*��ʼ��������*/	
//		for(i=0;i<16;i++)
    status = NRF_Tx_Dat(ciphertext);//ÿ��ֻ�ܷ���ǰ�ĸ�
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
  }// while(1)}

		EXTI_ClearITPendingBit(EXTI_Line4);     //����жϱ�־λ
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
