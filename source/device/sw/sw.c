#include "sw.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "gpio.h"
#include "delayDefs.h"

// switch SW1 GPIO 16 /PIN 07  (info screen)
#define        SW1_GP                   16       // (PIN_07)
#define        SW1_PORT        GPIOA2_BASE       // (16/8=2)
#define        SW1_PIN     (1<< (SW1_GP%8))      // sw1    



uInt8 Sw1State()
{
  uInt32 l_value;
  uInt8  idx;       
  
  for(idx = 0; idx < 50; idx ++)
  {  
     l_value = GPIOPinRead(SW1_PORT,SW1_PIN); 
     if((l_value & SW1_PIN) == SW1_PIN) return 0;
     MAP_UtilsDelay(DELAY_CLK_1_MS);
  }
  
   return 1;  
}

// switch SW2 GPIO 15/PIN 06 (normal work)
#define        SW2_GP                   15       // (PIN_06)
#define        SW2_PORT        GPIOA1_BASE       // (15/8=1)      
#define        SW2_PIN     (1<< (SW2_GP%8))      //  sw2    


uInt8 Sw2State()
{
  uInt32 l_value;
  uInt8  idx;       
  
  for(idx = 0; idx < 50; idx ++)
  {  
     l_value = GPIOPinRead(SW2_PORT,SW2_PIN); 
     if((l_value & SW2_PIN) == SW2_PIN) return 0;
     MAP_UtilsDelay(DELAY_CLK_1_MS);
  }
  
   return 1;  
}

