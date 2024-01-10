// globals.cpp

#include "globals.h"
#include <CIVmaster.h>

// Define the global variables here

//------------------------------------------------ -----------------
// Civ-Objekt erstellen
CIV civ; // Civ-Interface-Objekt erstellen; Dadurch können Sie später einfach auf die Schnittstelle zum Radio zugreifen
// Diese Struktur trägt das Ergebnis der civ-Methoden writeMsg und readMsg
CIVresult_t CIVresultL;
uint16_t ts_CIVcmdSent = 0; // Zeitstempel[ms] des letzten gesendeten Befehls
                            // Dies gilt für alle Befehle, der letzte gewinnt!
                            // Wird zur Bestimmung des Timeouts verwendet
uint16_t ts_RXTX_sent = 0;   // Zeitstempel[ms] der letzten an das Radio gesendeten RXTX-Anfrage
uint16_t ts_f_sent = 0;      // Zeitstempel[ms] der letzten an das Radio gesendeten Frequenzabfrage
// Hinzugefügt von DL1BZ
uint16_t ts_TXPWR_sent = 0;   // Zeitstempel[ms] der letzten an das Funkgerät gesendeten TXPWR-Anfrage

#ifdef modmode
uint16_t ts_Mod_sent = 0; // Zeitstempel[ms] der letzten an das Radio gesendeten ModMode-Anfrage
#endif
//!// Fügen Sie hier zusätzliche Timer ein, falls Sie zusätzliche Abfragen hinzufügen
int touchcalibrate = 0;
bool CIVwaitForAnswer = false; // Wenn wahr, wurde ein Befehl an das Radio gesendet, das wurde noch nicht beantwortet

// Globale Variablen der Funkdaten
// "Datenbank"-Variablen:
radioOnOff_t G_radioOn = RADIO_NDEF; // Initially don't know whether the radio is switched on and connected
bool G_RXTX = 0;                     // 0 == RX; 1 == TX on
unsigned long G_frequency = 0;        // Operating frequency in [Hz]
unsigned long G_SPLIT_frequency = 0;          // Operating frequency in [Hz]
unsigned long long G_Dipslay_RX_frequency = 0;      // Operating frequency in [Hz]
unsigned long G_Dipslay_TX_frequency = 0;      // Operating frequency in [Hz]
uint8_t G_Mod = MOD_NDEF;             // Modulation mode (USB, LSB, etc...)
uint8_t G_RXfilter = FIL_NDEF;        // RX filter in use (Fil1, Fil2, Fil3);
// Hinzugefügt von DL1BZ
unsigned short G_TXPWR = 0;          // TX Power (0..255 = 0-10W)
bool G_QO100 = 1;             // 0 == OFF; 1 == on
bool G_QO100_BAND_RX = 2;     // 2 == 2m Band RX;
bool LCD_Curennt_RX = 1;           // 0 == RX; 1 == TX on
int G_Sat;
int G_Split;
int G_ic705;
// Weitere globale Variablen hier hinzufügen, falls erforderlich

// Timer-Variablen -------------------------------------------------------------
uint16_t t_curr_lp;       // Aktuelle Zeit der aktuellen Baseloop (am Anfang) für Berechnungen
uint16_t ts_last_lp;      // Zeitstempel - wird am Ende jeder Baseloop-Ausführung aktualisiert
// Touch
int buttonX[4] = {1, 81, 162, 243};
int buttonY = 260;
int buttonWidth = 70;
int buttonHeight = 40;
bool buttonPressed[4] = {false, false, false,false};
int buttonSpacing = 0;
unsigned long debounceDelay = 500;
// WLAN-spezifische Variablen
#ifdef WIFI
#include <WiFi.h>
#include <PubSubClient.h>

extern boolean HF_ptt_Enable;
extern boolean VHF_ptt_Enable;
extern boolean UHF_ptt_Enable;
int bandvoltage;
//const int LED;
// const int C_RELAIS;
int brightness;

PubSubClient client;
#endif

#ifdef IC705
uint8_t civAddr = CIV_ADDR_705;
#endif
#ifdef IC7300
uint8_t civAddr = CIV_ADDR_7300;
#endif
#ifdef IC9700
uint8_t civAddr = CIV_ADDR_9700;
#endif
uint8_t G_currentBand;
uint8_t G_subVFOSplitBand;  // TX Split
uint8_t G_currentDownBand; // RX Upconverter
uint8_t G_subVFOUPBand; //TX Upconverter
// Weitere globale Variablen hier hinzufügen, falls benötigt
