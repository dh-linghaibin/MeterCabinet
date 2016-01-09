

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "type.h"

void EquipInit(void);

void EquipAddrRead(void);

u8 EquipGetAddr(void);

void EquipLed(u8 data);

u8 EquipButonLeft(void);
u8 EquipButonRight(void);
u8 EquipButonUp(void);
u8 EquipButonDown(void);
u8 EquipButonOk(void);

#endif
