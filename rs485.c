


#include "rs485.h"
#include "iostm8s207m8.h"
#include "equipment.h"

#define RS485_DR PD_ODR_ODR7

void Rs485Init(void)
{
	/***485_PD5****/
	PD_DDR |= BIT(5);
	PD_CR1 |= BIT(5); 
	PD_CR2 |= BIT(5);
	/***485_PD6****/
	PD_DDR &= ~BIT(6);
	PD_CR1 |= BIT(6); 
	PD_CR2 &= ~BIT(6);
	/***485方向流控制_PD7****/
	PD_DDR |= BIT(7);
	PD_CR1 |= BIT(7); 
	PD_CR2 |= BIT(7);
    
	RS485_DR = 0;
	UART3_CR1=0x00;
	UART3_CR2=0x00;
	UART3_CR3=0x00; 
	UART3_BRR2=0x02;
	UART3_BRR1=0x68;
	UART3_CR2=0x2c;//Allowed to receive, send, receive interrupt open
    
    //1 Set the timer to generate an interrupt once 1ms
	TIM1_PSCRH = 0X1F;
	TIM1_PSCRL = 0X3F;
	TIM1_ARRH = 0X00;
	TIM1_ARRL = 0X01;
	TIM1_IER = 0X01;
	TIM1_CR1 = 0X01;
    RS485_DR = 0;
    //Rs485Int(1);
}

static u8 rs485_send_ok;
static u8 array[16] = {0x3a,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x0a};

void Rs485Send(u8 addr,u8 cmd,
               u8 dat1,u8 dat2,
               u8 dat3,u8 dat4,
               u8 dat5,u8 dat6,
               u8 dat7,u8 dat8,
               u8 dat9,u8 dat10)
{
    u8 i_count = 0;
    while(rs485_send_ok == 1);
	UART3_SR&= ~BIT(6);  //Clear send completion status bit
	UART3_CR2 |= BIT(6);//
    
    array[1] = addr;
    array[2] = cmd;
    array[3] = dat1;
    array[4] = dat2;
    array[5] = dat3;
    array[6] = dat4;
    array[7] = dat5;
    array[8] = dat6;
    array[9] = dat7;
    array[10] = dat8;
    array[11] = dat9;
    array[12] = dat10;
    
    array[13] = array[1]+array[2];
    
    array[14] = 0;//clear
    /*add check*/
    for(i_count = 3;i_count < 12;i_count++)
    {
        array[14] += array[i_count];
    }
    
	RS485_DR = 1;
	rs485_send_ok = 1;
	UART3_DR = array[0];
}

static u8 Rs485_data[15];

u8 Rs485Check(void)
{
    u8 check_a = 0;
    u8 check_b = 0;
    u8 i_count = 0;
    
    check_a = Rs485_data[0] + Rs485_data[1];
    
    for(i_count = 2;i_count < 11;i_count++)
    {
        check_b += Rs485_data[i_count];
    }
    
    if( (check_a == Rs485_data[12])&&(check_b == Rs485_data[13]) )
    {
        return 0x88;
    }
    else
    {
        return 0x44;
    }
}

u8 Rs485GetDate(u8 num)
{
    return Rs485_data[num];
}

static u8 Rs485_ok = 0;

u8 Rs485GetFlag(void)
{
    return Rs485_ok;
}

void Rs485FlagClear(void)
{
    Rs485_ok = 0x00;
}

#pragma vector=0x16
__interrupt void UART3_TX_IRQHandler(void)
{
    static u8 rs485_count = 1;
    asm("sim");//
    UART3_SR&= ~(1<<6);//
    UART3_DR = array[rs485_count];
    if(rs485_count < 16)
    {
      rs485_count++;
    }
    else
    {
        rs485_count = 1;
        RS485_DR = 0;
        UART3_CR2 &= ~BIT(6);//
        rs485_send_ok = 0;
    }
    asm("rim");//
    return;
}

static u8 rs485_sta = 0;    
static u8 rs485_add = 0;
#pragma vector=0x17
__interrupt void UART3_RX_IRQHandler(void)
{
    u8 data;
    data = UART3_DR;
    while((UART3_SR & 0x80) == 0x00);//Wait for data transfer
    if(data == 0x3a)
    {
        rs485_sta = 0x40;
    }
    else if(data == EquipGetAddr())
    {
        if(rs485_sta == 0x40)
        {
            rs485_sta = 0x80;
        }
    }
    if(rs485_sta == 0x80)
    {
        Rs485_data[rs485_add] = data;
        rs485_add++;
        if(rs485_add == 15)
        {
            rs485_sta = 0;
            rs485_add = 0;
            if(data == 0x0a)
            {
                Rs485_ok = 0x80;//Data reception is completed
            }
        }
    }
    return;
}

#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)
{
    static u16 tim1_count = 0;
    static u8 Rs485_count = 0;
    TIM1_SR1 &= (~0x01);//Clear the interrupt flag
    if(tim1_count < 3000)
    {
        tim1_count++;
    }
    else
    {
        tim1_count = 0;
        if(rs485_sta > 0x00)
        {
            if(Rs485_count < 10)
            {
                Rs485_count++;
            }
            else
            {
                Rs485_count = 0;
                rs485_sta = 0;
                rs485_add = 0;
            }
        }
    }
  return;
}
#pragma vector=0xE
__interrupt void TIM1_CAP_COM_IRQHandler(void)
{

  return;
}

/*****************************
Function name: void UART interrupt (u8 uart, u8 mode)
Returns: None
Input value: 0 Serial port 30- 11- 1- start off then interrupt receive interrupt
Function: switch interrupt
*****************************/
void Rs485Int(u8 mode)
{
	UART3_CR2_REN = mode;
}


