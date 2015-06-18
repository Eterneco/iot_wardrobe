#include "lcd.h"
#include "bitmaps.h"
#include "fonts.h"
#include "typeDefs.h"
#include "delayDefs.h"

#include <string.h>
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include "spi.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "gpio.h"



// GPIO
#define        PWR_GP                   7   // (PIN_62) power
#define        RES_GP                   0   // (PIN_50) reset
#define        SCE_GP                  30   // (PIN_53) sce
#define        DC_GP                    8   // (PIN_63) data command

#define        SPI_CLK_GP              14   // (PIN_05) spi clk
#define        SPI_MOSI_GP             11   // (PIN_02) spi mosi


// PORT _ A0,A1,A2,A3 = (GPIO / 8)
#define        PWR_PORT              GPIOA0_BASE   // (7/8=0) power     
#define        RES_PORT              GPIOA0_BASE   // (0/8=0) reset
#define        SCE_PORT              GPIOA3_BASE   // (30/8=3) sce
#define        DC_PORT               GPIOA1_BASE   // (8/8=1) data command 

#define        SPI_CLK_PORT          GPIOA1_BASE   // (14/8=1) spi clk
#define        SPI_MOSI_PORT         GPIOA1_BASE   // (11/8=1) spi mosi 


//  PIN _ 1,2,3,4,5,6,7,,,, = 1 << (GPIO % 8)
#define        PWR_PIN        (1<< (PWR_GP%8))     // (7) power     
#define        RES_PIN        (1<< (RES_GP%8))     // (1) reset
#define        SCE_PIN        (1<< (SCE_GP%8))     // (64) sce
#define        DC_PIN         (1<< (DC_GP%8))      // (1) data command 

#define        SPI_CLK_PIN    (1<< (SPI_CLK_GP%8))  // () sce
#define        SPI_MOSI_PIN   (1<< (SPI_MOSI_GP%8)) // () data command 

void SpiWriteByte(uInt8 data)
{
    uInt8 idx, l_data;
    
    l_data = data;
    
    for(idx=0; idx<8; idx ++)
    {
        GPIOPinWrite(SPI_CLK_PORT,  SPI_CLK_PIN, 0x00);
        
        if((l_data & 0x80) == 0x80)
           GPIOPinWrite(SPI_MOSI_PORT,  SPI_MOSI_PIN, SPI_MOSI_PIN);
        else
           GPIOPinWrite(SPI_MOSI_PORT,  SPI_MOSI_PIN, 0x00);
        
        MAP_UtilsDelay(DELAY_CLK_1_US);
        GPIOPinWrite(SPI_CLK_PORT,  SPI_CLK_PIN, SPI_CLK_PIN);
        MAP_UtilsDelay(DELAY_CLK_1_US);
         
        l_data = l_data << 1;
    }

}

void LcdWriteCommand(uInt8 command)
{
  
    GPIOPinWrite(SCE_PORT, SCE_PIN,0x00);                // sce 
    GPIOPinWrite(DC_PORT,  DC_PIN, 0x00);                //  dc
    
    SpiWriteByte(command);
    
    GPIOPinWrite(DC_PORT,  DC_PIN, DC_PIN);              //  dc
    GPIOPinWrite(SCE_PORT, SCE_PIN, SCE_PIN);            // sce 
    
}

void LcdWriteData(uInt8 data)
{
    GPIOPinWrite(SCE_PORT, SCE_PIN, 0x00);               // sce 
    GPIOPinWrite(DC_PORT,  DC_PIN, DC_PIN);              //  dc
 
    SpiWriteByte(data);
    
    GPIOPinWrite(DC_PORT,  DC_PIN, 0x00);                //  dc
    GPIOPinWrite(SCE_PORT, SCE_PIN, SCE_PIN);            // sce 
}


void LcdIoInit()
{
	// pin_mux_config
}

void LcdSpiInit()
{
	// pin_mux_config
    GPIOPinWrite(SPI_CLK_PORT,  SPI_CLK_PIN, SPI_CLK_PIN);
}

