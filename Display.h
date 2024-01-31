#ifndef DISPLAY_H
#define DISPLAY_H
#include "globals.h"
#include <TFT_eSPI.h> 
//-----------------------------------------------------------------------------------------
//for TFT
TFT_eSPI tft = TFT_eSPI();
void init_TFT(void)
{
  #ifdef LCD
    tft.begin();
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);              // switch backlight on
    tft.fillScreen(BLACK);
    tft.setRotation(3);
    //tft.fillRoundRect(0, 0, tft.width(), 30, 5, MAROON);   // background for screen title
    tft.drawRoundRect(0, 0, tft.width(), 30, 5, WHITE);    //with white border.
    Draw_RX();
    drawButton();
    #ifdef useTouch
     
    tft.setTouch(calData);
    #endif
  #endif
}
#endif  // DISPLAY_H