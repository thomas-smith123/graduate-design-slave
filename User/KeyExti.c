//#include "stm32f10x_exti.h"
//#include "KeyExti.h"
//static void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;  
//  /* Configure one bit for preemption priority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
//  /* �����ж�Դ PE4*/
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}
//void KeyExti_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure; 
//	EXTI_InitTypeDef EXTI_InitStructure;
//	/* config the extiline clock and AFIO clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO,ENABLE);												
//	/* config the NVIC */
//	NVIC_Configuration();
//	/* EXTI line gpio config*/	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;       
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
//  GPIO_Init(GPIOE, &GPIO_InitStructure);
//	/* EXTI line mode config */
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4); 
//  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure); 
//}
///*********************************************END OF FILE**********************/
