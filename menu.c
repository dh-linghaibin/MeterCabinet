


#include "menu.h"
#include "rs485.h"
#include "lcd.h"
#include "equipment.h"
#include "setppermotor.h"

void MeunShow()
{
    u8 page;
    u8 status;
    switch(page)
    {
    case 0:
      switch(page)
      {
        case Back_zero:
            LCDString(0,0,"  ",0);
            LCDString(3,0,"  ",0);
            LCDString(1,0,"回",1);
            LCDString(2,0,"零",1);
            LCDString(1,1,"  当前位置:",1);
            LCDNum(EquipGetAddr()/10);
            LCDNum(EquipGetAddr()%10);
            LCDString(2,1,"  旋转位置:01 ",1);

            LCDString(1,1,"  抽屉位置:",1);
            LCDNum(SetpGetPostion()/10);
            LCDNum(SetpGetPostion()%10);
            break;
        case Motor_rotation:
            LCDString(0,0,"  ",0);
            LCDString(3,0,"  ",0);
            LCDString(1,0,"旋",1);
            LCDString(2,0,"转",1);
            LCDString(0,1,"              ",1);
            LCDString(1,1,"  当前位置:",1);
            LCDNum(SetpGetPostion()/10);
            LCDNum(SetpGetPostion()%10);

            LCDString(2,1,"  旋转位置:",1);
            LCDNum(SetpGetPostion()/10);
            LCDNum(SetpGetPostion()%10);
            break;
        case Drawer_out:
        break;
        case Drawer_out_place:
        break;
        case Drawer_back:
        break;
        case Drawer_back_place:
        break;
        case Button_detection:
        break;
        case Button_led:
        break;
        case Light:
        break;
        case Check_num:
        break;
        case Check_num_zero:
        break;
        case Detection_barrier:
            LCDString(0,0,"检",1);
            LCDString(1,0,"测",1);
            LCDString(2,0,"阻",1);
            LCDString(3,0,"档",1);
            //if(dr_num == 0)
            // {
            LCDString(1,1,"  无阻挡正常 ",1);
            //}
            // else
            //
            LCDString(1,1,"  有阻挡异常 ",1);
            // }
            break;
        default:
        break;
      }
      break;
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
      break;
    }
}