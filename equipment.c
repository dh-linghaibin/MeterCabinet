

#include "equipment.h"
#include "iostm8s207m8.h"


#define fmq PD_ODR_bit.ODR4

void EquipInit(void)
{
    CLK_CKDIVR=0x00;//Clock prescaler, 8 default assignment, 0x18.16M-0x00; 8M-0x08; 4M-0x10;
    //addr PF3-PF6
    PF_DDR_bit.DDR3 = 0;
    PF_CR1_bit.C13 = 1;
    PF_CR2_bit.C23 = 0;
    
    PF_DDR_bit.DDR4 = 0;
    PF_CR1_bit.C14 = 1;
    PF_CR2_bit.C24 = 0;
    
    PF_DDR_bit.DDR5 = 0;
    PF_CR1_bit.C15 = 1;
    PF_CR2_bit.C25 = 0;
    
    PF_DDR_bit.DDR6 = 0;
    PF_CR1_bit.C16 = 1;
    PF_CR2_bit.C26 = 0;
    //LED PA5
    PA_DDR_bit.DDR5 = 1;
    PA_CR1_bit.C15 = 1;
    PA_CR2_bit.C25 = 1;
    
    //RUN LED PD3
    PD_DDR_bit.DDR3 = 1;
    PD_CR1_bit.C13 = 1;
    PD_CR2_bit.C23 = 1;
    //FMQ PD4
    PD_DDR_bit.DDR4 = 1;
    PD_CR1_bit.C14 = 1;
    PD_CR2_bit.C24 = 1;
    //BUNTU PH0-3 PF7
    PH_DDR_bit.DDR0 = 0;
    PH_CR1_bit.C10 = 1;
    PH_CR2_bit.C20 = 0;
    
    PH_DDR_bit.DDR1 = 0;
    PH_CR1_bit.C11 = 1;
    PH_CR2_bit.C21 = 0;
    
    PH_DDR_bit.DDR2 = 0;
    PH_CR1_bit.C12 = 1;
    PH_CR2_bit.C22 = 0;
    
    PH_DDR_bit.DDR3 = 0;
    PH_CR1_bit.C13 = 1;
    PH_CR2_bit.C23 = 0;
    
    PF_DDR_bit.DDR7 = 0;
    PF_CR1_bit.C17 = 1;
    PF_CR2_bit.C27 = 0;
    
    fmq = 0;
}

#define adr_4 PF_IDR_IDR6	
#define adr_3 PF_IDR_IDR5
#define adr_2 PF_IDR_IDR4	
#define adr_1 PF_IDR_IDR3

static u8 equip_addr = 0;

void EquipAddrRead(void)
{
	equip_addr = 0;
	if(adr_4 == 0)
	{
		equip_addr |= BIT(0);
	}else
	{
		equip_addr &= ~BIT(0);
	}
	if(adr_3 == 0)
	{
		equip_addr |= BIT(1);
	}else
	{
		equip_addr &= ~BIT(1);
	}
	if(adr_2 == 0)
	{
		equip_addr |= BIT(2);
	}else
	{
		equip_addr &= ~BIT(2);
	}
	if(adr_1 == 0)
	{
		equip_addr |= BIT(3);
	}else
	{
		equip_addr &= ~BIT(3);
	}
}

u8 EquipGetAddr(void)
{
    return equip_addr;
}

void EquipLed(u8 data)
{
    PA_ODR_bit.ODR5 = data;
}

#define BUTTON_IN1 PH_IDR_IDR0	
#define BUTTON_IN2 PH_IDR_IDR1
#define BUTTON_IN3 PH_IDR_IDR2	
#define BUTTON_IN4 PH_IDR_IDR3
#define BUTTON_IN5 PF_IDR_IDR7

u8 EquipButonUp(void)
{
    static u16 button_count = 0;// key delay count
    if(BUTTON_IN1 == 0) {
        if(button_count < 1000) {
            button_count++;
        } else {
            if(button_count == 1000) {
                button_count++;
                return 0x01;
            }
        }
    } else {
        button_count = 0;
    }
    return 0x00;
}

u8 EquipButonOk(void)
{
    static u16 button_count = 0;// key delay count
    if(BUTTON_IN2 == 0) {
        if(button_count < 1000) {
            button_count++;
        } else {
            if(button_count == 1000) {
                button_count++;
                return 0x01;
            }
        }
    } else {
        button_count = 0;
    }
    return 0x00;
}

u8 EquipButonRight(void)
{
    static u16 button_count = 0;// key delay count
    if(BUTTON_IN3 == 0) {
        if(button_count < 1000) {
            button_count++;
        } else {
            if(button_count == 1000) {
                button_count++;
                return 0x01;
            }
        }
    } else {
        button_count = 0;
    }
    return 0x00;
}

u8 EquipButonLeft(void)
{
    static u16 button_count = 0;// key delay count
    if(BUTTON_IN4 == 0) {
        if(button_count < 1000) {
            button_count++;
        } else {
            if(button_count == 1000) {
                button_count++;
                return 0x01;
            }
        }
    } else {
        button_count = 0;
    }
    return 0x00;
}

u8 EquipButonDown(void)
{
    static u16 button_count = 0;// key delay count
    if(BUTTON_IN5 == 0) {
        if(button_count < 1000) {
            button_count++;
        } else {
            if(button_count == 1000) {
                button_count++;
                return 0x01;
            }
        }
    } else {
        button_count = 0;
    }
    return 0x00;
}






