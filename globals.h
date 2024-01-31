#ifndef GLOBALS_H
#define GLOBALS_H
#include "defines.h"
// Touch einstellungen
int touchcalibrate = 0;
int buttonX[6] = { 0, 80, 160, 240, 320, 400 };
int buttonY = 260;
int buttonWidth = 80;
int buttonHeight = 40;
bool buttonPressed[6] = { false, false, false, false, false, false };
const String buttonLabel[6] = {
  "Sat",     // 00 (00 .. 08 is according to ICOM's documentation)
  "Split",   // 01
  "HF PA",   // 02
  "UHF PA",  // 02
  "SAT PA",  // 02
  "Menu"
};
const char* shellyHF_URL = "http://192.168.4.197/relay/0";
const char* shellySAT_URL = "http://192.168.4.78/relay/0";
String utc_time;
  unsigned long currentTime = millis();
  unsigned long setZeitabfrage = 1000;
//*************************************************************//*************************************************************
// tables for band selection and bittpattern calculation
uint8_t currentBand = NUM_BANDS;  //
unsigned long G_frequency = 0;        // Operating frequency in [Hz]
unsigned long G_SPLIT_frequency = 0;          // Operating frequency in [Hz]
unsigned long long G_Dipslay_RX_frequency = 0;      // Operating frequency in [Hz]
unsigned long G_Dipslay_TX_frequency = 0;      // Operating frequency in [Hz]
uint8_t G_currentBand;
uint8_t G_subVFOSplitBand;  // TX Split
uint8_t G_currentDownBand; // RX Upconverter
uint8_t G_subVFOUPBand; //TX Upconverter
// lower limits[kHz] of the bands:
constexpr unsigned long lowlimits[NUM_BANDS] = {
  1000, 2751, 4501, 6001, 8501, 13001, 16001, 19501, 23001, 26001, 35001, 144000, 430000, 1240000, 2320000, 10450000
};
// upper limits[kHz] of the bands:
constexpr unsigned long uplimits[NUM_BANDS] = {
  2750, 4500, 6000, 8500, 13000, 16000, 19500, 23000, 26000, 35000, 60000, 148000, 470000, 1300000, 2450000, 10500000
};
// display for the TFT display. ie show what band the unit is current on in "meters"
const String band2string[NUM_BANDS + 1] = {
  // 160     80   60    40     30      20     17      15     12     10      6     NDEF
  "160m", " 80m", " 60m", " 40m", " 30m", " 20m", " 17m", " 15m", " 12m", " 10m", "  6m", "  2m", "70cm", " 23cm", "13cm", "3cm", "no TX"
};
constexpr unsigned long band2int[NUM_BANDS + 1] = {
  160, 80, 60, 40, 30, 20, 17, 15, 12, 10, 6, 2, 70, 23, 13, 3
  //"160", " 80", "60", "40", " 30m", " 20m", " 17m", " 15m", " 12m", " 10m", "  6m", "  2m", "70cm", " Out"
};
const String modModeStr[11] = {
  "LSB   ", // 00 (00 .. 08 is according to ICOM's documentation) 
  "USB   ", // 01
  "AM    ", // 02
  "CW    ", // 03
  "RTTY  ", // 04
  "FM    ", // 05
  "WFM   ", // 06
  "CW-R  ", // 07
  "RTTY-R", // 08
  "DV    ", // 09 (Note: on the ICOM CIV bus, this is coded as 17 in BCD-code, i.e. 0x17)
  "NDEF  "  // 10
};
const String FilStr[4] = {
  "NDEF",
  "FIL1",   // 1 (1 .. 3 is according to ICOM's documentation)
  "FIL2",
  "FIL3"
};
const String DataStr[2] = {
  "",
  "-D"
};



enum onOff_t : uint8_t {
  OFF = 0,
  ON = 1,
  NDEF
};
uint8_t VFOAMode = 0;
uint8_t VFOAFilter = 0;
uint8_t VFOAModeData = 0;

bool ShellyHF;   // 0 == OFF; 1 == on
bool ShellySAT;  // 2 == 2m Band RX;
bool isShellyHF;
bool isShellySAT;

bool is_RXTX = 0;
bool is_SPLIT = 0;
bool is_SAT = 1;
bool LCD_Curennt_RX = 1;  // 0 == RX; 1 == TX on
int G_Sat = 0;
int G_Split = 0;
String timeFormat(time_t timeValue);


// get the bandnumber matching to the frequency (in kHz)

byte get_Band(unsigned long frq) {
  byte i;
  for (i = 0; i < NUM_BANDS; i++) {
    if ((frq >= lowlimits[i]) && (frq <= uplimits[i])) {
      return i;
    }
  }
  return NUM_BANDS;  // no valid band found -> return not defined
}
#endif  //GLOBALS_H