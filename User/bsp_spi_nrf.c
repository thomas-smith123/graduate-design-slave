/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   2.4g无线模块/nrf24l01+/master 应用bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "bsp_spi_nrf.h"
#include "pcf8563.h"
#include "usart.h"
#include "AES.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

 u8 RX_BUF[RX_PLOAD_WIDTH];		//接收数据缓存
 u8 TX_BUF[TX_PLOAD_WIDTH];		//发射数据缓存
 u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // 定义一个静态发送地址
 u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};

void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
} 

/**
  * @brief  SPI的 I/O配置
  * @param  无
  * @retval 无
  */
void SPI_NRF_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /*开启相应IO端口的时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG,ENABLE);

 /*使能SPI1时钟*/
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2 , ENABLE );

   /*配置 SPI_NRF_SPI的 SCK,MISO,MOSI引脚，GPIOA^5,GPIOA^6,GPIOA^7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用功能
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

  /*配置SPI_NRF_SPI的CE引脚,和SPI_NRF_SPI的 CSN 引脚*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);


   /*配置SPI_NRF_SPI的IRQ引脚*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //上拉输入
  GPIO_Init(GPIOG, &GPIO_InitStructure); 
		  
  /* 这是自定义的宏，用于拉高csn引脚，NRF进入空闲状态 */
  NRF_CSN_HIGH(); 
 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 					//主模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 				//数据大小8位
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 				//时钟极性，空闲时为低
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//第1个边沿有效，上升沿为采样时刻
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   					//NSS信号由软件产生
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //8分频，9MHz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  				//高位在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI2, ENABLE);
}

/**
  * @brief   用于向NRF读/写一字节数据
  * @param   写入的数据
  *		@arg dat 
  * @retval  读取得的数据
  */
u8 SPI_NRF_RW(u8 dat)
{  	
   /* 当 SPI发送缓冲器非空时等待 */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  
   /* 通过 SPI2发送一字节数据 */
  SPI_I2S_SendData(SPI2, dat);		
 
   /* 当SPI接收缓冲器为空时等待 */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

/**
  * @brief   用于向NRF特定的寄存器写入数据
  * @param   
  *		@arg reg:NRF的命令+寄存器地址
  *		@arg dat:将要向寄存器写入的数据
  * @retval  NRF的status寄存器的状态
  */
u8 SPI_NRF_WriteReg(u8 reg,u8 dat)
{
 	u8 status;
	 NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
    NRF_CSN_LOW();
				
	/*发送命令及寄存器号 */
	status = SPI_NRF_RW(reg);
		 
	 /*向寄存器写入数据*/
    SPI_NRF_RW(dat); 
	          
	/*CSN拉高，完成*/	   
  	NRF_CSN_HIGH();	
		
	/*返回状态寄存器的值*/
   	return(status);
}

/**
  * @brief   用于从NRF特定的寄存器读出数据
  * @param   
  *		@arg reg:NRF的命令+寄存器地址
  * @retval  寄存器中的数据
  */
u8 SPI_NRF_ReadReg(u8 reg)
{
 	u8 reg_val;

	NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
 	NRF_CSN_LOW();
				
  	 /*发送寄存器号*/
	SPI_NRF_RW(reg); 

	 /*读取寄存器的值 */
	reg_val = SPI_NRF_RW(NOP);
	            
   	/*CSN拉高，完成*/
	NRF_CSN_HIGH();		
   	
	return reg_val;
}	

/**
  * @brief   用于向NRF的寄存器中写入一串数据
  * @param   
  *		@arg reg : NRF的命令+寄存器地址
  *		@arg pBuf：用于存储将被读出的寄存器数据的数组，外部定义
  * 	@arg bytes: pBuf的数据长度
  * @retval  NRF的status寄存器的状态
  */
u8 SPI_NRF_ReadBuf(u8 reg,u8 *pBuf,u8 bytes)
{
 	u8 status, byte_cnt;

	  NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
	NRF_CSN_LOW();
		
	/*发送寄存器号*/		
	status = SPI_NRF_RW(reg); 

 	/*读取缓冲区数据*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)		  
	   pBuf[byte_cnt] = SPI_NRF_RW(NOP); //从NRF24L01读取数据  

	 /*CSN拉高，完成*/
	NRF_CSN_HIGH();	
		
 	return status;		//返回寄存器状态值
}

/**
  * @brief   用于向NRF的寄存器中写入一串数据
  * @param   
  *		@arg reg : NRF的命令+寄存器地址
  *		@arg pBuf：存储了将要写入写寄存器数据的数组，外部定义
  * 	@arg bytes: pBuf的数据长度
  * @retval  NRF的status寄存器的状态
  */
u8 SPI_NRF_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes)
{
	 u8 status,byte_cnt;
	 NRF_CE_LOW();
   	 /*置低CSN，使能SPI传输*/
	 NRF_CSN_LOW();			

	 /*发送寄存器号*/	
  	 status = SPI_NRF_RW(reg); 
 	
  	  /*向缓冲区写入数据*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
		{
//			printf("%2x ",*pBuf);
			SPI_NRF_RW(*pBuf++);	//写数据到缓冲区 			
		}	 
	  	   
	/*CSN拉高，完成*/
	NRF_CSN_HIGH();			
  
  	return (status);	//返回NRF24L01的状态 		
}

