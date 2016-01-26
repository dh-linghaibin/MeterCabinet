

#include "setppermotor.h"
#include "iostm8s207m8.h"
#include "delay.h"
#include "eeprom.h"

static const u16 timerlow[15]={57000,44000,30000,20000,15000,
12000,11000,10000,9000,8500,8000,7500,7300,7100,7000};

#define Total_Circle 400
#define Average_Pulse	35	 //Single drawer pulse equivalent 175

#define moto_dr PA_ODR_bit.ODR4 /*Motor Direction*/
#define moto_hz PA_ODR_bit.ODR3 /*Motor speed*/
#define b_sar   PD_IDR_bit.IDR0 

static u8 cabinet_position = 0;
static u16 cabinet_angle = 0;

static u8 cabinet_encoder = 0;

void SetpInit(void)
{
    //
    PA_DDR_bit.DDR4 = 1;
    PA_CR1_bit.C14 = 1;
    PA_CR2_bit.C24 = 1;
    //
    PA_DDR_bit.DDR3 = 1;
    PA_CR1_bit.C13 = 1;
    PA_CR2_bit.C23 = 1;
    
    PD_DDR_bit.DDR0 = 0;
    PD_CR1_bit.C10 = 0;
    PD_CR2_bit.C20 = 0;
    
    PD_DDR_bit.DDR2 = 0;
    PD_CR1_bit.C12 = 1;
    PD_CR2_bit.C22 = 1;
    
    EXTI_CR1 |= BIT(7);//����PD���ж�
	EXTI_CR1 &= ~BIT(6);
    
    //3 Set the timer to generate an interrupt once 1ms
	TIM3_PSCR  =0x00;  
	TIM3_EGR = 0x01; 
	TIM3_CNTRH = 0x0;          // Counter Starting Value;
	TIM3_CNTRL = 0x0;     
	TIM3_ARRH = 0X40;
	TIM3_ARRL = 0X01;
	TIM3_IER = 0X01;
	TIM3_CR1 = 0X00;
    //EepromWrite(11,11);
    //EepromWrite(12,0);
    if(EepromRead(14) != 0x55)
    {
        EepromWrite(14,0x55);
        EepromWrite(10,1);
        EepromWrite(11,0);
        EepromWrite(12,0);
        EepromWrite(13,0);
    }
    cabinet_position = EepromRead(10); //clear position
    cabinet_angle = EepromRead(11);
    cabinet_angle |= (EepromRead(12)>>8);
    cabinet_encoder = (EepromRead(13));
}

void SetpSetEncoder(u8 cmd) {
    cabinet_encoder = cmd;
    EepromWrite(13,cmd);
}

u8 SetpReadEncoder(void) {
    return cabinet_encoder;
}

u8 SetpGetPostion(void)
{
    return cabinet_position;
}

void SetpHzSet(u16 hz)
{
    TIM3_ARRH=hz>>8;           
 	TIM3_ARRL=hz;	 //246
}

void SetpStart(u8 cmd)
{
    TIM3_CR1 = cmd; 
}

u8 SetpZero(void)
{
    u8 cheack_save = 0;//����λ��״̬
	u16 stepes = 0;
	u16 stepes_2 = 0;
    u16 duozou = 0;
	moto_dr = 0;/*�ı䷽��*/
	if(b_sar == 0)//������λ
	{
		cheack_save ++;//������λ
		while(b_sar == 0)
		{
			//WDT();//�忴�Ź�
			if(stepes < 100)
			{
				moto_hz = !moto_hz;
				DelayUs(1500);
				stepes++;
			}
			else if(stepes < 200)
			{
				moto_hz = !moto_hz;
				DelayUs(1200);
				stepes++;
			}
			else if(stepes < 300)
			{
				moto_hz = !moto_hz;
				DelayUs(1100);
				stepes++;
			}
			else if(stepes < 400)
			{
				moto_hz = !moto_hz;
				DelayUs(1000);
				stepes++;
			}
			else if(stepes < 500)
			{
				moto_hz = !moto_hz;
				DelayUs(1000);
				stepes++;
			}
			else if(stepes < 600)
			{
				moto_hz = !moto_hz;
				DelayUs(1000);
				stepes++;
			}
			else
			{
				moto_hz = !moto_hz;
				DelayUs(1000);
			}
		}
	}
	else//����λ
	{
		stepes = 200;
	}
	while(stepes)//����λ
	{
		//WDT();//�忴�Ź�
		if(stepes > 600)
		{
			moto_hz = !moto_hz;
			DelayUs(1000);
			stepes_2++;
			if(stepes_2 == 100)
			{
				stepes_2 = 0;
				stepes = 550;
			}
		}
		else if(stepes > 500)
		{
			moto_hz = !moto_hz;
			DelayUs(2000);
			stepes_2++;
			if(stepes_2 == 100)
			{
				stepes_2 = 0;
				stepes = 450;
			}
		}
		else if(stepes > 400)
		{
			moto_hz = !moto_hz;
			DelayUs(3000);
			stepes_2++;
			if(stepes_2 == 100)
			{
				stepes_2 = 0;
				stepes = 350;
			}
		}
		else //if(stepes > 300)
		{
			moto_hz = !moto_hz;
			DelayUs(4200);
			stepes_2++;
			if(stepes_2 == 300)
			{
				stepes_2 = 0;
				stepes = 0;
			}
		}
	}
	DelayUs(300);
	//WDT();//�忴�Ź�
	moto_dr = 1;/*�ı䷽��*/
	while(b_sar == 0)
	{
		cheack_save ++;//������λ
		moto_hz = !moto_hz;
		DelayUs(4200);
		//WDT();//�忴�Ź�
	}
	duozou = 0;
	while(duozou < 50)
	{
		moto_hz = !moto_hz;
		DelayUs(4200);
		duozou++;
		//WDT();//�忴�Ź�
	}
	
	cabinet_position = 1; //clear position
    EepromWrite(10,cabinet_position);
    cabinet_angle = 0;
	EepromWrite(11,cabinet_angle);
    EepromWrite(12,cabinet_angle<<8);
	if(cheack_save > 0)
	{
		return 0X21;
	}
	else
	{
		return 0X22;//�����д���
	}
}

