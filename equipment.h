

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "type.h"

void EquipInit(void);

void EquipAddrRead(void);

u8 EquipGetAddr(void);

void EquipLed(u8 data);

#endif
