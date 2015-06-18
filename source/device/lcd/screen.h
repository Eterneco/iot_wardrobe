#ifndef __SCREEN_H
#define __SCREEN_H

#include "typeDefs.h"

extern void  ScreenClear();

extern void  ScreenInfo();

extern void  ScreenApInit();
extern void  ScreenApError();
extern void  ScreenApError(char * ssid);
extern void  ScreenApSucces(char * ssid);

extern void  ScreenWeatherForecast();
extern void  ScreenWeatherForecast1(uInt8 idx);
extern void  ScreenWeatherForecast2(uInt8 idx);


#endif
