

#ifndef MENU_H
#define MENU_h

#include "type.h"

void MeunShow(u8 cmd, u8 data);
void MenuSetPage(u8 num);
u8 MenReadPage(void);
void MenuSetRow(u8 num);
u8 MenReadRow(void);
void MenuSetOk(u8 num);
u8 MenReadOk(void);

void MenuAddRotate(void);
void MenuSubRotate(void);
void MenuAddDoor(void);
void MenuSubDoor(void);

u8 MenuGetRotate(void);
u8 MenuGetDoor(void);

#endif

