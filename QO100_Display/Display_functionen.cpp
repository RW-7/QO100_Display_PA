//Display_functionen.cpp
#include "defines.h" // Hinzugefügt, um Farbkonstanten zu importieren
#include "globals.h"
#include <TFT_eSPI.h>
#include "Display_functionen.h" 
#include "CIV_actions.h" 
#include "z_userprog.h"
#include <TimeLib.h>

// Deklaration der globalen Variablen aus der globals.h-Datei
extern bool HF_ptt_Enable;
extern bool VHF_ptt_Enable;
extern bool UHF_ptt_Enable;
extern TFT_eSPI tft;

void drawButton() {  // Erstelle Touch Buttons mit Label 
int buttonCount = sizeof(buttonX) / sizeof(buttonX[0]);
  for (int i = 0; i < buttonCount; i++) {
    tft.drawRoundRect(buttonX[i], buttonY, buttonWidth, buttonHeight, 5, WHITE);    //with white border.
    tft.setFreeFont(NULL);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.setCursor(buttonX[i] + 5, buttonY + 15);
    tft.print(buttonLabel[i]);
  }
}

void touch_button(int x, int y, const String label, uint16_t color){
  tft.setFreeFont(NULL);
  tft.setTextSize(2);
  tft.setTextColor(color);
  tft.setCursor(x + 5, y + 15);
  tft.print(label);
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


void Draw_TX() {
  //TX 
  // tft.fillRoundRect(190, 80, 50, 55, 10, RED);
  // tft.drawLine(215, 88, 215, 128, WHITE);
  // tft.drawLine(194, 88, 215, 103, WHITE);
  // tft.drawLine(235, 88, 215, 103, WHITE);
  //TX Topline
  tft.setFreeFont(NULL);  // Set default font
  tft.setTextSize(2);
  //tft.drawRoundRect(7, 2, 25, 20, 5, RED);  // X, Y, Breite, Höhe, abgerundete Ecken, Rahmenfarbe
  tft.fillRect(2, 1, 40, 28, BLACK); //debug setting background
  tft.setTextColor(RED);
  tft.setCursor(7, 7);
  tft.print("TX");
}

void Draw_RX() {
  
  // tft.fillRoundRect(190, 80, 50, 55, 10, GREEN);
  // tft.fillRect(197, 90, 15, 30, BLACK);
  // tft.drawLine(232, 85, 232, 125, BLACK);
  // tft.drawLine(232, 85, 212, 90, BLACK);
  // tft.drawLine(232, 125, 212, 120, BLACK);
  // RX Topline
  tft.setFreeFont(NULL);  // Set default font
  tft.setTextSize(2);
 // tft.drawRoundRect(7, 2, 25, 20, 5, WHITE);  // X, Y, Breite, Höhe, abgerundete Ecken, Rahmenfarbe
  tft.fillRect(1, 1, 40, 28, BLACK); //debug setting background
  tft.setTextColor(WHITE);
  tft.setCursor(9, 7);
  tft.print("RX");
}

void Clear_Scr() {
  tft.fillRect(0, 30, 250, 104, BLACK);
}

void BT_Conn_Status(const char* read_Conn_Status) {
  const char* Conn_Yes = strstr(read_Conn_Status, "R_ON");
  Serial.println ("BT_Conn_Status");
  tft.setFreeFont(NULL);  // Set default font
  tft.setTextSize(2);

  if (read_Conn_Status == Conn_Yes) {
    tft.drawRoundRect(0, 0, tft.width(), 30, 5, WHITE);    //with white border
    tft.fillRect(50, 1, 250, 28, BLACK); //debug setting background
    Serial.println ("Conn_Yes");
    //tft.fillRect(80, 1, 85, 28, MAROON);  // productive setting background
    tft.setTextColor(GREEN);
    tft.setCursor(50, 7);
    tft.print("BT CAT");
    Clear_Scr();
    Draw_RX();
    drawButton();
  } else {
    Serial.println ("Conn_NO");
     tft.drawRoundRect(0, 0, tft.width(), 30, 5, WHITE);    //with white border.
    //tft.fillRect(80, 1, 155, 28, MAROON);  // productive setting background
    tft.setTextColor(RED);
    tft.setCursor(50, 7);
    tft.print("BT CAT");
    tft.setTextColor(WHITE);
    tft.setCursor(130, 7);
    tft.print("OFFLINE");
    Clear_Scr();  // clear screen
    tft.setCursor(0, 40);
    tft.print("Please pair\n\nyour ICOM IC-705\n\nvia Bluetooth !");
    drawButton();
  }
}


//------------------------------------------------------------------
//    Show frequency in 'kHz' and band in 'Meters' text on TFT vk3pe
//------------------------------------------------------------------
void setTimeString() {
  timeClient.update();
  
  tft.setFreeFont(NULL);  // Set font to GLCD
  tft.fillRect(330, 1, 140, 25, BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(330, 7);
  tft.print("UTC: ");
   tft.setCursor(375, 7);
  tft.print(timeClient.getFormattedTime());
}
void show_Meters(void) {
    
}
void show_Mode(uint8_t newModMode, uint8_t newRXfilter) {
  // tft.setFreeFont(NULL);  // Set font to GLCD
  // // tft.setFreeFont(&FreeSans9pt7b);
  // // tft.setFreeFont(&Tiny3x3a2pt7b);
  // tft.fillRect(105, 80, 85, 55, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
  // tft.drawRoundRect(105, 80, 85, 55, 5, WHITE);
  // tft.setTextSize(2);
  // tft.setCursor(115, 90);
  // tft.setTextColor(YELLOW);
  // tft.print(modModeStr[newModMode]);
  // tft.setCursor(115, 115);
  // tft.print(FilStr[newRXfilter]);

  tft.setFreeFont(NULL);  // Set font to GLCD
  // tft.setFreeFont(&FreeSans9pt7b);
  // tft.setFreeFont(&Tiny3x3a2pt7b);
  tft.fillRect(225, 1, 90, 24, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
  tft.setTextSize(2);
  tft.setCursor(225, 7);
  tft.setTextColor(YELLOW);
  tft.print(modModeStr[newModMode]);
  tft.setCursor(270, 7);
  tft.print(FilStr[newRXfilter]);


}

void user_TXPWR(unsigned short getTXPWR) {
  unsigned long TXPWR_W;
  TXPWR_W = ((getTXPWR * 100) / 255);  // calculate in procent like IC705

  tft.setFreeFont(NULL);  // Set default font
  tft.setTextSize(2);
  //tft.fillRect(170, 1, 65, 28, MAROON);  // productive setting background
  tft.fillRect(140, 1, 64, 28, BLACK); //debug setting background
  tft.setTextColor(WHITE);  // print TXPWR white if output power < 4W
  if (getTXPWR > 101) {
    tft.setTextColor(YELLOW);  // print TXPWR yellow if out > 4W for attention of PA max. input power
  }
  if (getTXPWR > 128) {
    tft.setTextColor(ORANGE);  // print TXPWR orange if out > 5W for attention of PA max. input power
  }
  tft.setCursor(140, 7);
  tft.print(0.1 * TXPWR_W, 1);  //output TXPWR as Watt with one decimal digit
  tft.println("W");             // add W for display TXPWR value
}
//------------------------------------------------------------
// process the frequency received from the radio
//------------------------------------------------------------

void set_LCD_Curennt_RX(unsigned long frequency) { // Links Oben
  unsigned long freq_kHz;

  freq_kHz = G_frequency / 1000;       // frequency is now in kHz
  G_currentBand = get_Band(freq_kHz);  // get band according the current frequency

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
   if(G_currentBand == NUM_BANDS ){
    tft.setTextColor(RED); 
  }
  tft.print(band2string[G_currentBand]);  
  tft.fillRect(105, 80, 85, 55, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
  tft.drawRoundRect(105, 80, 85, 55, 5, WHITE);
  tft.setTextSize(1);
  tft.setCursor(115, 100);
  tft.setTextColor(YELLOW);
  if(G_Split == 1){
    tft.print("RX");
  }else{
    tft.print("TX & RX");
  }
  
  tft.setCursor(115, 125);
  tft.print("");
}
void set_LCD_Curennt_TX(unsigned long frequency) { // Rechts Oben
  unsigned long freq_kHz;

  freq_kHz = G_SPLIT_frequency / 1000;       // frequency is now in kHz
  G_subVFOSplitBand = get_Band(freq_kHz);  // get band according the current frequency
  if(G_Split == 1){
    tft.setFreeFont(&FreeSansBold9pt7b);   //bigger numbers etc from now on. <<<<<<<<-------------------
    // tft.setFreeFont(&FreeMonoBold18pt7b);
    // tft.setFreeFont(&Orbitron_Light_32);
    tft.fillRoundRect(tft.width()/2+1, 35, tft.width()/2, 40, 5, BLUE);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    
    tft.setTextSize(2);
    tft.setCursor(tft.width()/2+6, 67); 
    tft.setTextColor(WHITE);  // at power up not set!
    
    // mod by DL1BZ
    tft.print(0.000001 * G_SPLIT_frequency, 5);  //show Frequency in MHz with 5 decimal digits

    tft.fillRect(tft.width()/2+0, 80, 105, 55, BLACK);  //-erase   x,y,width, height
    tft.drawRoundRect(tft.width()/2+0, 80, 105, 55, 5, WHITE);
    tft.setCursor(tft.width()/2+5, 120);                //-
    tft.setTextColor(YELLOW);  //-
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextSize(2);
    if(G_subVFOSplitBand == NUM_BANDS ){
      tft.setTextColor(RED); 
    }
    tft.print(band2string[G_subVFOSplitBand]);  //-
    tft.fillRect(tft.width()/2+105, 80, 85, 55, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    tft.drawRoundRect(tft.width()/2+105, 80, 85, 55, 5, WHITE);
    tft.setTextSize(1);
    tft.setCursor(tft.width()/2+115, 100);
    tft.setTextColor(YELLOW);
    tft.print("TX");
    tft.setCursor(tft.width()/2+115, 125);
    tft.print("Split");
  }else{
    tft.fillRoundRect(tft.width()/2+1, 35, tft.width()/2, 40, 5, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    tft.fillRect(tft.width()/2+0, 80, 105, 55, BLACK);  //-erase   x,y,width, height
    tft.drawRoundRect(tft.width()/2+0, 80, 105, 55, 5, BLACK);
    tft.fillRect(tft.width()/2+105, 80, 85, 55, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    tft.drawRoundRect(tft.width()/2+105, 80, 85, 55, 5, BLACK);
  }
}

void set_LCD_SAT_RX(unsigned long long frequency) { //Links Unten
    unsigned long freq_kHz;
 Serial.println ("set_LCD_SAT_RX");
  freq_kHz = frequency / 1000;       // frequency is now in kHz
  G_currentDownBand = get_Band(freq_kHz);  // get band according the current frequency
  if(G_Sat == 1){
    tft.setFreeFont(&FreeSansBold9pt7b);   //bigger numbers etc from now on. <<<<<<<<-------------------
    // tft.setFreeFont(&FreeMonoBold18pt7b);
    // tft.setFreeFont(&Orbitron_Light_32);
    tft.fillRoundRect(0, 140, tft.width()/2-1, 40, 5, BLUE);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    tft.setTextColor(WHITE);  // at power up not set!
  
    tft.setTextSize(2);
    tft.setCursor(5, 172);
    // mod by DL1BZ

    tft.print(0.000001 * frequency, 5);  //show Frequency in MHz with 5 decimal digits

    tft.setCursor(5, 225);                //-
    tft.fillRect(0, 190, 110, 55, BLACK);  //-erase   x,y,width, height
    tft.drawRoundRect(0, 190, 105, 55, 5, WHITE);
    tft.setTextColor(YELLOW);  //-
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextSize(2);
    if(G_currentDownBand == NUM_BANDS ){
      tft.setTextColor(RED); 
    }
    tft.print(band2string[G_currentDownBand]);  
    tft.fillRect(105, 190, 85, 55, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    tft.drawRoundRect(105, 190, 85, 55, 5, WHITE);
    tft.setTextSize(1);
    tft.setCursor(115, 210);
    tft.setTextColor(YELLOW);
    tft.print("RX");
    tft.setCursor(115, 235);
    tft.print("DownConv.");
  }else{
    tft.fillRoundRect(0, 140, tft.width()/2-1, 40, 5, BLACK);
    tft.fillRect(0, 190, 110, 55, BLACK);  //-erase   x,y,width, height
    tft.drawRoundRect(0, 190, 105, 55, 5, BLACK);
    tft.fillRect(105, 190, 110, 55, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    tft.drawRoundRect(105, 190, 85, 55, 5, BLACK);
  }
}
void set_LCD_SAT_TX(unsigned long frequency) { //Rechts Unten
  unsigned long freq_kHz;
  Serial.println ("set_LCD_SAT_TX");
  if( G_Sat == 1 && G_Split == 1) {
    freq_kHz = G_Dipslay_TX_frequency / 1000;       // frequency is now in kHz
    G_subVFOUPBand = get_Band(freq_kHz);  // get band according the current frequency

    tft.setFreeFont(&FreeSansBold9pt7b);   //bigger numbers etc from now on. <<<<<<<<-------------------
    // tft.setFreeFont(&FreeMonoBold18pt7b);
    // tft.setFreeFont(&Orbitron_Light_32);
    tft.fillRoundRect(tft.width()/2+1, 140, tft.width()/2, 40, 5, BLUE);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    tft.setCursor(tft.width()/2+6, 172); 
    tft.setTextSize(2);
    tft.setTextColor(WHITE);  // at power up not set!
  
    // mod by DL1BZ
    tft.print(0.000001 * G_Dipslay_TX_frequency, 5);  //show Frequency in MHz with 5 decimal digits

    tft.fillRect(tft.width()/2+0, 190, 105, 55, BLACK);  //-erase   x,y,width, height
    tft.drawRoundRect(tft.width()/2+0, 190, 105, 55, 5, WHITE);
    tft.setCursor(tft.width()/2+5, 225);                //-
    tft.setTextColor(YELLOW);  //-
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextSize(2);
      if(G_subVFOUPBand == NUM_BANDS ){
      tft.setTextColor(RED); 
    }
    tft.print(band2string[G_subVFOUPBand]);  //-
    tft.fillRect(tft.width()/2+105, 190, 85, 55, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    tft.drawRoundRect(tft.width()/2+105, 190, 85, 55, 5, WHITE);
    tft.setTextSize(1);
    tft.setCursor(tft.width()/2+115, 210);
    tft.setTextColor(YELLOW);
    tft.print("TX");
    tft.setCursor(tft.width()/2+115, 235);
    tft.print("UPconv.");
  }else{
    tft.fillRoundRect(tft.width()/2+1, 140, tft.width()/2, 40, 5, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    tft.fillRect(tft.width()/2+0, 190, 105, 55, BLACK);  //-erase   x,y,width, height
    tft.drawRoundRect(tft.width()/2+0, 190, 105, 55, 5, BLACK);
    tft.fillRect(tft.width()/2+105, 190, 105, 55, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
    tft.drawRoundRect(tft.width()/2+105, 190, 85, 55, 5, BLACK);
  }
}
void userPTT(uint8_t newState) {

#ifdef debug
  Serial.println(newState);  //prints '1' for Tx, '0' for Rx
#endif
  tft.setFreeFont(&FreeSansBold9pt7b);  //previous setup text was smaller.
  //tft.setTextColor(WHITE) ;


  if (newState) {  // '1' = Tx mode
    Draw_TX();
  }  //Tx mode
  else {
    Draw_RX();
  }  //Rx mode

#ifdef Inv_PTT
  digitalWrite(PTTpin, !newState);  //--inverted-- output version:  Clr =Tx, Hi =Rx
  if (HF_ptt_Enable) {
    digitalWrite(PTTpinHF, !newState);
  }
  if (VHF_ptt_Enable) {
    digitalWrite(PTTpinVHF, !newState);
  }
  if (UHF_ptt_Enable) {
    digitalWrite(PTTpinUHF, !newState);
  }
#else
  digitalWrite(PTTpin, newState);  // Clr =Rx, Hi =Tx
  if (HF_ptt_Enable) {
    digitalWrite(PTTpinHF, newState);
  }
  if (VHF_ptt_Enable) {
    digitalWrite(PTTpinVHF, newState);
  }
  if (UHF_ptt_Enable) {
    digitalWrite(PTTpinUHF, newState);
  }
#endif
}
