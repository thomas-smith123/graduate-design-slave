#include "stm32f10x.h"
#ifndef __PCF8563_H
#define __PCF8563_H	
#define I2C1_OWN_ADDRESS7 0x0A
#define Write_PCF8563 0xa2
#define Read_PCF8563  0xa3
#define I2C_Speed 400000
#define control_status1 0x00
#define control_status2 0x01
#define Seconds 0x02
#define Minutes 0x03
#define Hours 0x04
#define Days 0x05
#define Weeks 0x06
#define Months 0x07
#define Years 0x08
#define Minute_Alarm 0x09
#define Hour_Alarm 0x0A
#define Day_Alarm 0x0B
#define Weekday_Alarm 0x0C
#define CLOCK_Frequency 0x0D
#define Timer_Control 0x0E
#define Timer_Countdown_Value 0x0F
#define PCF8563_Address_Control_Status_1         (unsigned char)0x00  //����/״̬�Ĵ���1  
#define PCF8563_Address_Control_Status_2         (unsigned char)0x01  //����/״̬�Ĵ���2  
#define PCF8563_Address_CLKOUT                   (unsigned char)0x0d  //CLKOUTƵ�ʼĴ���  
#define PCF8563_Address_Timer                    (unsigned char)0x0e  //��ʱ�����ƼĴ���  
#define PCF8563_Address_Timer_VAL                (unsigned char)0x0f  //��ʱ���������Ĵ���  
  
#define PCF8563_Address_Years                    (unsigned char)0x08  //��  
#define PCF8563_Address_Months                   (unsigned char)0x07  //��  
#define PCF8563_Address_Days                     (unsigned char)0x05  //��  
#define PCF8563_Address_WeekDays                 (unsigned char)0x06  //����  
#define PCF8563_Address_Hours                    (unsigned char)0x04  //Сʱ  
#define PCF8563_Address_Minutes                  (unsigned char)0x03  //����  
#define PCF8563_Address_Seconds                  (unsigned char)0x02  //��  
  
#define PCF8563_Alarm_Minutes                    (unsigned char)0x09  //���ӱ���  
#define PCF8563_Alarm_Hours                      (unsigned char)0x0a  //Сʱ����  
#define PCF8563_Alarm_Days                       (unsigned char)0x0b  //�ձ���  
#define PCF8563_Alarm_WeekDays                   (unsigned char)0x0c  //���ڱ��� 
typedef struct 
{
 int year;
 int month;
 int day;
 int hour;
 int mint;
 int second;
}TIME;
void PCF8563_Init(void);
static void I2C_Mode_Config(void);
void PCF_SetTime(int year, int month, 
								 int day, int hour, 
								 int mint, int second);
TIME PCF8563_GetTime(void);
u8 rtc8563_Read(u8 rtc_addr);
#endif  
