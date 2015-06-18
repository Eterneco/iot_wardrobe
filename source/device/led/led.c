#include "led.h"

#include "hw_types.h"
#include "hw_memmap.h"

#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "gpio.h"



// GPIO
#define        RED_GP                    9   // (PIN_64) red
#define        YELLOW_GP                10   // (PIN_01) yellow


// PORT _ A0,A1,A2,A3 = (GPIO / 8)
#define        RED_PORT              GPIOA1_BASE   //  9/ 8 = 1
#define        YELLOW_PORT           GPIOA1_BASE   // 10/ 8 = 1


//  PIN _ 1,2,3,4,5,6,7,,,, = 1 << (GPIO % 8)
#define        RED_PIN        (1<< (RED_GP%8))      
#define        YELLOW_PIN     (1<< (YELLOW_GP%8))   




void LedRedOn()
{
    GPIOPinWrite(RED_PORT, RED_PIN,RED_PIN);    
}

void LedYellowOn()
{
    GPIOPinWrite(YELLOW_PORT, YELLOW_PIN,YELLOW_PIN);   
}

 void LedRedOff()
{
    GPIOPinWrite(RED_PORT, RED_PIN,0x00);               
}

 void LedYellowOff()
{
    GPIOPinWrite(YELLOW_PORT, YELLOW_PIN,0x00);                
}
