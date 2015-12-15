/*
*Forward
Reverse
Brake (slow decay)
Fast-decay synchronous
rectification(2)
Fast-decay synchronous
rectification(2)
Sleep mode
*
*As long as sleep is equal to 1 on sleep
*when not sleeping(1) close enable(0) brake
*/

#include "manipulator.h"
#include "iostm8s207m8.h"
#include "eeprom.h"

#define MAN_SLEEP PH_ODR_ODR7

#define MAN_DIR1 PI_ODR_ODR0
#define MAN_DIR2 PI_ODR_ODR1
#define MAN_DIR3 PI_ODR_ODR2
#define MAN_DIR4 PI_ODR_ODR3
#define MAN_DIR5 PI_ODR_ODR4
#define MAN_DIR6 PI_ODR_ODR5
#define MAN_DIR7 PI_ODR_ODR6
#define MAN_DIR8 PI_ODR_ODR7

#define MAN_EN1 PG_ODR_ODR0
#define MAN_EN2 PG_ODR_ODR1
#define MAN_EN3 PG_ODR_ODR2
#define MAN_EN4 PG_ODR_ODR3
#define MAN_EN5 PG_ODR_ODR4
#define MAN_EN6 PG_ODR_ODR5
#define MAN_EN7 PG_ODR_ODR6
#define MAN_EN8 PG_ODR_ODR7

#define MAN_BACK1 PB_IDR_IDR0
#define MAN_BACK2 PB_IDR_IDR1
#define MAN_BACK3 PB_IDR_IDR2
#define MAN_BACK4 PB_IDR_IDR3
#define MAN_BACK5 PB_IDR_IDR4
#define MAN_BACK6 PB_IDR_IDR5
#define MAN_BACK7 PB_IDR_IDR6
#define MAN_BACK8 PB_IDR_IDR7

#define MAN_OUT1 PE_IDR_IDR0
#define MAN_OUT2 PE_IDR_IDR1
#define MAN_OUT3 PE_IDR_IDR2
#define MAN_OUT4 PE_IDR_IDR3
#define MAN_OUT5 PE_IDR_IDR4
#define MAN_OUT6 PE_IDR_IDR5
#define MAN_OUT7 PE_IDR_IDR6
#define MAN_OUT8 PE_IDR_IDR7

#define MAN_LIGHT1 PC_ODR_ODR0
#define MAN_LIGHT2 PC_ODR_ODR1
#define MAN_LIGHT3 PC_ODR_ODR2
#define MAN_LIGHT4 PC_ODR_ODR3
#define MAN_LIGHT5 PC_ODR_ODR4
#define MAN_LIGHT6 PC_ODR_ODR5
#define MAN_LIGHT7 PC_ODR_ODR6
#define MAN_LIGHT8 PC_ODR_ODR7

static u8 Shield = 0;//save shiled

void ManipuInit(void)
{
    u8 data = 0;
    //sleep PH7
    PH_DDR_bit.DDR7 = 1;
    PH_CR1_bit.C17 = 1;
    PH_CR2_bit.C27 = 1;
    //direction Current collection PF0
    PF_DDR_bit.DDR0 = 0;
    PF_CR1_bit.C10 = 0;
    PF_CR2_bit.C20 = 0;
    //direction PI0-7
    PI_DDR = 0xff;
    PI_CR1 = 0xff;
    PI_CR2 = 0xff;
    //Enable PG0-7
    PG_DDR = 0xff;
    PG_CR1 = 0xff;
    PG_CR2 = 0xff;
    //Back position ok PB0-7
    PB_DDR = 0x00;
    PB_CR1 = 0x00;
    PB_CR2 = 0x00;
    //OUT position ok PE0-7
    PE_DDR = 0x00;
    PE_CR1 = 0x00;
    PE_CR2 = 0x00;
    //LED PC0-7
    PC_DDR = 0xff;
    PC_CR1 = 0xff;
    PC_CR2 = 0xff;
    //brake
    MAN_SLEEP = 1;
    PG_ODR = 0X00; //Enable all set 0
    PC_ODR = 0X00;//Off light
    //Button PA6
    PA_DDR_bit.DDR6 = 0;
    PA_CR1_bit.C16 = 1;
    PA_CR2_bit.C26 = 0;
    //Set the AD conversion**********
    //DC_CSR |= BIT(0)|BIT(3); //Select channel
    ADC_CSR &= ~BIT(6);//No analog Dog
    ADC_CSR &= ~BIT(4);
    ADC_CR1 |= BIT(1);//Continuous conversion mode
    ADC_CR2 |= BIT(1);//Scan mode is enabled
    ADC_TDRL = 0x01;
    
    data = EepromRead(20); //clear position
    if(data != 0x55)
    {
        EepromWrite(20,0x55);
        EepromWrite(21,0x00);
        EepromWrite(22,0x00);
        EepromWrite(23,0x00);
        EepromWrite(24,0x00);
        EepromWrite(25,0x00);
    }
}

u8 ManipuGetNum(u8 cmd)
{
    u8 data = 0;
    data = EepromRead(20+cmd); //clear position
    return data;
}

u8 ManipuNumClear(void)
{
    EepromWrite(20,0x55);
    EepromWrite(21,0x00);
    EepromWrite(22,0x00);
    EepromWrite(23,0x00);
    EepromWrite(24,0x00);
    EepromWrite(25,0x00);
    return 0x21;
}

