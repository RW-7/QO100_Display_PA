#ifndef DISPLAY_H
#define DISPLAY_H
#include "globals.h"
#include <SPI.h>
#include <TFT_eSPI.h>
//-----------------------------------------------------------------------------------------
//for TFT
TFT_eSPI tft = TFT_eSPI();
uint16_t touchX, touchY;
uint16_t calData[5] = { 263, 3632, 304, 3568, 1 };
void Draw_RX() {

  // RX Topline
  tft.setFreeFont(NULL);  // Set default font
  tft.setTextSize(2);
  // tft.drawRoundRect(7, 2, 25, 20, 5, WHITE);  // X, Y, Breite, Höhe, abgerundete Ecken, Rahmenfarbe
  tft.fillRect(1, 1, 40, 28, BLACK);
  tft.setTextColor(WHITE);
  tft.setCursor(9, 7);
  tft.print("RX");
}
void Draw_TX() {

  //TX Topline
  tft.setFreeFont(NULL);  // Set default font
  tft.setTextSize(2);
  //tft.drawRoundRect(7, 2, 25, 20, 5, RED);  // X, Y, Breite, Höhe, abgerundete Ecken, Rahmenfarbe
  tft.fillRect(2, 1, 40, 28, BLACK);  //debug setting background
  tft.setTextColor(RED);
  tft.setCursor(7, 7);
  tft.print("TX");
}

void set_LCD_Curennt_RX() {  // Links Oben
  //freq_kHz = G_frequency / 1000;       // frequency is now in kHz
  // G_currentBand = get_Band(freq_kHz);  // get band according the current frequency

  tft.setFreeFont(&FreeSansBold9pt7b);  //bigger numbers etc from now on. <<<<<<<<-------------------
  // tft.setFreeFont(&FreeMonoBold18pt7b);
  // tft.setFreeFont(&Orbitron_Light_32);
  tft.fillRoundRect(0, 35, tft.width() / 2 - 1, 40, 5, BLUE);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
  // tft.drawRoundRect(0, 35, tft.width(), 40, 5, BLACK);    //with white border.
  tft.setTextColor(WHITE);  // at power up not set!

  tft.setTextSize(2);
  tft.setCursor(5, 67);
  // mod by DL1BZ
  tft.print(0.000001 * G_frequency, 5);   //show Frequency in MHz with 5 decimal digits
                                        // tft.println("M"); // add W for display TXPWR value
  tft.setCursor(5, 120);                //-
  tft.fillRect(0, 80, 105, 55, BLACK);  //-erase   x,y,width, height
  tft.drawRoundRect(0, 80, 105, 55, 5, WHITE);
  tft.setTextColor(YELLOW);  //-
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextSize(2);
  if(G_currentBand == NUM_BANDS ){
   tft.setTextColor(RED);
  }
  tft.print(band2string[G_currentBand]);
  tft.fillRect(105, 80, 85, 55, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
  tft.drawRoundRect(105, 80, 85, 55, 5, WHITE);
  tft.setTextSize(1);
  tft.setCursor(115, 100);
  tft.setTextColor(YELLOW);
  if(is_SPLIT == 1){
     tft.print("RX");
  }else{
     tft.print("TX & RX");
  }

  tft.setCursor(115, 125);
  tft.print("");
}
//------------------------------------------------------------------
//    Show frequency in 'kHz' and band in 'Meters' text on TFT vk3pe
//------------------------------------------------------------------
void setTimeString(String utc_time) {
  
  tft.setFreeFont(NULL);  // Set font to GLCD
  tft.fillRect(330, 1, 140, 25, BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(330, 7);
  tft.print("UTC: ");
   tft.setCursor(375, 7);
  tft.print(utc_time);
}
void drawButton() {  // Erstelle Touch Buttons mit Label
  int buttonCount = sizeof(buttonX) / sizeof(buttonX[0]);
  for (int i = 0; i < buttonCount; i++) {
    tft.drawRoundRect(buttonX[i], buttonY, buttonWidth, buttonHeight, 5, WHITE);  //with white border.
    tft.setFreeFont(NULL);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.setCursor(buttonX[i] + 5, buttonY + 15);
    tft.print(buttonLabel[i]);
  }
}

void touch_button(int x, int y, const String label, uint16_t color) {
  tft.setFreeFont(NULL);
  tft.setTextSize(2);
  tft.setTextColor(color);
  tft.setCursor(x + 5, y + 15);
  tft.print(label);
}
void touch_calibrate() {
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

  Serial.println();
  Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++) {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println();
  Serial.println();

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(4000);
}
void init_TFT(void) {
#ifdef LCD
  tft.begin();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);  // switch backlight on
  tft.fillScreen(BLACK);
  tft.setRotation(3);
  //tft.fillRoundRect(0, 0, tft.width(), 30, 5, MAROON);   // background for screen title
  // Header
  tft.drawRoundRect(0, 0, tft.width(), 30, 5, WHITE);  //with white border.
  Draw_RX();                                           // RX Button
  set_LCD_Curennt_RX();                               // Frequenzanzeige links oben
  drawButton();                                        // Touch Buttons mit Label
  tft.setTouch(calData);

#endif
}
#endif  // DISPLAY_H