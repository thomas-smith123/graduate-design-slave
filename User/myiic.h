#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//¡À?3¨¬D¨°??1??¡ì?¡ã¨º1¨®?¡ê??¡ä?-¡Á¡Â??D¨ª?¨¦¡ê?2?¦Ì?¨®?¨®¨²???¨¹¨¨?o?¨®?¨ª?
//ALIENTEK???¡éSTM32?a¡¤¡é¡ã?
//IIC?y?¡¥ ¡ä¨²??	   
//?y¦Ì??-¡Á¨®@ALIENTEK
//??¨º???¨¬3:www.openedv.com
//DT??¨¨??¨²:2012/9/9
//¡ã?¡À?¡êoV1.0
//¡ã?¨¨¡§?¨´¨®D¡ê?¦Ì¨¢¡ã?¡À????¡ê
//Copyright(C) 1??Y¨ºDD?¨°¨ª¦Ì?¡Á¨®????¨®D?T1??? 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO¡¤??¨°¨¦¨¨??
 
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO2¨´¡Á¡Âo¡¥¨ºy	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //¨º?¨¨?SDA 

//IIC?¨´¨®D2¨´¡Á¡Âo¡¥¨ºy
void IIC_Init(void);                //3?¨º??¡¥IIC¦Ì?IO?¨²				 
void IIC_Start(void);				//¡¤¡é?¨ªIIC?a¨º?D?o?
void IIC_Stop(void);	  			//¡¤¡é?¨ªIIC¨ª¡ê?1D?o?
void IIC_Send_Byte(u8 txd);			//IIC¡¤¡é?¨ª¨°???¡Á??¨²
u8 IIC_Read_Byte(unsigned char ack);//IIC?¨¢¨¨?¨°???¡Á??¨²
u8 IIC_Wait_Ack(void); 				//IIC¦Ì¨¨¡äyACKD?o?
void IIC_Ack(void);					//IIC¡¤¡é?¨ªACKD?o?
void IIC_NAck(void);				//IIC2?¡¤¡é?¨ªACKD?o?

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















