#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//㊣?3足D辰??1??∫?～那1車?㏒??∩?-℅¯??D赤?谷㏒?2?米?車?車迆???邦豕?o?車?赤?
//ALIENTEK???⊿STM32?a﹞⊿～?
//IIC?y?‘ ∩迆??	   
//?y米??-℅車@ALIENTEK
//??那???足3:www.openedv.com
//DT??豕??迆:2012/9/9
//～?㊣?㏒oV1.0
//～?豕“?迄車D㏒?米芍～?㊣????㏒
//Copyright(C) 1??Y那DD?辰赤米?℅車????車D?T1??? 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//3?那??‘IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//那1?邦GPIOB那㊣?車
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //赤?赤足那?3?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 那?3???
}
//2迆谷迆IIC?e那?D?o?
void IIC_Start(void)
{
	SDA_OUT();     //sda??那?3?
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//?‘℅?I2C℅邦??㏒?℅?㊣?﹞⊿?赤?辰?車那?那y?Y 
}	  
//2迆谷迆IIC赤㏒?1D?o?
void IIC_Stop(void)
{
	SDA_OUT();//sda??那?3?
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//﹞⊿?赤I2C℅邦???芍那?D?o?
	delay_us(4);							   	
}
//米豕∩y車|∩eD?o?米?角∩
//﹞米???米㏒o1㏒??車那?車|∩e那∫～邦
//        0㏒??車那?車|∩e3谷1|
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA谷豕???a那?豕?  
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
	IIC_SCL=0;//那㊣?車那?3?0 	   
	return 0;  
} 
//2迆谷迆ACK車|∩e
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
//2?2迆谷迆ACK車|∩e		    
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
//IIC﹞⊿?赤辰???℅??迆
//﹞米??∩車?迆車D?T車|∩e
//1㏒?車D車|∩e
//0㏒??T車|∩e			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//角-米赤那㊣?車?a那?那y?Y∩?那?
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //??TEA5767?a豕y???車那㊣??那?㊣?D?米?
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//?芍1??℅??迆㏒?ack=1那㊣㏒?﹞⊿?赤ACK㏒?ack=0㏒?﹞⊿?赤nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA谷豕???a那?豕?
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
        IIC_NAck();//﹞⊿?赤nACK
    else
        IIC_Ack(); //﹞⊿?赤ACK   
    return receive;
}



























