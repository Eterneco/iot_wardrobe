#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// simplelink includes
#include "device.h"

// driverlib includes
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "interrupt.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "pin.h"
#include "timer.h"
#include "utils.h"
#include "pin_mux_config.h"
#include "system.h"
#include "screen.h"
#include "wardrobe.h"
#include "weather.h"
#include "delayDefs.h"


//Free_rtos/ti-rtos includes
#include "osi.h"


// common interface includes
#ifndef NOTERM
#include "uart_if.h"
#endif
#include "sw.h"
#include "led.h"
#include "lcd.h"
#include "gpio.h"
#include "gpio_if.h"
#include "timerA.h"
#include "timer_if.h"
#include "configure.h"
#include "uartA0.h"
#include "network_if.h"
#include "udma_if.h"



#define SLEEP_TIME              8000000
#define SPAWN_TASK_PRIORITY     9
#define OSI_STACK_SIZE          6000




// GLOBAL VARIABLES -- Start

cInt8          	acSendBuff[TX_BUFF_SIZE];
cInt8          	acRecvbuff[RX_BUFF_SIZE]; 	 // 4000
SlSecParams_t 	SecurityParams = {0};  // AP Security Parameters

uInt8			g_wfParserStatus	 = 0;
uInt8  			g_checkAccesPoint    = 0;


/********************************************************************************
   global procedures
********************************************************************************/


void SystemCheckAP()
{
    g_checkAccesPoint = 1;
}

static int CreateConnection(unsigned long ulDestinationIP)
{
    int iLenorError;
    SlSockAddrIn_t  sAddr;
    int iAddrSize;
    int iSockIDorError = 0;

    sAddr.sin_family = SL_AF_INET;
    sAddr.sin_port = sl_Htons(80);

    //Change the DestinationIP endianity , to big endian
    sAddr.sin_addr.s_addr = sl_Htonl(ulDestinationIP);

    iAddrSize = sizeof(SlSockAddrIn_t);

    //Create TCP socket, IPv4
    iSockIDorError = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);

    if( iSockIDorError < 0 )
    {
        DBG_PRINT("Error: Error Number = %d .\n\r", iSockIDorError );
        return iSockIDorError;
    }

    //Connect with server
    iLenorError = sl_Connect(iSockIDorError, ( SlSockAddr_t *)&sAddr, iAddrSize);
    if( iLenorError < 0 )
    {
        // error
        DBG_PRINT("Error: Error Number = %d.  \n\r", iLenorError );
        return iLenorError;
    }

    DBG_PRINT("Socket Id: %d was created.",iSockIDorError);

    return iSockIDorError;//success, connection created
}

static sInt16 Connect2AccessPoint()
{
    // Reset The state of the machine
    Network_IF_ResetMCUStateMachine();

    //
    // Start the driver
    Network_IF_InitDriver(ROLE_STA);

    // Initialize AP security params
    SecurityParams.Key = (signed char *) SECURITY_KEY;
    SecurityParams.KeyLen = strlen(SECURITY_KEY);
    SecurityParams.Type = SECURITY_TYPE;

    // Connect to the Access Point
    return(Network_IF_ConnectAP(SSID_NAME,SecurityParams));
}

/********************************************************************************
                       Info Screen
********************************************************************************/

uInt8 SystemInfoScreenTrigger()
{
     return Sw1State();
}

void SystemInfoScreenTask()
{
	uInt8 idx;

	LcdInit();
	ScreenInfo();

	if(g_wfParserStatus == 1)
	{

	    WardrobeStateClr();

		for(idx =0; idx <WEATHER_FORECAST_QNT; idx ++)
		{
			WardrobeStateSet(idx);
			ScreenWeatherForecast1(idx);
			ScreenWeatherForecast2(idx);
		}

	    g_wardrobeLedClrTime = WARDROBE_LED_CLR_TIME;
	    g_wardrobeLedClrFlag = 1;
	}

	LcdOff();
    DBG_PRINT("Info Screen Completed\n\r");
}