static u16 GetAd(u8 chx)
{
    u16 ad_value;
    u8 i,h = 0,l = 0;
    ADC_CSR = chx;
    ADC_CR1 = 0x01; 
    for(i=0;i<100;i++); 
    ADC_CR1 = ADC_CR1 | 0x01; 
    // enable ad
    while((ADC_CSR & 0x80) == 0); // wait ad
    h = ADC_DRH;
    l = ADC_DRL;
    ad_value = (h<<8) + l;
    return ad_value;
}

static u8 button_flag = 0;

u8 ManipuDir(u8 bit,u8 dir)
{
    u16 time_count = 0;//time out count
    u16 back_count = 0;//In place to ensure that
    u16 current = 0;//sava current value
    u16 current_count = 0;//sava current count
    u8 user_data = 0;
    
    user_data = EepromRead(21);
    if(user_data == 0xff)
    {
        user_data = EepromRead(22);
        if(user_data == 0xff)
        {
            user_data = EepromRead(23);
            if(user_data == 0xff)
            {
                user_data = EepromRead(24);
                if(user_data == 0xff)
                {
                    user_data = EepromRead(25);
                    if(user_data == 0xff)
                    {
                      
                    }
                    else
                    {
                        EepromWrite(25,user_data+1);
                    }
                }
                else
                {
                     EepromWrite(24,user_data+1);
                }
            }
            else
            {
                EepromWrite(23,user_data+1);
            }
        }
        else
        {
            EepromWrite(22,user_data+1);
        }
    }
    else
    {
        EepromWrite(21,user_data+1);
    }
    
    if(dir == 0)//back
    {
        switch(bit)
        {
        case 1:
            if(MAN_BACK1 == 0)//Not in place
            {
                MAN_DIR1 = 1;//Set Direction
                MAN_EN1 = 1;//Enable manipulator
                while(time_count < 60000)//time out 
                {
                    if(MAN_BACK1 == 1)//position ok
                    {
                        if(back_count < 500)
                        {
                            back_count++;
                        }
                        else
                        {
                            time_count = 65000;//out ok
                        }
                    }
                    else
                    {
                        back_count = 0;
                    }
                    //Current collection
                    current = GetAd(10);
                    if(current > 600)
                    {
                        if(current_count < 500)
                        {
                            current_count++;
                        }
                        else
                        {
                            time_count = 65000;//out ok
                        }
                    }
                    else
                    {
                        current_count = 0;
                    }
                    //count wait
                     time_count++;
                }
                if(MAN_BACK1 == 1)//To the positon of success
                {
                    button_flag = 1;//Start Check keys
                    return 0x21;
                }
                else//Failure to position
                {
                    return 0x22;
                }
            }
            else//position  ok
            {
                button_flag = 1;//Start Check keys
                return 0x21;
            }
        break;
        case 2:
        break;
        case 3:
        break;
        case 4:
        break;
        case 5:
        break;
        case 6:
        break;
        case 7:
        break;
        case 8:
        break;
        default:
          return 0x22;
        break;
        }
    }
    else if(dir == 1)//out
    {
        switch(bit)
        {
        case 1:
          break;
        case 2:
          break;
        case 3:
          break;
        case 4:
          break;
        case 5:
          break;
        case 6:
          break;
        case 7:
          break;
        case 8:
          break;
        default:
          return 0x22;
          break;
        }
    }
    return 0x22;
}

u8 ManipuCheckLayer(u8 data,u8 cmd)
{
    if(cmd == 0)
    {
        switch(data)
        {
        case 1:
          if(MAN_BACK1 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 2:
          if(MAN_BACK2 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 3:
          if(MAN_BACK3 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 4:
          if(MAN_BACK4 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 5:
          if(MAN_BACK5 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 6:
          if(MAN_BACK6 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 7:
          if(MAN_BACK7 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 8:
          if(MAN_BACK8 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        default:
          return 0x22;
          break;
        }
    }
    else
    {
        switch(data)
        {
        case 1:
          if(MAN_OUT1 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 2:
          if(MAN_OUT2 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 3:
          if(MAN_OUT3 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 4:
          if(MAN_OUT4 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 5:
          if(MAN_OUT5 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 6:
          if(MAN_OUT6 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 7:
          if(MAN_OUT7 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        case 8:
          if(MAN_OUT8 == 1)
          {
            return 0x21;
          }
          else
          {
            return 0x20;
          }
          break;
        default:
          return 0x22;
          break;
        }
    }
}

u8 ManipuCheckOk(void)
{
    u8 data = 0;
    data = PB_IDR;
    data |= Shield;

    return data;
}

u8 ManipuLight(u8 cmd,u8 data)
{
    switch(cmd)
    {   
    case 1:
      MAN_LIGHT1 = data;
      break;
    case 2:
      MAN_LIGHT2 = data;
      break;
    case 3:
      MAN_LIGHT3 = data;
      break;
    case 4:
      MAN_LIGHT4 = data;
      break;
    case 5:
      MAN_LIGHT5 = data;
      break;
    case 6:
      MAN_LIGHT6 = data;
      break;
    case 7:
      MAN_LIGHT7 = data;
      break;
    case 8:
      MAN_LIGHT8 = data;
      break;
    }
    return 0x21;
}

#define bun_back PA_IDR_IDR6

u8 ManipuBunRead(void)
{
    if(button_flag == 1)//Start Check keys
    {
        if(bun_back == 0)
        {
            button_flag = 2;
        }
    }
    else if(button_flag == 2)
    {
        return 0x21;
    }
    return 0x22;
}

