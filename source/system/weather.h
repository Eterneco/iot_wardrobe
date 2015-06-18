#ifndef __WEATHER_FORECAST_H
#define __WEATHER_FORECAST_H

#include "typeDefs.h"


#define WEATHER_FORECAST_QNT   3


typedef struct
{
	cInt8    sunFrom [6];
	cInt8    sunTo   [6];

}twfInfo;



typedef struct
{
	cInt8    timeFrom[6];
	cInt8    timeTo  [6];
	uInt8	 nCnt;
	cInt8	 nValue1[15];
	cInt8	 nValue2[15];
	cInt8	 nValue3[15];
	uInt8 *  icon;
}twfDescr;


typedef struct
{
	  char	 descr[9];
      float  value;
      char	 unit[5];

}twfValue;


typedef struct
{
	twfDescr  info;

	twfValue  wind;
	twfValue  temp;
	twfValue  press;
	twfValue  humi;
	twfValue  clouds;
	twfValue  fall;
}twfData;

extern twfInfo  wfInfo;
extern twfData  tabWFData[WEATHER_FORECAST_QNT];

extern void  WeatherForecastGet(int iSockID, cInt8 * acSendBuff,cInt8 * acRecvbuff);
extern uInt8 WeatherForecastParse(cInt8 * acRecvbuff);

 
#endif
  