/********************************************************************************
                       Check Acces Point
********************************************************************************/

uInt8 SystemCheckAccesPointTrigger()
{
    return (g_checkAccesPoint);
}

void SystemCheckAccesPointTask()
{
    sInt16 apConnection;

    g_checkAccesPoint = 0;

    // view screen, blink on
    ScreenApInit();
    g_apBlink = 1; // on

    // Connect to specific AP
    apConnection = Connect2AccessPoint();
    g_apBlink = 0; // off

    if(apConnection < 0)
    {
        LedRedOn();
        DBG_PRINT("can't connect to %s AP",SSID_NAME);
        ScreenApError(SSID_NAME);
    }
    else
    {
        LedRedOff();
        DBG_PRINT("connected to %s AP",SSID_NAME);
        ScreenApSucces(SSID_NAME);
    }

    // Stop the driver
    Network_IF_DeInitDriver();
    DBG_PRINT("Check AP Completed\n\r");
}


/********************************************************************************
                       Normal Work
********************************************************************************/

uInt8 SystemNormalWorkTrigger()
{
     return Sw2State();
}


void SystemNormalWorkTask()
{
    int     iSocketDesc;
    sInt16  apConnection;

    		 long ulStatus;
    unsigned long ulDestinationIP;

    //
    ScreenWeatherForecast();
    g_wfBlink = 1; 	  // on
    g_wfParserStatus = 0;

    while(1)
    {

        // Connect to specific AP
        apConnection = Connect2AccessPoint();

        if(apConnection < 0)
        {
        	g_wfBlink = 0;
        	LedYellowOn();
            DBG_PRINT("can't connect to %s AP",SSID_NAME);
            ScreenApError(SSID_NAME);
            break;
        }
        else
        {
            LedYellowOff();
            DBG_PRINT("connected to %s AP",SSID_NAME);
        }

        // Get the serverhost IP address using the DNS lookup
        ulStatus = Network_IF_GetHostIP(WF_SERVER_NAME,&ulDestinationIP);
        if(ulStatus < 0)
        {
        	LedYellowOn();
            DBG_PRINT("DNS lookup failed. \n\r");
            break;
        }

        // Create a TCP connection to the server
        iSocketDesc = CreateConnection(ulDestinationIP);
        if(iSocketDesc < 0)
        {
        	LedYellowOn();
            DBG_PRINT("Socket creation failed.\n\r");
            break ;
        }

        WeatherForecastGet(iSocketDesc,acSendBuff,acRecvbuff);
        g_wfParserStatus = WeatherForecastParse(acRecvbuff);

        // Close the socket
        LedRedOff();
        LedYellowOff();
        close(iSocketDesc);
        DBG_PRINT("\n\rSocket closed\n\r");
        break;
    }

    g_wfBlink = 0; // blink off

    // Stop the driver
    Network_IF_DeInitDriver();
    DBG_PRINT("GET_WEATHER_FORECAST Completed\n\r");

    if(g_wfParserStatus)
       WardrobeWork(acRecvbuff);
}


//--------------------------------------------------------------------------------

void SystemTaskManager(void *pvParameters)
{

    while(1)
    {
           if(SystemCheckAccesPointTrigger())    SystemCheckAccesPointTask();
           if(SystemNormalWorkTrigger())         SystemNormalWorkTask();
           if(SystemInfoScreenTrigger())         SystemInfoScreenTask();

           MAP_UtilsDelay(SLEEP_TIME);
    }

}

void SystemTaskCreate()
{
    // Initializing DMA
    UDMAInit();


    // Start the SimpleLink Host
    //
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);

    //
    // Start the GetWeather task
    //
    osi_TaskCreate(SystemTaskManager,
                    (const signed char *)"system",
                    OSI_STACK_SIZE,
                    NULL,
                    1,
                    NULL );

}
