//settings.h
#include <Arduino.h>
//!!
// Wählen Sie zunächst das verwendete Radio aus, indem Sie die entsprechenden Zeilen kommentieren bzw. auskommentieren!
// Bitte vergessen Sie nicht, den Debug-Modus entsprechend Ihren Wünschen im ein- oder auszuschalten
//!//


// some general defines --------------------------------------------------------------------------------------

// HW pins in use:

// Arduino uno, nano, Pro mini etc.
// Pin 08 and 09    Altsoftserial COM for CIV HW onewire

// Arduino 2560
// Pin 18 and 19    COM1 for CIV HW onewire

// ESP32:
// Pin 16 and 17    COM2 for CIV HW onewire 
// Pin 16           Trigger for timing measurements (debugging) in case of CIV via Bluetooth

//------------------------------------------------------------------------------------------------------------
// times and timers ...

// Strings are needed only if you want to output something in Serial Monitor,
// otherwise: save RAM !!
#ifdef debug
const String  radioOnOffStr[6] = {
  "R_OFF",
  "R_ON",
  "R_OFF_TR",     // transit from OFF to ON
  "R_ON_TR",      // transit from ON to OFF
  "R_NDEF",       // don't know
  "R_TOGGLE"
};

const String    RXTXstr[2] = {
"RX", // 0 or false
"TX"  // 1 or true
};
#endif




// clear test translation of the modulation modes
#if defined(debug) && defined(modmode)
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
#endif
//=========================================================================================
// creating bandinfo based on the frequency info

//-----------------------------------------------------------------------------------------
// tables for band selection and bittpattern calculation

//for IC-705 which has no 60M band:
//---------------------------------
// !!! pls adapt "NUM_BANDS" if changing the number of entries in the tables below !!!

// lower limits[kHz] of the bands: NOTE, these limits may not accord with band  edges in your country.
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


// clear text translation of the Filter setting
#if defined(debug) && defined(modmode)
const String FilStr[4] = {
  "NDEF",
  "FIL1",   // 1 (1 .. 3 is according to ICOM's documentation)
  "FIL2",
  "FIL3"
};
#endif

//!// put your own defines here if necessary:
// e.g. :


//------------------------------------------------------------------------------------------------------------
// Debugging ...

// Speed of the Serial output messages


// Measure the delay between the incoming PTT line and the outgoing PTT line
#ifdef ESPtriggerMeas
  #define P_Trigger 16
  bool          Trigger=false;
  unsigned long TS_start_delayMeas= millis();
  unsigned long TS_stop_delayMeas = millis();
  #define init_delayMeas pinMode(P_Trigger, INPUT);
  #define set_delayMeas  if (Trigger!=digitalRead(P_Trigger)) {Trigger=digitalRead(P_Trigger);TS_start_delayMeas = millis();}
  #define eval_delayMeas TS_stop_delayMeas = millis();Serial.print("delay: ");Serial.println(TS_stop_delayMeas - TS_start_delayMeas);
#else
  #define init_delayMeas    
  #define set_delayMeas  
  #define eval_delayMeas 
#endif

// time measurement in mikro seconds

#ifdef debug
// for Debugging ...
  extern unsigned long G_timemarker1;
  extern unsigned long G_timemarker1a;
  #define SET_TIME_MARKER1 G_timemarker1 = micros();
  #define EVAL_TIME_MARKER1 G_timemarker1a = micros();Serial.print("t1:  ");Serial.println(G_timemarker1a-G_timemarker1);
  #define EVAL_TIME_MARKER1g G_timemarker1a = micros();if ((G_timemarker1a-G_timemarker1)>3000){Serial.print("t1:  ");Serial.println(G_timemarker1a-G_timemarker1);}
#else
  #define SET_TIME_MARKER1
  #define EVAL_TIME_MARKER1
  #define EVAL_TIME_MARKER1g
#endif

