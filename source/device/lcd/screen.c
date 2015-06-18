#include "screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lcd.h"
#include "fonts.h"
#include "bitmaps.h"
#include "weather.h"
#include "delayDefs.h"
#include "configure.h"

#include "rom_map.h"
#include "utils.h"





// --------- SCREENS -----------
// ------------------------------

void ScreenClear()
{
   LcdClear(0,0,6,84);
}

// welcome
void ScreenInfo()
{
   LcdBitmapa(0,0,info);
   MAP_UtilsDelay(DELAY_CLK_1_SEK *5);
}

// AP Init
void ScreenApInit()
{
   LcdBitmapa(0,0,apInit);
   MAP_UtilsDelay(DELAY_CLK_1_SEK);
}

// AP Error
void ScreenApError(char * ssid)
{
   LcdBitmapa(0,0,apError);
   LcdStrWK(5,6, ssid); 
   MAP_UtilsDelay(DELAY_CLK_1_SEK *2);
   LcdOff();
}

// AP Succes
void ScreenApSucces(char * ssid)
{
   LcdBitmapa(0,0,apSucces);
   LcdStrWK(5,6, ssid); 
   MAP_UtilsDelay(DELAY_CLK_1_SEK *2);
   LcdOff();
}


// Weather Forecast
void ScreenWeatherForecast()
{
   LcdOn();
   LcdBitmapa(0,0,weatherForecast);
   MAP_UtilsDelay(DELAY_CLK_1_SEK * 1);
}



// WeatherForecast1
void ScreenWeatherForecast1(uInt8 idx)
{
   uInt8   l_index;
   cInt8 * l_name[3];

   l_name[0]=tabWFData[idx].info.nValue1;
   l_name[1]=tabWFData[idx].info.nValue2;
   l_name[2]=tabWFData[idx].info.nValue3;
   
   ScreenClear();

   LcdStrWKn(0, 6, tabWFData[idx].info.timeFrom, 5);
   LcdStrWKn(0, 6 + 6*5, "-",1);
   LcdStrWKn(0, 6 + 6*5 + 6,tabWFData[idx].info.timeTo,5);

   LcdBitmapa(1, 24, tabWFData[idx].info.icon);

   for(l_index=0; l_index< tabWFData[idx].info.nCnt; l_index++)
   {
      LcdClear(5, 0,1,84);
      LcdStrWK (5, 0, l_name[l_index]);
      MAP_UtilsDelay(DELAY_CLK_1_SEK );
   }
   
   MAP_UtilsDelay(DELAY_CLK_1_SEK *3);
}

// WeatherForecast2
void ScreenWeatherForecast2(uInt8 idx)
{
   int   l_size;
   cInt8 value[15];
   
   ScreenClear();
   
   // wind  
   LcdStrWK(0, 0,tabWFData[idx].wind.descr);
   l_size = sprintf(value,"%.2f",tabWFData[idx].wind.value);
   LcdStrWK(0, 6* strlen(tabWFData[idx].wind.descr),value);
   LcdStrWK(0, 6* strlen(tabWFData[idx].wind.descr) + 6*l_size,tabWFData[idx].wind.unit);
   
   // temperature
   LcdStrWK(1, 0,tabWFData[idx].temp.descr);
   l_size = sprintf(value,"%.2f",tabWFData[idx].temp.value);
   LcdStrWK(1, 6* strlen(tabWFData[idx].temp.descr),value);
   LcdStrWK(1, 6* strlen(tabWFData[idx].temp.descr) + 6*l_size,tabWFData[idx].temp.unit);
   
   // pressure  
   LcdStrWK(2, 0,tabWFData[idx].press.descr);
   l_size = sprintf(value,"%.2f",tabWFData[idx].press.value);
   LcdStrWK(2, 6* strlen(tabWFData[idx].press.descr),value);
   LcdStrWK(2, 6* strlen(tabWFData[idx].press.descr) + 6*l_size,tabWFData[idx].press.unit);
   
   // humidity  
   LcdStrWK(3, 0,tabWFData[idx].humi.descr);
   l_size = sprintf(value,"%.2f",tabWFData[idx].humi.value);
   LcdStrWK(3, 6* strlen(tabWFData[idx].humi.descr),value);
   LcdStrWK(3, 6* strlen(tabWFData[idx].humi.descr) + 6*l_size,tabWFData[idx].humi.unit);
   
   // clouds  
   LcdStrWK(4, 0,tabWFData[idx].clouds.descr);
   l_size = sprintf(value,"%.2f",tabWFData[idx].clouds.value);
   LcdStrWK(4, 6* strlen(tabWFData[idx].clouds.descr),value);
   LcdStrWK(4, 6* strlen(tabWFData[idx].clouds.descr) + 6*l_size,tabWFData[idx].clouds.unit);
   
   // falls
   LcdStrWK(5, 0,tabWFData[idx].fall.descr);
   if(strcmp("no fall",tabWFData[idx].fall.descr) !=0)
   {
	   l_size = sprintf(value,"%.2f",tabWFData[idx].fall.value);
	   LcdStrWK(5, 6* strlen(tabWFData[idx].fall.descr),value);
	   LcdStrWK(5, 6* strlen(tabWFData[idx].fall.descr) + 6*l_size,tabWFData[idx].fall.unit);
   }
   
   MAP_UtilsDelay(DELAY_CLK_1_SEK *6);
}
