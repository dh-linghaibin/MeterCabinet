

#ifndef RS485_H
#define RS485_H

#include "type.h"


#define Back_zero           0x01 /*����*/
#define Motor_rotation      0x02 /*��ת*/
#define Drawer_out          0x03 /*��е�ֳ�ȥ*/
#define Drawer_out_place    0x04 /*��е�ֳ�ȥ��λ*/
#define Drawer_back         0x05 /*��е�ֻ���*/
#define Drawer_back_place   0x06 /*��е�ֻ�����λ*/
#define Button_detection 	0x07 /*��ť�Ƿ���*/
#define Button_led       	0x08 /*��ťָʾ��*/
#define Light              	0x09 /*������*/
#define Check_num           0x0a //��ѯ��е��ʹ�ô���
#define Check_num_zero      0x0b //��ѯ��е��ʹ�ô���
#define Detection_barrier   0x0c /*����赲*/

#define CORRECT				0x88 //���л����С���ʾͨѶ�ɹ�
#define ERROR			    0x44 //���л����С���ʾͨѶ����
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
