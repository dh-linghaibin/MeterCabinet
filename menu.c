


#include "menu.h"
#include "rs485.h"
#include "lcd.h"
#include "equipment.h"
#include "setppermotor.h"

static u8 menu_page = 0x01;
static u8 menu_row = 0;
static u8 menu_rotate_num = 10;
static u8 menu_door_num = 5;
static u8 menu_ok = 0;


void MenuAddRotate(void) {
    if(menu_rotate_num < 16) {
        menu_rotate_num++;
    } else {
        menu_rotate_num = 0;
    }
}

void MenuSubRotate(void) {
    if(menu_rotate_num > 0) {
        menu_rotate_num--;
    } else {
        menu_rotate_num = 16;
    }
}

u8 MenuGetRotate(void) {
    return menu_rotate_num;
}

void MenuAddDoor(void) {
    if(menu_door_num < 8) {
        menu_door_num++;
    } else {
        menu_door_num = 1;
    }
}

void MenuSubDoor(void) {
    if(menu_door_num > 1) {
        menu_door_num--;
    } else {
        menu_door_num = 8;
    }
}

u8 MenuGetDoor(void) {
    return menu_door_num;
}

void MenuSetPage(u8 num) {
    menu_page = num;
}

u8 MenReadPage(void) {
    return menu_page;
}

void MenuSetRow(u8 num) {
    menu_row = num;
}

u8 MenReadRow(void) {
    return menu_row;
}

void MenuSetOk(u8 num) {
    menu_ok = num;
}

u8 MenReadOk(void) {
    return menu_ok;
}

void MeunShow(u8 cmd, u8 data)
{
      switch(menu_page)
      {
        case 0x04:
            LCDString(0,0,"  ",0);
            LCDString(3,0,"  ",0);
            LCDString(1,0,"��",1);
            LCDString(2,0,"��",1);
            LCDString(0,1,"  ��������ɹ�",1);
            LCDString(1,1,"  ��ǰλ��:",1);
            LCDNum(cmd/10);
            LCDNum(cmd%10);
            LCDString(2,1,"  ��תλ��:01 ",1);
            LCDNum(data/10);
            LCDNum(data%10);
            LCDString(3,1,"  �������:",1);
            LCDNum(EquipGetAddr()/10);
            LCDNum(EquipGetAddr()%10);
            break;
        case 0x05:
            LCDString(0,0,"  ",0);
            LCDString(3,0,"  ",0);
            LCDString(1,0,"��",1);
            LCDString(2,0,"ת",1);
            LCDString(0,1,"  ��������ɹ�",1);
            LCDString(1,1,"  ��ǰλ��:",1);
            LCDNum(cmd/10);
            LCDNum(cmd%10);
            LCDString(2,1,"  ��תλ��:",1);
            LCDNum(data/10);
            LCDNum(data%10);
            LCDString(3,1,"  �������:",1);
            LCDNum(EquipGetAddr()/10);
            LCDNum(EquipGetAddr()%10);
            break;
        case 0x06:
            LCDString(0,0,"  ",0);
            LCDString(3,0,"  ",0);
            LCDString(1,0,"��",1);
            LCDString(2,0,"��",1);
            LCDString(0,7,"  ",1);
            LCDString(0,1,"  ��������ɹ�",1);
            LCDString(0,1,"  ��ǰλ��:",1);
            LCDNum(cmd/10);
            LCDNum(cmd%10);
            LCDString(1,1,"  ����λ��:",1);
            LCDNum(data/10);
            LCDNum(data%10);
            LCDString(3,1,"  �������:",1);
            LCDNum(EquipGetAddr()/10);
            LCDNum(EquipGetAddr()%10);
        break;
        case 0x01:
            LCDString(0,0,"��",0);
            LCDString(3,0,"��",0);
            LCDString(1,0,"��",1);
            LCDString(2,0,"��",1);
            LCDString(0,1,"    ��������  ",1);
            LCDString(1,1,"  ��ǰλ��:",1);
            LCDNum(cmd/10);
            LCDNum(cmd%10);
            LCDString(3,1,"  �������:",1);
            LCDNum(EquipGetAddr()/10);
            LCDNum(EquipGetAddr()%10);
            break;
        case 0x02:
            LCDString(0,0,"  ",0);
            LCDString(1,0,"��",0);
            LCDString(2,0,"��",0);
            LCDString(3,0,"  ",0);

            LCDString(0,1,"  ������: ",1);
            if(SetpReadEncoder() == 0) {
                LCDString(0,6,"�� ",1);
            } else {
                LCDString(0,6,"�� ",1);
            }
            LCDString(1,1,"  ����汾:V1 ",1);
            LCDString(2,1,"  Ӳ���汾:V1 ",1);
            LCDString(3,1,"  ����:16-1-9",1);
            LCDString(0,1,"��",1);
        break;
        case 0x03:
            LCDString(0,0,"��",0);
            LCDString(1,0,"��",0);
            LCDString(2,0,"ģ",0);
            LCDString(3,0,"ʽ", 0);

            LCDString(0,1,"  ��תλ��:",1);
            LCDNum(menu_rotate_num/10);
            LCDNum(menu_rotate_num%10);
            LCDString(1,1,"  �����:",1);
            LCDNum(menu_door_num/10);
            LCDNum(menu_door_num%10);
            LCDString(2,1,"  ����ر�:",1);
            LCDNum(menu_door_num/10);
            LCDNum(menu_door_num%10);
            LCDString(3,1,"  �ص���λ   ",1);
            switch(menu_row) {
                case 0:
                    if(menu_ok == 0) {
                        LCDString(0,1,"��",1);
                    } else {
                        LCDString(0,1,"��",1);
                    }
                    LCDString(1,1," ",1);
                    LCDString(2,1," ",1);
                    LCDString(3,1," ",1);
                break;
                case 1:
                    LCDString(0,1," ",1);
                    if(menu_ok == 0) {
                        LCDString(1,1,"��",1);
                    } else {
                        LCDString(1,1,"��",1);
                    }
                    LCDString(2,1," ",1);
                    LCDString(3,1," ",1);
                break;
                case 2:
                    LCDString(0,1," ",1);
                    LCDString(1,1," ",1);
                    if(menu_ok == 0) {
                        LCDString(2,1,"��",1);
                    } else {
                        LCDString(2,1,"��",1);
                    }
                    LCDString(3,1," ",1);
                break;
                case 3:
                    LCDString(0,1," ",1);
                    LCDString(1,1," ",1);
                    LCDString(2,1," ",1);
                    if(menu_ok == 0) {
                        LCDString(3,1,"��",1);
                    } else {
                        LCDString(3,1,"��",1);
                    }
                break;
                default:break;
        }
        break;
        default:
        break;
      }
}