static u8 k; 
static u8 up_or_down;           
static u8 half_over; 
static u8 n_max;
static u16 ksteps;       
static u16 ksteps_inc; 
static u16 ksteps_save;
static u32 steps; 
static u32 steps_half; 
static u32 steps_count;  
static u32 steps_count2;  
static u32 steps_keep_count;
static u16 stop_arr[16] = {
0,23,54,77,100,122,154,177,200,223,254,277,300,322,354,377,
};
	
//���±������ڲ���������п���
static u16 Encoder_count;/*����������*/

u8 SetpRotation(u8 tar_pos)
{
    u16 angle = 0;//save Reaches the required angular position
    u16 result_move = 0;//��Ҫ��ת�ĽǶ�ֵ
    angle = stop_arr[tar_pos-1];
    if(cabinet_angle < angle)
	{
		if((angle-cabinet_angle) <= (Total_Circle/2))
		{
			moto_dr = 1;/*�ı䷽��*/
			result_move = angle-cabinet_angle;
		}
		else
		{
			moto_dr = 0;/*�ı䷽��*/
			result_move=Total_Circle+cabinet_angle-angle;
		}
	}
	else if(cabinet_angle>=angle)
	{
		if((cabinet_angle-angle)<=(Total_Circle /2))
		{
			moto_dr = 0;/*�ı䷽��*/
			result_move=cabinet_angle-angle;
		}
		else if((cabinet_angle-angle)>(Total_Circle /2))
		{
			moto_dr = 1;/*�ı䷽��*/
			result_move=Total_Circle-cabinet_angle+angle;
		}
	}
    
    if(result_move == 0) return 0;
 	steps = result_move* Average_Pulse *2;
 	steps_half = steps/2;  
    
    Encoder_count = 0;//clear
    
    n_max = 14;               //��������ٶȵ�λ���ã��ɶ����޸�  
 	ksteps_inc = 20;          //�����������������޸�����������
 	ksteps_save = 10;  
 	steps_count = 0;
 	steps_count2 = 0; 
 	up_or_down = 1;
 	half_over = 0; 
    
    moto_hz = 0;
    
    SetpHzSet(timerlow[0]);
	SetpStart(1);
    
 	for(k=0;(half_over==0)&&(k<=n_max);k+=1)   
	{     
		ksteps_save=ksteps_save+ksteps_inc;
		ksteps=ksteps_save;     
		while(ksteps>0);  //���ٽ׶�  
	}    
	k-=1;
	if(half_over==0)
	{
		up_or_down=2;
		steps_keep_count=steps-steps_count2;
		while(steps_keep_count);
	}                                             
	up_or_down=3;
 	for(;steps_count<steps;k-=1)
	{       
		ksteps_save=ksteps_save-ksteps_inc;
		ksteps=ksteps_save;	//���ٽ׶�
		if(k<1)
		{
			k=1;
			ksteps=7000;
		}
		while(ksteps>0);       
	}
	SetpStart(0);
	moto_hz = 1;
	DelayUs(2);
	moto_hz = 0;
	DelayUs(2);
	//WDT();//�忴�Ź�
	
	cabinet_position = tar_pos; //clear position
    EepromWrite(10,cabinet_position);
    
    cabinet_angle = angle;
	EepromWrite(11,cabinet_angle);
    EepromWrite(12,cabinet_angle<<8);
    
	return result_move;
}


#pragma vector=0x11
__interrupt void TIM3_UPD_OVF_BRK_IRQHandler(void)
{
     asm("sim");//���жϣ�simΪ���ж�
    TIM3_SR1 &= (~0x01);        //����жϱ�־
    
    SetpHzSet(timerlow[k]);
   // WDT();//�忴�Ź�
    if(up_or_down == 1)
    {
        ksteps--;
        steps_count+=1;
        steps_count2+=2;          
        if(steps_count>=steps_half)
        {
            half_over=1;
            ksteps=0;
        }
        moto_hz = !moto_hz;
    }
    if(up_or_down == 2)
    {
        steps_keep_count-=1;
        steps_count+=1;
        moto_hz = !moto_hz;
    }
    if(up_or_down == 3)
    {
        ksteps--;
        steps_count=steps_count+1;
        if(steps_count>=steps) 
        {
          ksteps=0;
        }
        moto_hz = !moto_hz;
    }
    asm("rim");//���жϣ�simΪ���ж�
}
#pragma vector=0x12
__interrupt void TIM3_CAP_COM_IRQHandler(void)
{
 
}

#pragma vector=8
__interrupt void EXTI_PORTD_IRQHandler(void)
{
    Encoder_count++;
}
