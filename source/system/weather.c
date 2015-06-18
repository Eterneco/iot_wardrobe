#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather.h"


// simplelink includes
#include "device.h"

// driverlib includes
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "system.h"
#include "screen.h"
#include "bitmaps.h"

// common interface includes
#include "configure.h"
#include "delayDefs.h"
#include "uartA0.h"

twfInfo  wfInfo;
twfData  tabWFData[WEATHER_FORECAST_QNT];		// weather forecast data, table


void WeatherForecastGet(int iSockID, cInt8 * acSendBuff,cInt8 * acRecvbuff)
{
    int iTXStatus;
    int iRXDataStatus;
    char* pcBufLocation;

    const char prefixBuffer[] = "GET /data/2.5/forecast?";
    const char postBuffer[]   = "&mode=xml&units=metric HTTP/1.1\r\nHost: api.openweathermap.org\r\nAccept: */";
    const char postBuffer2[]  = "*\r\n\r\n";


     memset(acRecvbuff, 0, RX_BUFF_SIZE);

    // Puts together the HTTP GET string.
    //
     pcBufLocation = acSendBuff;
     strcpy(pcBufLocation, prefixBuffer);
     pcBufLocation += strlen(prefixBuffer);

     #ifdef LOCATION_GPS
         int l_size;
         l_size = sprintf(pcBufLocation,"lat=%.6f&lon=%.6f",LOCATION_GPS_LAT,LOCATION_GPS_LON);
         pcBufLocation += l_size;
     #else
         strcpy(pcBufLocation,"q=");
         pcBufLocation += strlen("q=");
         strcpy(pcBufLocation,LOCATION_CITY_NAME);
         pcBufLocation += strlen(LOCATION_CITY_NAME);
     #endif

     strcpy(pcBufLocation, postBuffer);
     pcBufLocation += strlen(postBuffer);
     strcpy(pcBufLocation, postBuffer2);


    //
    // Send the HTTP GET string to the open TCP/IP socket.
    //
    DBG_PRINT("Sent HTTP GET request. \n\r");
    iTXStatus = sl_Send(iSockID, acSendBuff, strlen(acSendBuff), 0);
    MAP_UtilsDelay(DELAY_CLK_1_SEK *3);
    DBG_PRINT("Return value: %d \n\r", iTXStatus);

    //
    // Store the reply from the server in buffer.
    //
    DBG_PRINT("Received HTTP GET response data. \n\r");
    iRXDataStatus = sl_Recv(iSockID, &acRecvbuff[0], RX_BUFF_SIZE, 0);
    DBG_PRINT("Return value: %d \n\r", iRXDataStatus);
    acRecvbuff[RX_BUFF_SIZE - 1] = 0;

    DBG_PRINT(acRecvbuff);
}

