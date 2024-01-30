//Display_functionen.cpp
#include "a_defines.h" // Hinzugefügt, um Farbkonstanten zu importieren
#include "globals.h"
#include <TFT_eSPI.h>
#include "Display_functionen.h" 
#include <TimeLib.h>
extern TFT_eSPI tft;

void set_LCD_Curennt_RX(unsigned long frequency) { // Links Oben


  //freq_kHz = G_frequency / 1000;       // frequency is now in kHz
 // G_currentBand = get_Band(freq_kHz);  // get band according the current frequency

  tft.setFreeFont(&FreeSansBold9pt7b);   //bigger numbers etc from now on. <<<<<<<<-------------------
  // tft.setFreeFont(&FreeMonoBold18pt7b);
  // tft.setFreeFont(&Orbitron_Light_32);
  tft.fillRoundRect(0, 35, tft.width()/2-1, 40, 5, BLUE);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
  // tft.drawRoundRect(0, 35, tft.width(), 40, 5, BLACK);    //with white border.
  tft.setTextColor(WHITE);  // at power up not set!
 
  tft.setTextSize(2);
  tft.setCursor(5, 67);
  // mod by DL1BZ
  tft.print(0.000001 * frequency, 5);  //show Frequency in MHz with 5 decimal digits
                                       // tft.println("M"); // add W for display TXPWR value
  tft.setCursor(5, 120);                //-
  tft.fillRect(0, 80, 105, 55, BLACK);  //-erase   x,y,width, height
  tft.drawRoundRect(0, 80, 105, 55, 5, WHITE);
  tft.setTextColor(YELLOW);  //-
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextSize(2);
  //  if(G_currentBand == NUM_BANDS ){
  //   tft.setTextColor(RED); 
  // }
  //tft.print(band2string[G_currentBand]);  
  tft.fillRect(105, 80, 85, 55, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
  tft.drawRoundRect(105, 80, 85, 55, 5, WHITE);
  tft.setTextSize(1);
  tft.setCursor(115, 100);
  tft.setTextColor(YELLOW);
  // if(G_Split == 1){
  //   tft.print("RX");
  // }else{
    tft.print("TX & RX");
  //}
  
  tft.setCursor(115, 125);
  tft.print("");
}
void touch_calibrate()
{
  uint16_t calData[5];
 

  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(4000);
}