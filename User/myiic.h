#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//��?3��D��??1??��?�㨺1��?��??��?-����??D��?����?2?��?��?����???����?o?��?��?
//ALIENTEK???��STM32?a�����?
//IIC?y?�� �䨲??	   
//?y��??-����@ALIENTEK
//??��???��3:www.openedv.com
//DT??��??��:2012/9/9
//��?��?��oV1.0
//��?����?����D��?�̨���?��????��
//Copyright(C) 1??Y��DD?������?����????��D?T1??? 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO��??������??
 
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO2������o����y	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //��?��?SDA 

//IIC?����D2������o����y
void IIC_Init(void);                //3?��??��IIC��?IO?��				 
void IIC_Start(void);				//����?��IIC?a��?D?o?
void IIC_Stop(void);	  			//����?��IIC����?1D?o?
void IIC_Send_Byte(u8 txd);			//IIC����?����???��??��
u8 IIC_Read_Byte(unsigned char ack);//IIC?����?��???��??��
u8 IIC_Wait_Ack(void); 				//IIC�̨���yACKD?o?
void IIC_Ack(void);					//IIC����?��ACKD?o?
void IIC_NAck(void);				//IIC2?����?��ACKD?o?

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















