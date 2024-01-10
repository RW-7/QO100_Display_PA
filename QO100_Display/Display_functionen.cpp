//Display_functionen.cpp
#include "defines.h" // Hinzugefügt, um Farbkonstanten zu importieren
#include "globals.h"
#include <TFT_eSPI.h>
#include "Display_functionen.h" 
#include "CIV_actions.h" 
#include "z_userprog.h"

// Deklaration der globalen Variablen aus der globals.h-Datei
extern bool HF_ptt_Enable;
extern bool VHF_ptt_Enable;
extern bool UHF_ptt_Enable;
extern TFT_eSPI tft;


void drawButton(int x, int y, int width, int height, const char* label, bool pressed) {
  // Hintergrund
  tft.fillRect(x, y, width, height, pressed ? GREEN : BLACK);
  
  for (int i = 0; i < 5; i++) {
    tft.drawRoundRect(x + i, y + i, width - 2 * i, height - 2 * i, 5 - i, WHITE);
  }

  // Text
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(x + 15, y + 25);
  tft.print(label);
}
void touch_button(){
 for (int i = 0; i < 3; i++) {
    drawButton(buttonX[i], buttonY, buttonWidth, buttonHeight, (i == 0) ? "Sat" : ((i == 1) ? "Split" : "Ic705"), false);
    // Füge Abstand zwischen den Buttons hinzu
    buttonX[i] += buttonWidth + buttonSpacing;
  }
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
  tft.drawRoundRect(2, 1, 65, 30, 5, RED);  // X, Y, Breite, Höhe, abgerundete Ecken, Rahmenfarbe
  tft.fillRect(2, 1, 65, 30, BLACK); //debug setting background
  tft.setTextColor(RED);
  tft.setCursor(5, 7);
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
  tft.fillRect(1, 1, 65, 30, BLACK); //debug setting background
  tft.setTextColor(WHITE);
  tft.setCursor(5, 7);
  tft.print("RX");
}

void Clear_Scr() {
  tft.fillRect(0, 31, 240, 104, BLACK);
}

void BT_Conn_Status(const char* read_Conn_Status) {
  const char* Conn_Yes = strstr(read_Conn_Status, "R_ON");
  
  tft.setFreeFont(NULL);  // Set default font
  tft.setTextSize(2);

  if (read_Conn_Status == Conn_Yes) {
    //tft.fillRect(80, 1, 85, 28, MAROON);  // productive setting background
    tft.setTextColor(GREEN);
    tft.setCursor(80, 7);
    tft.print("BT CAT");
    Clear_Scr();
    Draw_RX();
    touch_button();
  } else {
    //tft.fillRect(80, 1, 155, 28, MAROON);  // productive setting background
    tft.setTextColor(RED);
    tft.setCursor(80, 7);
    tft.print("BT CAT");
    tft.setTextColor(WHITE);
    tft.setCursor(170, 7);
    tft.print("OFFLINE");
    Clear_Scr();  // clear screen
    tft.setCursor(0, 40);
    tft.print("Please pair\n\nyour ICOM IC-705\n\nvia Bluetooth !");
  }
}


//------------------------------------------------------------------
//    Show frequency in 'kHz' and band in 'Meters' text on TFT vk3pe
//------------------------------------------------------------------
void show_Meters(void) {

  // Show Freq[KHz]
  
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
  tft.fillRect(251, 7, 85, 30, BLACK);  //erase previous freq   vk3pe x,y,width,height,colour 10,40,137,40
  tft.setTextSize(2);
  tft.setCursor(251, 7);
  tft.setTextColor(YELLOW);
  tft.print(modModeStr[newModMode]);
  tft.setCursor(300, 7);
  tft.print(FilStr[newRXfilter]);


}

void user_TXPWR(unsigned short getTXPWR) {
  unsigned long TXPWR_W;
  TXPWR_W = ((getTXPWR * 100) / 255);  // calculate in procent like IC705

  tft.setFreeFont(NULL);  // Set default font
  tft.setTextSize(2);
  //tft.fillRect(170, 1, 65, 28, MAROON);  // productive setting background
  tft.fillRect(170, 1, 80, 28, BLACK); //debug setting background
  tft.setTextColor(WHITE);  // print TXPWR white if output power < 4W
  if (getTXPWR > 101) {
    tft.setTextColor(YELLOW);  // print TXPWR yellow if out > 4W for attention of PA max. input power
  }
  if (getTXPWR > 128) {
    tft.setTextColor(ORANGE);  // print TXPWR orange if out > 5W for attention of PA max. input power
  }
  tft.setCursor(170, 7);
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
  tft.print("RX");
  tft.setCursor(115, 125);
  tft.print("");
}
void set_LCD_Curennt_TX(unsigned long frequency) { // Rechts Oben
  unsigned long freq_kHz;

  freq_kHz = G_SPLIT_frequency / 1000;       // frequency is now in kHz
  G_subVFOSplitBand = get_Band(freq_kHz);  // get band according the current frequency

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
  if(G_currentBand == NUM_BANDS ){
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
}

void set_LCD_SAT_RX(unsigned long long frequency) { //Links Unten
    unsigned long freq_kHz;

  freq_kHz = frequency / 1000;       // frequency is now in kHz
  G_currentDownBand = get_Band(freq_kHz);  // get band according the current frequency

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
  tft.fillRect(0, 190, 105, 55, BLACK);  //-erase   x,y,width, height
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

}
void set_LCD_SAT_TX(unsigned long frequency) { //Rechts Unten
  unsigned long freq_kHz;

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
