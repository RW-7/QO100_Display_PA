//Display.cpp
#include "globals.h"
#include "Display.h"
#ifdef LCD

#include <TFT_eSPI.h>       //using this LIB now.  https://github.com/Bodmer/TFT_eSPI 

//-----------------------------------------------------------------------------------------
//for TFT
TFT_eSPI tft = TFT_eSPI();
// Initialisiere die Touch-Variablen
// uint16_t touchX = 0;
// uint16_t touchY = 0;
uint16_t touchX, touchY;
uint16_t calData[5] = { 353, 3568, 269, 3491, 7  };


//=================================================
// Mapping of port-pins to functions on ESP32 TTGO
//=================================================

// // the Pins for SPI
// #define TFT_CS    5
// #define TFT_DC   16
// #define TFT_MOSI 19
// #define TFT_SCLK 18
// #define TFT_RST  23
// #define TFT_BL    4
#endif
//-----------------------------------------------------------------------------------------
// initialise the TFT display
//-----------------------------------------------------------------------------------------

void init_TFT(void)
{
  #ifdef LCD
    tft.begin();
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);              // switch backlight on
    tft.fillScreen(BLACK);
    tft.setRotation(3);
    //tft.fillRoundRect(0, 0, tft.width(), 30, 5, MAROON);   // background for screen title
<<<<<<< HEAD
    tft.drawRoundRect(0, 0, tft.width(), 30, 5, WHITE);    //with white border.
=======
    tft.drawRoundRect(0, 0, tft.width(), 35, 5, WHITE);    //with white border.
>>>>>>> df88bebd5ed2826a2ecc2b96c45f826ec8f9e18d
    tft.setTextSize(2);                  //for default Font only.Font is later changed.
    tft.setTextColor(YELLOW);

   // tft.setTextColor(WHITE);            //white from now on

   tft.setCursor(185, 45);               //
   // tft.setTextSize(3);
   //tft.print("MHz");
   // tft.setTextSize(1);
    // tft.setCursor(150, 95) ; 
   //  tft.setCursor(135, 107);             //
   //  tft.print("band");                   //"160m" etc   or Out if invalid Freq. for Ham bands.
    #ifdef useTouch
     
    tft.setTouch(calData);
    
    #endif
  #endif
}