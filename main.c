#include <stdio.h>
#include "hw_types.h"
#include "interrupt.h"
#include "hw_ints.h"
#include "pin.h"
#include "hw_apps_rcm.h"
#include "hw_common_reg.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "hw_memmap.h"
#include "timer.h"
#include "utils.h"
#include "timer_if.h"
#include "gpio_if.h"
#include "pin_mux_config.h"
#include "system.h"

#include "led.h"
#include "lcd.h"
#include "screen.h"
#include "timerA.h"
#include "uartA0.h"
#include "typeDefs.h"


#include "osi.h"



extern void (* const g_pfnVectors[])(void);


static void BoardInit(void)
{

  MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);

  MAP_IntMasterEnable();
  MAP_IntEnable(FAULT_SYSTICK);

  PRCMCC3200MCUInit();
}


void main(void)
{
   BoardInit();
   PinMuxConfig();

   TimerA0Init();
   UartA0Init();

   LcdConfigure();

   ScreenInfo();
   SystemCheckAP();

   // create task
   SystemTaskCreate();


   // start the task scheduler
   osi_start();

   while(1)
   {
   }
}
