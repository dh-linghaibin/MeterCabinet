/*
*
*Version:1.1
*
*linghaibin Steven
*
*/
#include "manipulator.h"
#include "equipment.h"
#include "eeprom.h"
#include "rs485.h"
#include "lcd.h"
#include "setppermotor.h"

int main(void)
{
  EquipInit();//bsp init
  EquipAddrRead();//read the machine address
  ManipuInit();//manipulator init and AD init
  EeepromInit();//eeprom Unlock
  Rs485Init();//com init  and TIM3 init
  SetpInit();//setpipulator init and TIM3 init
  LcdInit();//12864 lcd init
  while(1)
  {
    ManipuBunRead();//Starts keys
    
    if(Rs485GetFlag() == 0x80)//Data sent over
    {
        if(Rs485Check() == CORRECT)//chaeck
        {
            Rs485Send(EquipGetAddr(),CORRECT,0,0,0,0,0,0,0,0,0,0);//Verification is successful
            switch(Rs485GetDate(1))
            {
            case Back_zero:
              if(ManipuCheckOk() == 0xff)
              {
                Rs485Send(EquipGetAddr(),SetpZero(),0,0,0,0,0,0,0,0,0,0);
              }
              else
              {
                Rs485Send(EquipGetAddr(),FALSE,0,0,0,0,0,0,0,0,0,0);
              }
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
              break;
            case Drawer_out:
              Rs485Send(EquipGetAddr(),ManipuDir(Rs485GetDate(2),1),0,0,0,0,0,0,0,0,0,0);
              break;
            case Drawer_out_place:
              Rs485Send(EquipGetAddr(),ManipuCheckLayer(Rs485GetDate(2),1),0,0,0,0,0,0,0,0,0,0);
              break;
            case Drawer_back:
              Rs485Send(EquipGetAddr(),ManipuDir(Rs485GetDate(2),0),0,0,0,0,0,0,0,0,0,0);
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
              Rs485Send(EquipGetAddr(),0,0,0,0,0,0,0,0,0,0,0);
              break;
            default:
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