void WeatherForecastIcon(int number, uInt8 idx)
{

    switch(number)
    {
       // thunderstorm

        case 200 :  // thunderstorm with light rain
        			tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"thunderstorm");
                    strcpy(tabWFData[idx].info.nValue2,"light rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf11Icon;
                    break;
        case 201 :  // thunderstorm with rain
        			tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"thunderstorm");
                    strcpy(tabWFData[idx].info.nValue2,"rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf11Icon;
                    break;
        case 202 :  // thunderstorm with heavy rain
        			tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"thunderstorm");
                    strcpy(tabWFData[idx].info.nValue2,"heavy rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf11Icon;
                    break;
        case 210 :  // light thunderstorm
					tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"light");
                    strcpy(tabWFData[idx].info.nValue2,"thunderstorm");
                    tabWFData[idx].info.icon = (uInt8 *)wf11Icon;
                    break;
        case 211 :  // thunderstorm
					tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"thunderstorm");
                    tabWFData[idx].info.icon = (uInt8 *)wf11Icon;
                    break;
        case 212 :  // heavy thunderstorm
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"heavy");
                    strcpy(tabWFData[idx].info.nValue2,"thunderstorm");
                    tabWFData[idx].info.icon = (uInt8 *)wf11Icon;
                    break;
        case 221 :  // ragged thunderstorm
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"ragged");
                    strcpy(tabWFData[idx].info.nValue2,"thunderstorm");
                    tabWFData[idx].info.icon = (uInt8 *)wf11Icon;
                    break;
        case 230 :  // thunderstorm with light drizzle
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"thunderstorm");
                    strcpy(tabWFData[idx].info.nValue2,"light drizzle");
                    tabWFData[idx].info.icon = (uInt8 *)wf11Icon;
                    break;
        case 231 :  // thunderstorm with drizzle
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"thunderstorm");
                    strcpy(tabWFData[idx].info.nValue2,"drizzle");
                    tabWFData[idx].info.icon = (uInt8 *)wf11Icon;
                    break;
        case 232 :  // thunderstorm with heavy drizzle
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"thunderstorm");
                    strcpy(tabWFData[idx].info.nValue2,"heavy drizzle");
                    tabWFData[idx].info.icon = (uInt8 *)wf11Icon;
                    break;


        // drizzle

        case 300 :  // light intensity drizzle
                    tabWFData[idx].info.nCnt = 3;
                    strcpy(tabWFData[idx].info.nValue1,"light");
                    strcpy(tabWFData[idx].info.nValue2,"intensity");
                    strcpy(tabWFData[idx].info.nValue3,"drizzle");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 301 :  // drizzle
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"drizzle");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 302 :  // heavy intensity drizzle
                    tabWFData[idx].info.nCnt = 3;
                    strcpy(tabWFData[idx].info.nValue1,"heavy");
                    strcpy(tabWFData[idx].info.nValue2,"intensity");
                    strcpy(tabWFData[idx].info.nValue3,"drizzle");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 310 :  // light intensity drizzle rain
                    tabWFData[idx].info.nCnt = 3;
                    strcpy(tabWFData[idx].info.nValue1,"light");
                    strcpy(tabWFData[idx].info.nValue2,"intensity");
                    strcpy(tabWFData[idx].info.nValue3,"drizzle rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 311 :  // drizzle rain
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"drizzle rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 312 :  // heavy intensity drizzle rain
                    tabWFData[idx].info.nCnt = 3;
                    strcpy(tabWFData[idx].info.nValue1,"heavy");
                    strcpy(tabWFData[idx].info.nValue2,"intensity");
                    strcpy(tabWFData[idx].info.nValue3,"drizzle rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 313 :  // shower rain and drizzle
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"shower rain");
                    strcpy(tabWFData[idx].info.nValue2,"and drizzle");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 314 :  // heavy shower rain and drizzle
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"heavy shower");
                    strcpy(tabWFData[idx].info.nValue2,"rain drizzle");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 321 :  // shower drizzle
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"shower");
                    strcpy(tabWFData[idx].info.nValue2,"drizzle");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;

        // rain

        case 500 :  // light rain
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"light rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf10Icon;
                    break;
        case 501 :  // moderate rain
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"moderate");
                    strcpy(tabWFData[idx].info.nValue2,"rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf10Icon;
                    break;
        case 502 :  //heavy intensity rain
                    tabWFData[idx].info.nCnt = 3;
                    strcpy(tabWFData[idx].info.nValue1,"heavy");
                    strcpy(tabWFData[idx].info.nValue2,"intensity");
                    strcpy(tabWFData[idx].info.nValue3,"rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf10Icon;
                    break;
        case 503 :  //very heavy rain
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"very heavy");
                    strcpy(tabWFData[idx].info.nValue2,"rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf10Icon;
                    break;
        case 504 :  //extreme rain
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"extreme rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf10Icon;
                    break;
        case 511 :  //freezing rain
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"freezing");
                    strcpy(tabWFData[idx].info.nValue2,"rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;
        case 520 :  //light intensity shower rain
                    tabWFData[idx].info.nCnt = 3;
                    strcpy(tabWFData[idx].info.nValue1,"light");
                    strcpy(tabWFData[idx].info.nValue2,"intensity");
                    strcpy(tabWFData[idx].info.nValue3,"shower rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 521 :  //shower rain
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"shower rain ");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 522 :  //heavy intensity shower rain
                    tabWFData[idx].info.nCnt = 3;
                    strcpy(tabWFData[idx].info.nValue1,"heavy");
                    strcpy(tabWFData[idx].info.nValue2,"intensity");
                    strcpy(tabWFData[idx].info.nValue3,"shower rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;
        case 531 :  //ragged shower rain
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"light");
                    strcpy(tabWFData[idx].info.nValue2,"shower rain");
                    tabWFData[idx].info.icon = (uInt8 *)wf09Icon;
                    break;

        // snow

        case 600 :  //light snow
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"light snow");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;
        case 601 :  //snow
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"snow");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;
        case 602 :  //heavy snow
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"heavy snow");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;
        case 611 :  //sleet
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"sleet");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;
        case 612 :  //shower sleet
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"shower sleet");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;
        case 615 :  //light rain and snow
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"light rain");
                    strcpy(tabWFData[idx].info.nValue2,"and snow");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;
        case 616 :  //rain and snow
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"rain");
                    strcpy(tabWFData[idx].info.nValue2,"and snow");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;
        case 620 :  //light shower snow
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"light");
                    strcpy(tabWFData[idx].info.nValue2,"shower snow");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;
        case 621 :  //shower snow
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"shower snow");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;
        case 622 :  //heavy shower snow
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"heavy");
                    strcpy(tabWFData[idx].info.nValue2,"shower snow");
                    tabWFData[idx].info.icon = (uInt8 *)wf13Icon;
                    break;

        // atmosphere

        case 701 :  //mist
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"mist");
                    tabWFData[idx].info.icon = (uInt8 *)wf50Icon;
                    break;
        case 711 :  //smoke
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"smoke");
                    tabWFData[idx].info.icon = (uInt8 *)wf50Icon;
                    break;
        case 721 :  //haze
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"haze");
                    tabWFData[idx].info.icon = (uInt8 *)wf50Icon;
                    break;
        case 731 :  //sand, dust whirls
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"sand");
                    strcpy(tabWFData[idx].info.nValue1,"dust whirls");
                    tabWFData[idx].info.icon = (uInt8 *)wf50Icon;
                    break;
        case 741 :  //fog
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"fog");
                    tabWFData[idx].info.icon = (uInt8 *)wf50Icon;
                    break;
        case 751 :  //sand
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"sand");
                    tabWFData[idx].info.icon = (uInt8 *)wf50Icon;
                    break;
        case 761 :  //dust
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"dust");
                    tabWFData[idx].info.icon = (uInt8 *)wf50Icon;
                    break;
        case 762 :  //volcanic ash
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"volcanic ash");
                    tabWFData[idx].info.icon = (uInt8 *)wf50Icon;
                    break;
        case 771 :  //squalls
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"squalls");
                    tabWFData[idx].info.icon = (uInt8 *)wf50Icon;
                    break;
        case 781 :  //tornado
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"tornado");
                    tabWFData[idx].info.icon = (uInt8 *)wfTornadoIcon;
                    break;

        // clouds

        case 800 :  //clear sky
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"clear sky");
                    tabWFData[idx].info.icon = (uInt8 *)wf01Icon;
                    break;
        case 801 :  //few clouds
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"few clouds");
                    tabWFData[idx].info.icon = (uInt8 *)wf02Icon;
                    break;
        case 802 :  //scattered clouds
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"scattered");
                    strcpy(tabWFData[idx].info.nValue2,"clouds");
                    tabWFData[idx].info.icon = (uInt8 *)wf03Icon;
                    break;
        case 803 :  //broken clouds
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"broken");
                    strcpy(tabWFData[idx].info.nValue2,"clouds");
                    tabWFData[idx].info.icon = (uInt8 *)wf03Icon;
                    break;
        case 804 :  //overcast clouds
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"overcast");
                    strcpy(tabWFData[idx].info.nValue2,"clouds");
                    tabWFData[idx].info.icon = (uInt8 *)wf04Icon;
                    break;

        // extreme

        case 900 :  //tornado
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"tornado");
                    tabWFData[idx].info.icon = (uInt8 *)wfTornadoIcon;
                    break;
        case 901 :  //tropical storm
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"tropical storm");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 902 :  //hurricane
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"hurricane");
                    tabWFData[idx].info.icon = (uInt8 *)wfHurricaneIcon;
                    break;
        case 903 :  //cold
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"cold");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 904 :  //hot
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"hot");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 905 :  //windy
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"windy");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 906 :  //hail
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"hail");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;

        // additional

        case 951 :  //calm
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"calm");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 952 :  //light breeze
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"light breeze");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 953 :  //gentle breeze
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"gentle breeze");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 954 :  //moderate breeze
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"moderate");
                    strcpy(tabWFData[idx].info.nValue2,"breeze");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 955 :  //fresh breeze
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"fresh breeze");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 956 :  //strong breeze
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"strong breeze");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 957 :  //high wind, near gale
                    tabWFData[idx].info.nCnt = 2;
                    strcpy(tabWFData[idx].info.nValue1,"high wind");
                    strcpy(tabWFData[idx].info.nValue2,"near gale");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 958 :  //gale
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"gale");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 959 :  //severe gale
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"severe gale");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 960 :  //storm
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"storm");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 961 :  //violent storm
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"violent storm");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
        case 962 :  //hurricane
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"hurricane");
                    tabWFData[idx].info.icon = (uInt8 *)wfHurricaneIcon;
                    break;

    default:
                    tabWFData[idx].info.nCnt = 1;
                    strcpy(tabWFData[idx].info.nValue1,"unknown");
                    tabWFData[idx].info.icon = (uInt8 *)wfNaIcon;
                    break;
    }
}



