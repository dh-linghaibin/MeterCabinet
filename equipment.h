

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "type.h"

void EquipInit(void);

void EquipAddrRead(void);

u8 EquipGetAddr(void);

void EquipLed(u8 data);

u8 EquipButon1(void);
u8 EquipButon2(void);

#endif
