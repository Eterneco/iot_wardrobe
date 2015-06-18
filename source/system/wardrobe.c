#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// simplelink includes
#include "device.h"

// driverlib includes
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include "rom.h"
#include "rom_map.h"
#include "pin.h"
#include "gpio.h"
#include "utils.h"
#include "system.h"
#include "lcd.h"
#include "screen.h"
#include "weather.h"
#include "wardrobe.h"



// common interface includes
#include "configure.h"
#include "delayDefs.h"
#include "uartA0.h"


volatile uInt8 g_wardrobeLedClrFlag  = 0;
volatile uInt8 g_wardrobeLedClrTime  = 0;


// GPIO
#define        LED_1_GP                28   // (PIN_18) led 1
#define        LED_2_GP                17   // (PIN_08) led 2
#define        LED_3_GP                31   // (PIN_45) led 3


// PORT _ A0,A1,A2,A3 = (GPIO / 8)
#define        LED_1_PORT           GPIOA3_BASE   // 28/ 8 = 3
#define        LED_2_PORT           GPIOA2_BASE   // 17/ 8 = 2
#define        LED_3_PORT           GPIOA3_BASE   // 31/ 8 = 3

//  PIN _ 1,2,3,4,5,6,7,,,, = 1 << (GPIO % 8)
#define        LED_1_PIN        (1<< (LED_1_GP%8))
#define        LED_2_PIN     	(1<< (LED_2_GP%8))
#define        LED_3_PIN     	(1<< (LED_3_GP%8))


void WardrobeStateClr()
{
	g_wardrobeLedClrFlag = 0;

	GPIOPinWrite(LED_1_PORT, LED_1_PIN,0x00);
	GPIOPinWrite(LED_2_PORT, LED_2_PIN,0x00);
	GPIOPinWrite(LED_3_PORT, LED_3_PIN,0x00);
}



void WardrobeStateSet(uInt8 idx)
{

	//
	// 1 - umbrella / parasol
	if(strcmp(tabWFData[idx].fall.descr,"rain 3h ")==0)
			GPIOPinWrite(LED_1_PORT, LED_1_PIN, LED_1_PIN);


	// 2- scarf / szalik
	if(tabWFData[idx].temp.value < 5.0 )
			GPIOPinWrite(LED_2_PORT, LED_2_PIN, LED_2_PIN);

	// 3 - sunglasses / okulary przeciws³oneczne
	if(tabWFData[idx].clouds.value < 10)
		if(strcmp(tabWFData[idx].info.timeFrom,wfInfo.sunFrom) >= 0)
			if(strcmp(tabWFData[idx].info.timeTo,wfInfo.sunTo) <= 0)
				GPIOPinWrite(LED_3_PORT, LED_3_PIN, LED_3_PIN);

}

void WardrobeWork(cInt8 * acRecvbuff)
{
    uInt8   idx;

    WardrobeStateClr();

    for(idx =0; idx <WEATHER_FORECAST_QNT; idx ++)
    {
    	WardrobeStateSet(idx);
        ScreenWeatherForecast1(idx);
        ScreenWeatherForecast2(idx);
    }

    g_wardrobeLedClrTime = WARDROBE_LED_CLR_TIME;
    g_wardrobeLedClrFlag = 1;

    LcdOff();
}
