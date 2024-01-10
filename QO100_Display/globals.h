// globals.h

#ifndef GLOBALS_H
#define GLOBALS_H

#include "defines.h" // Abhängigkeiten hinzufügen, wenn nötig
#include "settings.h" // Abhängigkeiten hinzufügen, wenn nötig
#include "Display_functionen.h" 
#include "CIV_actions.h"
#include <CIVmaster.h> // Include only if necessary
#ifdef WIFI
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#endif
// Forward declarations
class CIV;
//struct CIVresult_t;

// modulation mode according to ICOMs documentation
enum radioModMode_t : uint8_t {
  MOD_LSB = 0,
  MOD_USB,
  MOD_AM,
  MOD_CW,
  MOD_RTTY,
  MOD_FM,
  MOD_WFM,
  MOD_CW_R,
  MOD_RTTY_R,
  MOD_DV   // 09 (Note: on the ICOM CIV bus, this is coded as 17 in BCD-code, i.e. 0x17)
};

// RX filter chosen according to ICOMs documentation
enum radioFilter_t : uint8_t {
  FIL_NDEF = 0,
  FIL1 = 1,
  FIL2,
  FIL3
};

enum radioOnOff_t : uint8_t {
  RADIO_OFF = 0,
  RADIO_ON  = 1,
  RADIO_OFF_TR,     // transit from OFF to ON
  RADIO_ON_TR,      // transit from ON to OFF
  RADIO_NDEF,       // don't know
  RADIO_TOGGLE
};

enum onOff_t : uint8_t {
    OFF,
    ON
};

// Hier die globalen Variablen einfügen
extern CIV civ; // Civ-Interface-Objekt
extern CIVresult_t CIVresultL;
extern uint16_t ts_CIVcmdSent;
extern uint16_t ts_RXTX_sent;
extern uint16_t ts_f_sent;
extern uint16_t ts_TXPWR_sent;

#ifdef modmode
extern uint16_t ts_Mod_sent;
#endif

extern bool CIVwaitForAnswer;

// Globale Variablen der Funkdaten
extern radioOnOff_t G_radioOn;
extern bool G_RXTX;
extern unsigned long G_frequency;
extern unsigned long G_SPLIT_frequency;
extern unsigned long long G_Dipslay_RX_frequency;
extern unsigned long G_Dipslay_TX_frequency;
extern uint8_t G_Mod;
extern uint8_t G_RXfilter;
extern unsigned short G_TXPWR;
extern bool G_QO100;
extern bool G_QO100_BAND_RX;
extern bool SET_QO100;
extern int G_Sat;
extern int G_Split;
extern int G_ic705;
extern int touchcalibrate;
extern unsigned long debounceDelay;
extern unsigned long debounceDelay1;
// Weitere globale Variablen hier hinzufügen, falls erforderlich
#ifdef WIFI
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;
#endif
// Timer-Variablen
extern uint16_t t_curr_lp;
extern uint16_t ts_last_lp;

// WLAN-spezifische Variablen
#ifdef WIFI
#include <WiFi.h>
//#include <PubSubClient.h>
//extern PubSubClient client;
#endif
extern bool HF_ptt_Enable;
extern bool VHF_ptt_Enable;
extern bool UHF_ptt_Enable;
//extern int bandvoltage;
//extern const int LED;
//extern const int C_RELAIS;
//extern int brightness;
extern const int freq;
extern const int ledChannel;
extern const int resolution;
extern int buttonX[6];
extern int buttonY;
extern int buttonWidth;
extern int buttonHeight;
extern int buttonSpacing;
extern bool buttonPressed[6];
extern int sliderX;
extern int sliderY;
extern int sliderWidth;
extern int sliderHeight;
extern int sliderSpacing;
extern uint8_t civAddr;
extern uint8_t G_currentBand;
extern uint8_t G_subVFOSplitBand;
extern uint8_t G_currentDownBand;
extern uint8_t G_subVFOUPBand;
// Weitere globale Variablen hier hinzufügen, falls benötigt

#endif // GLOBALS_H