/**
  * @brief  配置并进入接收模式
  * @param  无
  * @retval 无
  */
void NRF_RX_Mode(void)

{
	NRF_CE_LOW();	

   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址    

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);      //设置RF通信频率    

   SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度      

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   

   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 

/*CE拉高，进入接收模式*/	
  NRF_CE_HIGH();

}    

/**
  * @brief  配置发送模式
  * @param  无
  * @retval 无
  */
void NRF_TX_Mode(void)
{  
	NRF_CE_LOW();		

   SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //写TX节点地址 

   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK   

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  

   SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);       //设置RF通道为CHANAL

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	
   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断

/*CE拉高，进入发送模式*/	
  NRF_CE_HIGH();
    Delay(0xffff); //CE要拉高一段时间才进入发送模式
}

/**
  * @brief  主要用于NRF与MCU是否正常连接
  * @param  无
  * @retval SUCCESS/ERROR 连接正常/连接失败
  */
u8 NRF_Check(void)
{
	u8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	u8 buf1[5];
	u8 i; 
	 
	/*写入5个字节的地址.  */  
	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

	/*读出写入的地址 */
	SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
	 
	/*比较*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0xC2)
		break;
	} 
	       
	if(i==5)
		return SUCCESS ;        //MCU与NRF成功连接 
	else
		return ERROR ;        //MCU与NRF不正常连接
}

/**
  * @brief   用于向NRF的发送缓冲区中写入数据
  * @param   
  *		@arg txBuf：存储了将要发送的数据的数组，外部定义	
  * @retval  发送结果，成功返回TXDS,失败返回MAXRT或ERROR
  */
u8 NRF_Tx_Dat(u8 *txbuf)
{
	u8 state;  

	 /*ce为低，进入待机模式1*/
	NRF_CE_LOW();

	/*写数据到TX BUF 最大 32个字节*/						
   SPI_NRF_WriteBuf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);

      /*CE为高，txbuf非空，发送数据包 */   
 	 NRF_CE_HIGH();
	  	
	  /*等待发送完成中断 */                            
	while(NRF_Read_IRQ()!=0); 	
	
	/*读取状态寄存器的值 */                              
	state = SPI_NRF_ReadReg(STATUS);

	 /*清除TX_DS或MAX_RT中断标志*/                  
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state); 	

	SPI_NRF_WriteReg(FLUSH_TX,NOP);    //清除TX FIFO寄存器 

	 /*判断中断类型*/    
	if(state&MAX_RT)                     //达到最大重发次数
			 return MAX_RT; 

	else if(state&TX_DS)                  //发送完成
		 	return TX_DS;
	 else						  
			return ERROR;                 //其他原因发送失败
} 

/**
  * @brief   用于从NRF的接收缓冲区中读出数据
  * @param   
  *		@arg rxBuf ：用于接收该数据的数组，外部定义	
  * @retval 
  *		@arg 接收结果
  */
