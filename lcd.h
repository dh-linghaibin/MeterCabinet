

#ifndef LCD_H
#define LCD_H

#include "type.h"

void LcdInit(void);
void LCDString(u8 x,u8 y,u8 *p,u8 time);
void LCDNum(u8 num);

#endif