uInt8 WeatherForecastParse(cInt8 * acRecvbuff)
{
   cInt8 * l_ptrS;
   cInt8 * l_ptrE;
   cInt8 * l_ptr = acRecvbuff;

   uInt8 idx;
   int   l_number;

   // watherdata
   l_ptr = strstr(acRecvbuff,"<weatherdata>");
   if(l_ptr == 0) return 0;

   // sun rise
   l_ptr = strstr(l_ptr,"<sun rise=");
   if(l_ptr == 0)             return 0;
   l_ptr  += strlen("<sun rise=");

   // from
   l_ptr = strstr(l_ptr,"T");
   if(l_ptr == 0)             return 0;
   l_ptr = l_ptr + 1;
   strncpy(wfInfo.sunFrom,l_ptr,5);


   //to
   l_ptr = strstr(l_ptr,"T");
   if(l_ptr == 0)             return 0;
   l_ptr = l_ptr + 1;
   strncpy(wfInfo.sunTo,l_ptr,5);

   // forecast
   l_ptr = strstr(acRecvbuff,"<forecast>");
   if(l_ptr == 0) return 0;


   for(idx=0; idx < WEATHER_FORECAST_QNT; idx++)
   {

	   l_ptrS = l_ptr;

	   // time
       l_ptr = strstr(l_ptr,"<time from=");
       if(l_ptr == 0)             return 0;
       l_ptr  += strlen("<time from=");

	   // from
	   l_ptr = strstr(l_ptr,"T");
	   if(l_ptr == 0)             return 0;
	   l_ptr = l_ptr + 1;
	   strncpy(tabWFData[idx].info.timeFrom,l_ptr,5);


	   //to
	   l_ptr = strstr(l_ptr,"T");
	   if(l_ptr == 0)             return 0;
	   l_ptr = l_ptr + 1;
	   strncpy(tabWFData[idx].info.timeTo,l_ptr,5);

	   // symbol number
	   l_ptr = strstr(l_ptr,"number=\"");
	   if(l_ptr == 0)             return 0;
	   l_ptr  += strlen("number=\"");

	   sscanf(l_ptr,"%d",&l_number);
	   WeatherForecastIcon(l_number, idx);


	   // wind
	   l_ptr = strstr(l_ptr,"<windSpeed mps=\"");
	   if(l_ptr == 0) return 0;
	   l_ptr += strlen("<windSpeed mps=\"");

	   strcpy(tabWFData[idx].wind.descr,"wind ");
	   sscanf(l_ptr,"%f",&tabWFData[idx].wind.value);
	   strcpy(tabWFData[idx].wind.unit," mps");

	   // temperature
	   l_ptr = strstr(l_ptr,"temperature");
	   if(l_ptr == 0)             return 0;
	   l_ptr = strstr(l_ptr,"value=\"");
	   if(l_ptr == 0)         return 0;
	   l_ptr += strlen("value=\"");

	   strcpy(tabWFData[idx].temp.descr,"T= ");
	   sscanf(l_ptr,"%f",&tabWFData[idx].temp.value);
	   strcpy(tabWFData[idx].temp.unit," C");

	   // pressure
	   l_ptr = strstr(l_ptr,"pressure");
	   if(l_ptr == 0)             return 0;
	   l_ptr = strstr(l_ptr,"value=\"");
	   if(l_ptr == 0)         return 0;
	   l_ptr += strlen("value=\"");

	   strcpy(tabWFData[idx].press.descr,"P= ");
	   sscanf(l_ptr,"%f",&tabWFData[idx].press.value);
	   strcpy(tabWFData[idx].press.unit," hPa");

	   // humidity
	   l_ptr = strstr(l_ptr,"humidity");
	   if(l_ptr == 0)             return 0;
	   l_ptr = strstr(l_ptr,"value=\"");
	   if(l_ptr == 0)         return 0;
	   l_ptr += strlen("value=\"");

	   strcpy(tabWFData[idx].humi.descr,"H= ");
	   sscanf(l_ptr,"%f",&tabWFData[idx].humi.value);
	   strcpy(tabWFData[idx].humi.unit,"%");

	   // clouds
	   l_ptr = strstr(l_ptr,"clouds");
	   if(l_ptr == 0)             return 0;
	   l_ptr = strstr(l_ptr,"all=\"");
	   if(l_ptr == 0)         return 0;
	   l_ptr += strlen("all=\"");

	   strcpy(tabWFData[idx].clouds.descr,"clouds ");
	   sscanf(l_ptr,"%f",&tabWFData[idx].clouds.value);
	   strcpy(tabWFData[idx].clouds.unit,"%");


	   // falls
	   l_ptrS  = strstr(l_ptrS,"<precipitation");
	   if(l_ptr == 0)             return 0;

	   l_ptrE = strstr(l_ptrS,"/>");
	   if(l_ptrE == 0)          return 0;
	   *l_ptrE = 0;

	   // no fall
	   strcpy(tabWFData[idx].fall.descr,"no fall");

	   // optional rain

	   if(strstr(l_ptrS,"type=\"rain\"") !=0)
	   {
		   l_ptrS = strstr(l_ptrS,"value=\"");
		   l_ptrS += strlen("value=\"");

		   strcpy(tabWFData[idx].fall.descr,"rain 3h ");
		   sscanf(l_ptrS,"%f",&tabWFData[idx].fall.value);
		   strcpy(tabWFData[idx].fall.unit,"mm");
		}

	   // optional snow

	   if(strstr(l_ptrS,"type=\"snow\"") !=0)
	   {
		   l_ptrS = strstr(l_ptrS,"value=\"");
		   l_ptrS += strlen("value=\"");

		   strcpy(tabWFData[idx].fall.descr,"snow 3h ");
		   sscanf(l_ptrS,"%f",&tabWFData[idx].fall.value);
		   strcpy(tabWFData[idx].fall.unit,"mm");
		}
	   *l_ptrE = '/';
   }

   return 1;
}