u8 NRF_Rx_Dat(u8 *rxbuf)
{
	u8 state; 
	NRF_CE_HIGH();	 //进入接收状态
	 /*等待接收中断*/
	while(NRF_Read_IRQ()!=0); 
	
	NRF_CE_LOW();  	 //进入待机状态
	/*读取status寄存器的值  */               
	state=SPI_NRF_ReadReg(STATUS);
	 
	/* 清除中断标志*/      
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state);

	/*判断是否接收到数据*/
	if(state&RX_DR)                                 //接收到数据
	{
	  SPI_NRF_ReadBuf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
	     SPI_NRF_WriteReg(FLUSH_RX,NOP);          //清除RX FIFO寄存器
	  return RX_DR; 
	}
	else    
		return ERROR;                    //没收到任何数据
}
u8 shakehand(void)
{
//	__int64 time1,time2;
	int diff_time,rand_const;
	uint8_t ciphertext[AES_BLOCK_SIZE];
	u8 i,status,keyvalue;
	struct tm *now_GPS,*now_pcf;
	TIME now;
extern	uint8_t key[16];
	uint8_t yanzhengma[14];
	uint8_t ID_Num[10],mingwen1[16],mingwen2[16],miwen1[16],miwen2[16];
	uint8_t plaintext[16] = "1305054145jrd";
/*	const uint8_t const_cipher[AES_BLOCK_SIZE] = {
		0xff, 0x0b, 0x84, 0x4a, 0x08, 0x53, 0xbf, 0x7c,
		0x69, 0x34, 0xab, 0x43, 0x64, 0x14, 0x8f, 0xb9};	
		*/
	uint8_t roundkeys[AES_ROUND_KEY_SIZE];
	uint8_t frame[42]=
		{1,3,0,5,0,5,4,1,4,5,
									0,123,0,0,
									2,0,1,7,0,5,0,1,1,3,4,5,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		};
	uint8_t *syn,*seq,*ack,*fin,*time,*data;
	syn=frame+10;seq=frame+11;		
	ack=frame+12;fin=frame+13;
	time=frame+14;data=frame+28;
	now_GPS=(struct tm*)malloc(sizeof(struct tm));	
	now_pcf=(struct tm*)malloc(sizeof(struct tm));
		/*******************输入密码*********************/
//		printf("输入密码:\n");
//		for(i=0;i!=16;i++)
//		{
////			keyvalue=13;
////			keyvalue=keyarray_Scan();
//			while(!keydown());
//				keyvalue=keyarray_Scan();
//			switch(keyvalue)
//			{
//				case 1:key[i]=keyvalue;printf("1");break;
//				case 2:key[i]=keyvalue;printf("2");break;
//				case 3:key[i]=keyvalue;printf("3");break;
//				case 4:key[i]=keyvalue;printf("4");break;
//				case 5:key[i]=keyvalue;printf("5");break;
//				case 6:key[i]=keyvalue;printf("6");break;
//				case 7:key[i]=keyvalue;printf("7");break;
//				case 8:key[i]=keyvalue;printf("8");break;
//				case 9:key[i]=keyvalue;printf("9");break;
//				case 0:key[i]=0;printf("0");break;
//				case 10:i-=2;break;
//				default:break;
//			}
//		} 
//		printf("key:");
//		for(i=0;i<16;i++)printf("%d",key[i]);
//		printf("\n");
									// key schedule
	aes_key_schedule_128(key, roundkeys);//密钥扩充		
	srand(now.second);
	for(i=0;i<10;i++)
		ID_Num[i]=frame[i];
		*syn=1;*seq=rand();*ack=0;*fin=0;//*data=0;
		rand_const=*seq;
	now=PCF8563_GetTime();
//		printf("20%d年 ",now.year);printf("%d月 ",now.month);printf("%d日 ",now.day);
//		printf("%d时 ",now.hour);printf("%d分 ",now.mint);printf("%d秒 \n",now.second);
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
		delay_ms(100);
		status = NRF_Tx_Dat(ID_Num);//ID 22.75424900   0.7442174s
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
		status = NRF_Tx_Dat(miwen2+12);//23.49846640
		delay_ms(20);
		NRF_RX_Mode();//24.20729630  0.7091056s
		delay_ms(30);
		status = NRF_Rx_Dat(ID_Num);//24.91640190
		status = NRF_Rx_Dat(ID_Num+4);
		status = NRF_Rx_Dat(ID_Num+8);
		status = NRF_Rx_Dat(ID_Num+12);
		status = NRF_Rx_Dat(miwen1);
		status = NRF_Rx_Dat(miwen1+4);
		status = NRF_Rx_Dat(miwen1+8);
		status = NRF_Rx_Dat(miwen1+12);
		status = NRF_Rx_Dat(miwen2);
		status = NRF_Rx_Dat(miwen2+4);
		status = NRF_Rx_Dat(miwen2+8);
		status = NRF_Rx_Dat(miwen2+12);
		now =PCF8563_GetTime();	
//		printf("pcf8563 time: %d %d %d %d %d %d\n",now.year,now.month,now.day,now.hour,now.mint,now.second);
/*******************查找对应ID的密钥***********假设密码已知*********/

		//aes_decrypt_128(roundkeys,ID_Num,ID_Num);
		aes_decrypt_128(roundkeys,miwen1,miwen1);
		aes_decrypt_128(roundkeys,miwen2,miwen2);
		for(i=0;i<10;i++)frame[i]=ID_Num[i];
		for(i=0;i<16;i++)
		{
			frame[10+i]=miwen1[i];
			frame[26+i]=miwen2[i];
		}//帧重建
//		printf("帧重建完成\n");
		now_GPS->tm_year=*time*1000+*(time+1)*100+*(time+2)*10+*(time+3);
		now_GPS->tm_mon=*(time+4)*10+*(time+5);
		now_GPS->tm_mday=*(time+6)*10+*(time+7);
		now_GPS->tm_hour=*(time+8)*10+*(time+9);
		now_GPS->tm_min=*(time+10)*10+*(time+11);
		now_GPS->tm_sec=*(time+12)*10+*(time+13);	
		now_pcf->tm_hour=now.hour;
		now_pcf->tm_sec=now.second;
		now_pcf->tm_min=now.mint;
		now_pcf->tm_mday=now.day;
		now_pcf->tm_mon=now.month;
		now_pcf->tm_year=2000+now.year;
		diff_time=difftime(mktime(now_GPS),mktime(now_pcf));////////////////////next///////		
//		printf("%d diff_time \n",diff_time);
		if(abs(diff_time)<=60)
		{
			if(*syn==1 && *ack==(rand_const+1))//在重建帧之前判断
			{				
				now=PCF8563_GetTime();
			/*****************year******************/
			*time=now.year/1000;//2
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
				*syn=0;
				*ack=++*seq;
				*seq=rand();

			for(i=0;i<16;i++)
			{
				mingwen1[i]=frame[i+10];
				mingwen2[i]=frame[i+26];
			}
			//		printf("发送到的数据包为：\n");
		for(i=0;i<10;i++)printf("%d",ID_Num[i]);
		printf("\n");
		for(i=0;i<16;i++)printf("%d",mingwen1[i]);
		printf("\n");
		for(i=0;i<16;i++)printf("%d",mingwen2[i]);
		printf("\r\n");

				// encryption
			aes_encrypt_128(roundkeys, mingwen1, miwen1);//明文、密文、轮密钥
			aes_encrypt_128(roundkeys, mingwen2, miwen2);
			NRF_TX_Mode();
			delay_ms(150);
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
			printf("shakehand init!\r\n");
//			printf("输入验证码:");			printf("输入验证码：\n");
		for(i=0;i!=14;i++)
		{
			while(!keydown());
				keyvalue=keyarray_Scan();
			switch(keyvalue)
			{
				case 1:yanzhengma[i]=keyvalue;printf("1");break;
				case 2:yanzhengma[i]=keyvalue;printf("2");break;
				case 3:yanzhengma[i]=keyvalue;printf("3");break;
				case 4:yanzhengma[i]=keyvalue;printf("4");break;
				case 5:yanzhengma[i]=keyvalue;printf("5");break;
				case 6:yanzhengma[i]=keyvalue;printf("6");break;
				case 7:yanzhengma[i]=keyvalue;printf("7");break;
				case 8:yanzhengma[i]=keyvalue;printf("8");break;
				case 9:yanzhengma[i]=keyvalue;printf("9");break;
				case 0:yanzhengma[i]=0;printf("0");break;
				case 10:i-=2;break;
				default:break;
			}
		} 
		printf("\r\nkey:");
		for(i=0;i<14;i++)
		{
			printf("%d",yanzhengma[i]);
			data[i]=yanzhengma[i];
		}	
		printf("\r\n");		
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
		free(now_GPS);
		free(now_pcf);
			return 1;
			}
			else
			{		
				free(now_GPS);
				free(now_pcf);
				return 0;
			}
		}
		else 
		{
			return 0;
			free(now_GPS);
			free(now_pcf);
		}
	// decryption
//	aes_decrypt_128(roundkeys, ciphertext, ciphertext);

}
/*********************************************END OF FILE**********************/
