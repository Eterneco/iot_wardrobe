#include <math.h>
#include "typeDefs.h"
#include "timerA.h"  
#include "system.h"

#include "hw_types.h"
#include "hw_memmap.h"
#include "gpio.h"
#include "pin.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"

#include "timer.h"
#include "timer_if.h"
#include "gpio_if.h"
#include "wardrobe.h"

#include "lcd.h"
#include "bitmaps.h"


// definitions
#define TIMER_FREQ        80000000
#define TIMERA0_INTERVAL        2


///////// blink flags //////////
// ap
volatile uInt8 g_apBlink   = 0;
volatile uInt8 apBlinkChk  = 0;
// wf
volatile uInt8 g_wfBlink   = 0;
volatile uInt8 wfBlinkChk  = 0;


void TimerA0IntHandler(void); // blink



 //*****************************************************************************
//
//! TimerA0 interrupt handler
//
//*****************************************************************************                                                                 
void TimerA0Init()                                            
{    
  Timer_IF_Init    (PRCM_TIMERA0, TIMERA0_BASE, TIMER_CFG_PERIODIC, TIMER_A, 0);
  Timer_IF_IntSetup(TIMERA0_BASE, TIMER_A, TimerA0IntHandler);    
  
  Timer_IF_Start(TIMERA0_BASE, TIMER_A, TIMER_FREQ / TIMERA0_INTERVAL);
}                                                                                                                                                                                                                                                                                                                                                    


//*****************************************************************************
//
//! TimerA0
//
//*****************************************************************************
void TimerA0IntHandler(void)
{
  Timer_IF_InterruptClear(TIMERA0_BASE);

  if(g_apBlink == 1)
  {
     if(apBlinkChk) LcdBitmapa(1,24,apInitBlink1); // on
     else           LcdBitmapa(1,24,apInitBlink2); // off               
       
     apBlinkChk ^= 1;
  }
  
   if(g_wfBlink == 1)
  {
     if(wfBlinkChk) LcdBitmapa(2,23, wfInitBlink); 
     else           LcdClear  (2,23, 4, 36);              
       
     wfBlinkChk ^= 1;
  }

   if(g_wardrobeLedClrFlag == 1)
   {
	   if(g_wardrobeLedClrTime > 0 ) g_wardrobeLedClrTime -= 1;
	   else WardrobeStateClr();
    }


}
