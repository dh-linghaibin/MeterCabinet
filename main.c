/*
*
*Version:1.1
*
*linghaibin Steven
*
*/
#include "setppermotor.h"
#include "manipulator.h"
#include "equipment.h"
#include "eeprom.h"
#include "rs485.h"
#include "menu.h"
#include "lcd.h"

#define debug 1

int main(void)
{
  EquipInit();//bsp init
  EeepromInit();//eeprom Unlock
  EquipAddrRead();//read the machine address
  ManipuInit();//manipulator init and AD init
  Rs485Init();//com init  and TIM3 init
  SetpInit();//setpipulator init and TIM3 init
  LcdInit();//12864 lcd init 
  MeunShow(SetpGetPostion(),0x00);
  INTEN
  while(1)
  {
    if(EquipButonRight() == 0x01)
    {
        MenuSetOk(0);
        if(MenReadPage() < 3) {
            MenuSetPage(MenReadPage()+1);
        } else {
            MenuSetPage(0x01);
        }
        MeunShow(SetpGetPostion(),0x00);
    }
    if(EquipButonLeft() == 0x01)
    {
        MenuSetOk(0);
        if(MenReadPage() > 1) {
            MenuSetPage(MenReadPage()-1);
        } else {
            MenuSetPage(0x03);
        }
        MeunShow(SetpGetPostion(),0x00);
    }
    if(EquipButonUp() == 0x01) {
        if(MenReadOk() == 0) {
            if(MenReadPage() == 0x03) {
                if(MenReadRow() > 0) {
                    MenuSetRow(MenReadRow()-1);
                } else {
                    MenuSetRow(0x03);
                }
            }
        } else {
            if(MenReadRow() == 0) {
                MenuAddRotate();
            } else if((MenReadRow() == 1)||(MenReadRow() == 2)) {
                MenuAddDoor();
            }
        }
        MeunShow(0x56,0x00);
    }
    if(EquipButonDown() == 0x01) {
        if(MenReadOk() == 0) {
            if(MenReadPage() == 0x03) {
                if(MenReadRow() < 3) {
                    MenuSetRow(MenReadRow()+1);
                } else {
                    MenuSetRow(0x00);
                }
            }
        } else {
            if(MenReadRow() == 0) {
                MenuSubRotate();
            } else if((MenReadRow() == 1)||(MenReadRow() == 2)) {
                MenuSubDoor();
            }
        }
        MeunShow(SetpGetPostion(),0x00);
    }
    if(EquipButonOk() == 0x01) {
        if(MenReadPage() == 0x02) {
            if(SetpReadEncoder() == 0) {
                SetpSetEncoder(1);
            } else {
                SetpSetEncoder(0);
            }
        } else if(MenReadPage() == 0x03) {
            if(MenReadOk() == 0) {
                MenuSetOk(1);
            } else {
                MenuSetOk(0);
                switch(MenReadRow()) {
                    case 0:
                       SetpRotation(MenuGetRotate());
                    break;
                    case 1:
                        ManipuDir(MenuGetDoor(),0);
                    break;
                    case 2:
                        ManipuDir(MenuGetDoor(),1);
                    break;
                    case 3:
                        SetpZero();
                    break;
                }
            }
        }
        MeunShow(SetpGetPostion(),0x00);
    }
    ManipuBunRead();//Starts keys
    if(Rs485GetFlag() == 0x80)//Data sent over
    {
        if(Rs485Check() == CORRECT)//chaeck
        {
            Rs485Send(EquipGetAddr(),CORRECT,0,0,0,0,0,0,0,0,0,0);//Verification is successful
            switch(Rs485GetDate(1))
            {
            case Back_zero:
              MeunShow(Back_zero,0x00);
              if(ManipuCheckOk() == 0xff)
              {
                Rs485Send(EquipGetAddr(),SetpZero(),0,0,0,0,0,0,0,0,0,0);
              }
              else
              {
                Rs485Send(EquipGetAddr(),FALSE,0,0,0,0,0,0,0,0,0,0);
              }
              MenuSetPage(0x04);
              MeunShow(SetpGetPostion(),0x00);
              break;
            case Motor_rotation:
              if(ManipuCheckOk() == 0xff)
              {
                SetpRotation(Rs485GetDate(2));
                Rs485Send(EquipGetAddr(),TRUE,0,0,0,0,0,0,0,0,0,0);
              }
              else
              {
                Rs485Send(EquipGetAddr(),FALSE,0,0,0,0,0,0,0,0,0,0);
              }
              MenuSetPage(0x05);
              MeunShow(SetpGetPostion(),Rs485GetDate(2));
              break;
            case Drawer_out:
              Rs485Send(EquipGetAddr(),ManipuDir(Rs485GetDate(2),1),0,0,0,0,0,0,0,0,0,0);
              MenuSetPage(0x06);
              MeunShow(SetpGetPostion(),Rs485GetDate(2));
              break;
            case Drawer_out_place:
              Rs485Send(EquipGetAddr(),ManipuCheckLayer(Rs485GetDate(2),1),0,0,0,0,0,0,0,0,0,0);
              break;
            case Drawer_back:
              Rs485Send(EquipGetAddr(),ManipuDir(Rs485GetDate(2),0),0,0,0,0,0,0,0,0,0,0);
              MenuSetPage(0x06);
              MeunShow(SetpGetPostion(),Rs485GetDate(2));
              break;
            case Drawer_back_place:
              Rs485Send(EquipGetAddr(),ManipuCheckLayer(Rs485GetDate(2),0),0,0,0,0,0,0,0,0,0,0);
              break;
            case Button_detection:
              Rs485Send(EquipGetAddr(),ManipuBunRead(),0,0,0,0,0,0,0,0,0,0);
              break;
            case Button_led:
              EquipLed(Rs485GetDate(2));
              Rs485Send(EquipGetAddr(),TRUE,0,0,0,0,0,0,0,0,0,0);
              break;
            case Light:
              Rs485Send(EquipGetAddr(),ManipuLight(Rs485GetDate(2),Rs485GetDate(3)),0,0,0,0,0,0,0,0,0,0);
              break;
            case Check_num:
              Rs485Send(EquipGetAddr(),TRUE,ManipuGetNum(1),
                        ManipuGetNum(2),ManipuGetNum(3),
                        ManipuGetNum(4),ManipuGetNum(5),0,0,0,0,0);
              break;
            case Check_num_zero:
              Rs485Send(EquipGetAddr(),ManipuNumClear(),0,0,0,0,0,0,0,0,0,0);
              break;
            case Detection_barrier:
              ManipuSetShiel(Rs485GetDate(2));
              if(ManipuCheckOk() == 0xff)
              {
                Rs485Send(EquipGetAddr(),TRUE,ManipuCheckOk(),0,0,0,0,0,0,0,0,0);
              }
              else
              {
                Rs485Send(EquipGetAddr(),FALSE,ManipuCheckOk(),0,0,0,0,0,0,0,0,0);
              }
              break;
            default:
              Rs485Send(EquipGetAddr(),FALSE,0,0,0,0,0,0,0,0,0,0);
              break;
            }
        }
        else
        {
            Rs485Send(EquipGetAddr(),ERROR,0,0,0,0,0,0,0,0,0,0);//check fails
        }
        Rs485FlagClear();//Clear Flag
    }
  }
}
