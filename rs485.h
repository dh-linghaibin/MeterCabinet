

#ifndef RS485_H
#define RS485_H

#include "type.h"


#define Back_zero           0x01 /*回零*/
#define Motor_rotation      0x02 /*旋转*/
#define Drawer_out          0x03 /*机械手出去*/
#define Drawer_out_place    0x04 /*机械手出去到位*/
#define Drawer_back         0x05 /*机械手回来*/
#define Drawer_back_place   0x06 /*机械手回来到位*/
#define Button_detection 	0x07 /*按钮是否按下*/
#define Button_led       	0x08 /*按钮指示灯*/
#define Light              	0x09 /*照明灯*/
#define Check_num           0x0a //查询机械手使用次数
#define Check_num_zero      0x0b //查询机械手使用次数
#define Detection_barrier   0x0c /*检测阻挡*/

#define CORRECT				0x88 //上行或下行。表示通讯成功
#define ERROR			    0x44 //上行或下行。表示通讯出错
#define TRUE                0x21
#define FALSE 			    0x22

void Rs485Init(void);
void Rs485Send(u8 addr,u8 cmd,
               u8 dat1,u8 dat2,
               u8 dat3,u8 dat4,
               u8 dat5,u8 dat6,
               u8 dat7,u8 dat8,
               u8 dat9,u8 dat10);
u8 Rs485GetFlag(void);
void Rs485FlagClear(void);
u8 Rs485Check(void);
u8 Rs485GetDate(u8 num);
void Rs485Int(u8 mode);

#endif
