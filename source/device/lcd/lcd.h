#ifndef __PCD8544_H
#define __PCD8544_H

#include "typeDefs.h"

extern void  LcdConfigure();
extern void  LcdInit();
extern void  LcdOn();
extern void  LcdOff();

extern void  LcdClear  (uInt8 w, uInt8 k, uInt8 wL, uInt8 kL);

extern void  LcdChrWK  (uInt8 w, uInt8 k, uInt8 ch);

extern void  LcdStrWK  (uInt8 w, uInt8 k, cInt8 *dataPtr);
extern void  LcdStrWKn (uInt8 w, uInt8 k, cInt8 *dataPtr, uInt8 n);
extern uInt8 LcdStrWKnc(uInt8 w, uInt8 k, cInt8 *dataPtr, uInt8 n, uInt8 c); 

extern void  LcdBitmapa (uInt8 x, uInt8 y, const uInt8 * bitmapa);



#endif

