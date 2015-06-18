#ifndef __CONFIGURE_H
#define __CONFIGURE_H

#include "typeDefs.h"

/********************************************************************************
    network connection
********************************************************************************/
#define SSID_NAME               "HERE_AP_SSID"                /* AP SSID */
#define SECURITY_TYPE            SL_SEC_TYPE_WPA_WPA2         /* Security type (OPEN or WEP or WPA)*/
#define SECURITY_KEY            "AP_PASSWORD"                 /* Password of the secured AP */


/********************************************************************************
    wardrobe location
********************************************************************************/

#ifdef  LOCATION_GPS

	#define LOCATION_GPS_LAT	    "53.132309"
	#define LOCATION_GPS_LON	    "23.154518"

#else
	#define LOCATION_CITY_NAME      "BIALYSTOK"
#endif


/********************************************************************************
     WEATHER FORECAST
********************************************************************************/

#define WF_SERVER_NAME         "openweathermap.org"



#endif
