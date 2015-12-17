

#include "lcd.h"
#include "iostm8s207m8.h"
#include "delay.h"

#define CS_H    PH_ODR_bit.ODR6 = 1     //PH4 E
#define CS_L    PH_ODR_bit.ODR6 = 0
#define SID_H   PH_ODR_bit.ODR5 = 1     //PH5 SID
#define SID_L   PH_ODR_bit.ODR5 = 0
#define SCLK_H  PH_ODR_bit.ODR4 = 1     //PH6 SCLK
#define SCLK_L  PH_ODR_bit.ODR4 = 0

/*
*send 8 bit data to lcd
*/
static void SendByte(u8 zdata)
{
    u16 i;
    for(i=0; i<8; i++)
    {
        if((zdata << i) & 0x80)
        {
            SID_H;
        }
        else
        {
            SID_L;
        }
        SCLK_H;
        SCLK_L;
    }
}

/*
*Write Commed 
*/
static void WriteCom(u8 cmdcode)
{
   CS_H;
   SendByte(0xf8);
   SendByte(cmdcode & 0xf0);
   SendByte((cmdcode << 4) & 0xf0);
   DelayMs(2);
   CS_L;
}

/*
*Write Data
*
*/
static void WriteData(u8 Dispdata)
{  
  CS_H;
  SendByte(0xfa);	  //11111,RW(0),RS(1),0
  SendByte(Dispdata & 0xf0);
  SendByte((Dispdata << 4) & 0xf0);
  DelayMs(2);
  CS_L;
}

void LcdInit(void)
{
    PH_DDR_bit.DDR4 = 1;
    PH_DDR_bit.DDR5 = 1;
    PH_DDR_bit.DDR6 = 1;
  
    PH_CR1_bit.C14 = 1;
    PH_CR1_bit.C15 = 1;
    PH_CR1_bit.C16 = 1;
    
    PH_CR2_bit.C24 = 1;
    PH_CR2_bit.C25 = 1;
    PH_CR2_bit.C26 = 1;
    
    DelayMs(250);
    WriteCom(0x30);  //30-- basic command operation
    DelayMs(5);
    WriteCom(0x0c);  //The screen does not move the cursor right
    DelayMs(5);
    WriteCom(0x01);  //clear
    DelayMs(5);    //Clear lcd long time
    WriteCom(0x06);  //The lcd Open, the cursor open, anti-white Off
    DelayMs(10);
}

/*********************************************************
Function name: Display LCD Pos
Returns: None
Function: Set the display position
*********************************************************/
static void LCDPos(u8 x,u8 y) 
{
	u8 pos;
	switch(x)
	{
		case 0: x=0x80;break;
		case 1: x=0x90;break;
		case 2: x=0x88;break;
		case 3: x=0x98;break;
	}
	pos=x+y;
	WriteCom(pos);
}

/*********************************************************
Function name: Display LCD String ()
Returns: None
Function: Display string
*********************************************************/
void LCDString(u8 x,u8 y,u8 *p,u8 time)
{
	LCDPos(x,y);
	for(;*p!='\0';p++)
	{
		WriteData(*p);
		DelayMs(time);
	}
}

void LCDNum(u8 num)
{
    WriteData(0x30+num);
}