void LcdInit()
{
    GPIOPinWrite(PWR_PORT, PWR_PIN, PWR_PIN);         // power on  
    GPIOPinWrite(SCE_PORT, SCE_PIN, 0x00);            // sce 
    GPIOPinWrite(RES_PORT, RES_PIN, 0x00);            // reset on

  
   // Delay to keep printing speed reasonable. About 100 milliseconds.
    MAP_UtilsDelay(800000);
    
 
    GPIOPinWrite(RES_PORT, RES_PIN, RES_PIN);            // reset off
    GPIOPinWrite(SCE_PORT, SCE_PIN, SCE_PIN);            // sce 

    LcdWriteCommand( 0x21 );                    // LCD Extended Commands.
    LcdWriteCommand( 0xC8 );                    // Set LCD Vop (Contrast).
    LcdWriteCommand( 0x06 );                    // Set Temp coefficent.
    LcdWriteCommand( 0x13 );                    // LCD bias mode 1:48.
    LcdWriteCommand( 0x20 );                    // LCD Standard Commands, Horizontal addressing mode.
    LcdWriteCommand( 0x08 );                    // LCD blank
    LcdWriteCommand( 0x0C );                    // LCD in normal mode.
}



void LcdConfigure()
{
    LcdIoInit();
    LcdSpiInit();
    LcdInit();
}

void LcdOn()
{
    GPIOPinWrite(PWR_PORT, PWR_PIN, PWR_PIN);  
    LcdInit();    
}

void LcdOff()
{
    GPIOPinWrite(PWR_PORT, PWR_PIN, 0x00);      
}

void LcdPozWK ( uInt8 w, uInt8 k )
{
    LcdWriteCommand (0x80 | k);   
    LcdWriteCommand (0x40 | w);   
}

void LcdClear(uInt8 w, uInt8 k, uInt8 wys, uInt8 szer)
{
   uInt8 i, j; 
           
   for(i=0; i<wys; i++)
   {
      LcdPozWK (w + i, k); 
     
      for(j=0; j<szer; j++)
      {
         LcdWriteData(0x00); 
      }
       
   }
   
}

void LcdBitmapa(uInt8 w, uInt8 k, const uInt8 * bitmapa)
{
   uInt16 i;
   uInt8  height, width;

   height     = bitmapa[0];
   width      = bitmapa[1];
   
   LcdPozWK(w,k);
   
   for(i=2;i<height*width + 2; i++)
   {
      if((i-2)%width == 0 && i!=2)
      {
         w +=1;
         LcdPozWK(w,k);
      }
      LcdWriteData( bitmapa[i]);
   }
}


void LcdChrWK (uInt8 w, uInt8 k, uInt8 ch)
{
   uInt8 j;
  
   LcdPozWK(w,k);
   
   LcdWriteData(0x00);
   
   for(j=0; j<5; j++)
     LcdWriteData( tFonts8x5[ch - 32][j]);
} 

void LcdStrWK(uInt8 w, uInt8 k, cInt8 *dataPtr) 
{
  while ( *dataPtr ) 
  {
    LcdChrWK( w, k, (*dataPtr));
    k +=6;
    dataPtr++;
  }
}

void LcdStrWKn(uInt8 w, uInt8 k, cInt8 *dataPtr, uInt8 n) 
{
  
  while ( *dataPtr && n ) 
  {
    LcdChrWK( w, k, (*dataPtr));
    k +=6;
    dataPtr++;
    n = n-1;
  }
}

uInt8 LcdStrWKnc(uInt8 w, uInt8 k, cInt8 *dataPtr, uInt8 n, uInt8 c) 
{
  uInt8 l_size = 0;
  
  while ( *dataPtr && n && *dataPtr!=c) 
  {
    LcdChrWK( w, k, (*dataPtr));
    k +=6;
    dataPtr++;
    n = n-1;
    l_size +=1;
  }
  return l_size;
}

