

#ifndef SETPPERMOTOR_H
#define SETPPERMOTOR_H

#include "type.h"

void SetpInit(void);
u8 SetpZero(void);
u8 SetpRotation(u8 tar_pos);
u8 SetpGetPostion(void);
void SetpSetEncoder(u8 cmd);
u8 SetpReadEncoder(void);

u8 SetpGetLockBit(void);

#endif
