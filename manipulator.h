

#ifndef MANIPULATOR_H
#define MANIPULATOR_H


#include "type.h"

void ManipuInit(void);
u8 ManipuDir(u8 bit,u8 dir);
u8 ManipuCheckOk(void);
u8 ManipuDir(u8 bit,u8 dir);
u8 ManipuCheckLayer(u8 cmd,u8 data);
u8 ManipuLight(u8 cmd,u8 data);

u8 ManipuBunRead(void);

u8 ManipuGetNum(u8 cmd);
u8 ManipuNumClear(void);


#endif
