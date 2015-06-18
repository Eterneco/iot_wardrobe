#ifndef __WARDROBE_H
#define __WARDROBE_H

#include "typeDefs.h"

#define WARDROBE_LED_CLR_TIME  60


extern volatile uInt8 g_wardrobeLedClrFlag;
extern volatile uInt8 g_wardrobeLedClrTime;

extern void WardrobeStateClr();
extern void WardrobeStateSet(uInt8 idx);
extern void WardrobeWork(cInt8 * acRecvbuff);

 
#endif
  
