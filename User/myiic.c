#include "myiic.h"
#include "delay.h"
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
 
//3?��??��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//��1?��GPIOB����?��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //��?������?3?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 ��?3???
}
//2������IIC?e��?D?o?
void IIC_Start(void)
{
	SDA_OUT();     //sda??��?3?
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//?����?I2C����??��?��?��?����?��?��?����?��y?Y 
}	  
//2������IIC����?1D?o?
void IIC_Stop(void)
{
	SDA_OUT();//sda??��?3?
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����?��I2C����???����?D?o?
	delay_us(4);							   	
}
//�̨���y��|��eD?o?��?����
//����???�̡�o1��??����?��|��e����㨹
//        0��??����?��|��e3��1|
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����???a��?��?  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//����?����?3?0 	   
	return 0;  
} 
//2������ACK��|��e
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//2?2������ACK��|��e		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����?����???��??��
//����??�䨮?����D?T��|��e
//1��?��D��|��e
//0��??T��|��e			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//��-�̨�����?��?a��?��y?Y��?��?
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //??TEA5767?a��y???������??��?��?D?��?
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//?��1??��??����?ack=1������?����?��ACK��?ack=0��?����?��nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����???a��?��?
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����?��nACK
    else
        IIC_Ack(); //����?��ACK   
    return receive;
}



























