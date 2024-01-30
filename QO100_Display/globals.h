// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H
#include "a_defines.h"
#include <stdint.h>
#include <CIVmaster.h> // Include only if necessary

enum radioModMode_t : uint8_t {
  MOD_LSB = 0,
  MOD_USB = 1,
  MOD_AM = 2,
  MOD_CW = 3,
  MOD_RTTY = 4,
  MOD_FM = 5,
  MOD_WFM = 6,
  MOD_CW_R = 7,
  MOD_RTTY_R = 8,
  MOD_DV = 17   // 09 (Note: on the ICOM CIV bus, this is coded as 17 in BCD-code, i.e. 0x17)
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
// RX filter chosen according to ICOMs documentation
enum radioFilter_t : uint8_t {
  FIL_NDEF = 0,
  FIL1 = 1,
  FIL2,
  FIL3
};
const String    RXTXstr[2] = {
"RX", // 0 or false
"TX"  // 1 or true
};
enum radioOnOff_t : uint8_t {
  RADIO_OFF = 0,
  RADIO_ON  = 1,
  RADIO_OFF_TR,     // transit from OFF to ON
  RADIO_ON_TR,      // transit from ON to OFF
  RADIO_NDEF,       // don't know
  RADIO_TOGGLE
};
const String FilStr[4] = {
  "NDEF",
  "FIL1",   // 1 (1 .. 3 is according to ICOM's documentation)
  "FIL2",
  "FIL3"
};
const String buttonLabel[6] = {
  "Sat", // 00 (00 .. 08 is according to ICOM's documentation) 
  "Split", // 01
  "HF PA", // 02
  "UHF PA", // 02
  "SAT PA" , // 02
  "Menu"
};
enum onOff_t : uint8_t {
    OFF,
    ON
};
constexpr unsigned long lowlimits[NUM_BANDS] = {
  1000, 2751, 4501, 6001, 8501, 13001, 16001, 19501, 23001, 26001, 35001, 144000, 430000,1240000,2320000,10450000
};
// upper limits[kHz] of the bands:  //see NOTE above.
constexpr unsigned long uplimits[NUM_BANDS] = {
  2750, 4500, 6000, 8500, 13000, 16000, 19500, 23000, 26000, 35000, 60000, 148000, 470000,1300000,2450000,10500000
};
// "xxM" display for the TFT display. ie show what band the unit is current on in "meters"
const  String band2string[NUM_BANDS + 1] = {
  // 160     80   60    40     30      20     17      15     12     10      6     NDEF
  "160m", " 80m", " 60m", " 40m", " 30m", " 20m", " 17m", " 15m", " 12m", " 10m", "  6m", "  2m", "70cm", " 23cm","13cm","3cm","no TX"
};
constexpr unsigned long band2int[NUM_BANDS + 1 ] = {
   160,80,60,40,30,20,17,15,12,10,6,2,70,23,13,3
  //"160", " 80", "60", "40", " 30m", " 20m", " 17m", " 15m", " 12m", " 10m", "  6m", "  2m", "70cm", " Out"
};

constexpr uint8_t band2BCD [NUM_BANDS+1] = { 
// 160    80    60    40     30     20     17     15     12     10      6  NDEF
  0x01, 0x02, 0x03, 0x03,  0x04,  0x05,  0x06,  0x07,  0x08,  0x09,  0x0A, 0x00
};
// Touch


#endif // GLOBALS_H