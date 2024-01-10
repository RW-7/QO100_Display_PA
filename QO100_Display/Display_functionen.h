// display_functions.h

#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include "globals.h"  // Include the necessary global header file
#include <Arduino.h>  // Include Arduino core library for data types and functions
void Draw_TX();
void Draw_RX();
void Clear_Scr();
void BT_Conn_Status(const char* read_Conn_Status);
void show_Meters();
void show_Mode(uint8_t newModMode, uint8_t newRXfilter); // Mode & Filter
void user_TXPWR(unsigned short getTXPWR);
void set_LCD_Curennt_RX(unsigned long frequency); // RX und TX Frequenz im single modus. im Splitmodus RX (Gerätefrequenz)
void set_LCD_Curennt_TX(unsigned long frequency); // TX im Splitmodus (Gerätefrequenz)
void set_LCD_SAT_RX(unsigned long long frequency); // RX und TX Frequenz im single modus mit Downconverter.  RX im Splitmodus mit Downconverter
void set_LCD_SAT_TX(unsigned long frequency); // TX im Splitmodus mit Upconverter
void drawButton();
void touch_button(int x, int y, const String label, uint16_t color);
void touch_calibrate();
void userPTT(uint8_t newState);
#endif  // DISPLAY_FUNCTIONS_H
