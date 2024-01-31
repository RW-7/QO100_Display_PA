#ifndef CIV_ACTIONS_H
#define CIV_ACTIONS_H
/* includes -----------------------------------------------------------------*/
#include "globals.h"
#include <CIVmaster.h>  // CIVcmds.h is automatically included in addition
#include "Display.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#include "FrequencyDigits.h"

//-------------------------------------------------------------------------------
// create the civ and ICradio objects in use

CIV civ;  // create the CIV-Interface object
          // ESP -> IC705 (via Bluetooth)

//-------------------------------------------------------------------------------

CIVresult_t CIVresultL;
bool freqReceived = false;  // initially, no frequency info has been received from the radio
uint8_t freqPoll = 0;       // number of initial frequency querys in addtion to the broadcast info
uint16_t lpCnt = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


constexpr uint8_t CIV_C_VFO_DATA_READ[]  = {1,0x26};  
constexpr uint8_t CIV_D_VFOA_READ[]  = {1,0x00}; 
constexpr uint8_t CIV_D_VFOB_READ[]  = {1,0x01}; 
constexpr uint8_t CIV_C_SPLIT_READ[]  = {1,0x0F}; 
//------------------------------------------------------------
// process the frequency received from the radio

void set_Frequenz(unsigned long frequency) {
  unsigned long freq_kHz;
  freq_kHz = frequency / 1000;       // frequency is now in kHz
  G_currentBand = get_Band(freq_kHz);  // get band according the current frequency
  G_frequency = frequency ;
  set_LCD_Curennt_RX();
  if(is_SPLIT == 1 ){
    //TODO red subVFO
  }else{
    G_SPLIT_frequency = 0;
  }
  if(is_SAT == 1 && G_currentBand == SATRXBAND){
    G_Dipslay_RX_frequency=G_frequency+OFFSET_Dipslay_RX;
    if(is_SPLIT == 1){
      G_SPLIT_frequency=G_frequency+OFFSET_SPLIT_RXTX;
      G_Dipslay_TX_frequency=G_SPLIT_frequency+OFFSET_Dipslay_TX;
    }
  }else{
    G_Dipslay_RX_frequency = 0;
    G_Dipslay_TX_frequency = 0;
  }

#ifdef debug
  // Test-output to serial monitor:
  Serial.print("Frequency: ");
  Serial.print(freq_kHz);
  Serial.print("  Band: ");
  Serial.print(G_currentBand);
  Serial.print("  G_SPLIT_frequency LCD: ");
  Serial.print(G_SPLIT_frequency);
  Serial.print("  G_Dipslay_RX_frequency LCD: ");
  Serial.print(G_Dipslay_RX_frequency);
  Serial.print("  G_Dipslay_TX_frequency LCD: ");
  Serial.print(G_Dipslay_TX_frequency);
  Serial.print("  Band LCD: ");
  Serial.println(band2string[G_currentBand]);
#endif
}
bool getShellyStatus(const char *url) {
  // Überprüfen, ob die Verbindung zum WiFi besteht
  if (WiFi.status() == WL_CONNECTED) {
    // HTTP-Anfrage senden
    HTTPClient http;
    Serial.print("Senden einer HTTP-Anfrage an: ");
    Serial.println(url);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      // Antwort erhalten
      String payload = http.getString();
      Serial.println("Antwort erhalten!");
      Serial.println(payload);

      // JSON-Objekt erstellen und den Wert von "ison" extrahieren
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      bool isOn = doc["ison"];
      Serial.print("ISON-Status: ");
      Serial.println(isOn);
      return isOn;
    } else {
      Serial.print("Fehler beim Senden der Anfrage. HTTP-Fehlercode: ");
      Serial.println(httpCode);
      return false;
    }

    http.end();
  }
  return false;
}
bool setShellyStatus(const char *url,String paonoff) {
// Überprüfen, ob die Verbindung zum WiFi besteht
  if (WiFi.status() == WL_CONNECTED) {
    // Daten für die POST-Anfrage vorbereiten
    String postData = paonoff;//"turn=on";
    String getData = String(url) + "?" + paonoff;
    // HTTP-Anfrage senden
    HTTPClient http;
    Serial.print("Senden einer POST-Anfrage an: ");
    Serial.println(getData);

    http.begin(getData);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.GET();

    if (httpCode > 0) {
      // Antwort erhalten
      String payload = http.getString();
      Serial.println("Antwort erhalten!");
      Serial.println(payload);
      // JSON-Objekt erstellen und den Wert von "ison" extrahieren
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      bool isOn = doc["ison"];
      Serial.print("ISON-Status: ");
      Serial.println(isOn);
      return isOn;
    } else {
      Serial.print("Fehler beim Senden der Anfrage. HTTP-Fehlercode: ");
      Serial.println(httpCode);
      return false;
    }

    http.end();
  }
  return false;
}
void updateTimeString() {
  timeClient.update();
  utc_time=timeClient.getFormattedTime();
  setTimeString(utc_time);
}
//Antworten vom Radio
void CIV_getProcessAnswers() {
  // ----------------------------------  check, whether there is something new from the radio
  CIVresultL = civ.readMsg(CIV_ADDR_705);
  if (CIVresultL.retVal <= CIV_NOK) {  // valid answer received !
#ifdef debug
    //Serial.print('.');
#endif
    if (CIVresultL.retVal == CIV_OK_DAV) {  // Data available

      if ((CIVresultL.cmd[1] == CIV_C_F_SEND[1]) ||  // command CIV_C_F_SEND received
          (CIVresultL.cmd[1] == CIV_C_F_READ[1])) {  // command CIV_C_F_READ received

        freqReceived = true;

        // send the band info to the PA:
        set_Frequenz(CIVresultL.value);

      }  
      if ((CIVresultL.cmd[1] == CIV_C_VFO_DATA_READ[1])) {  // command CIV_C_F_READ received
        if(VFOAMode!=CIVresultL.datafield[1]){
          VFOAMode=CIVresultL.datafield[1];
          #ifdef debug
            Serial.print("CIV_C_VFO_DATA_READ 1:"); Serial.println(CIVresultL.datafield[1]);//Mode
          #endif
        }
        if(VFOAModeData!=CIVresultL.datafield[2]){
          VFOAModeData=CIVresultL.datafield[2];
          #ifdef debug
            Serial.print("CIV_C_VFO_DATA_READ 2:"); Serial.println(CIVresultL.datafield[2]);//Data
          #endif
        }
        if(VFOAFilter!=CIVresultL.datafield[3]){
          VFOAFilter=CIVresultL.datafield[3];
          #ifdef debug
            Serial.print("CIV_C_VFO_DATA_READ 3:"); Serial.println(CIVresultL.datafield[3]); //Filter
          #endif
        }

      }                                                                                // command CIV_C_F_SEND or CIV_C_F_READ received
      if ((CIVresultL.cmd[1] == CIV_C_TX[1])) {  // (this is a 2 Byte command!) // && (CIVresultL.cmd[2] == CIV_C_TX[2])
      if(is_RXTX!=CIVresultL.datafield[1]){
        is_RXTX=CIVresultL.datafield[1];
        if(is_RXTX == 1){
          Draw_TX();
        }else{
          Draw_RX();
        }
         //   setRXTX(CIVresultL.datafield[1]);  // store it away and do whatever you want with that ...
        #ifdef debug
          Serial.print("CIV_C_TX1:"); Serial.println(CIVresultL.datafield[1]);
          Serial.print("CIV_C_TX3:"); Serial.println(CIVresultL.datafield[3]);
        #endif
      }
       
       
      }
      if ((CIVresultL.cmd[1] == CIV_C_SPLIT_READ[1])) {  // (this is a 2 Byte command!)
      if(is_SPLIT!=CIVresultL.value){
        is_SPLIT=CIVresultL.value;
        #ifdef debug
          Serial.print("CIV_C_SPLIT_READ Value:"); Serial.println(CIVresultL.value);
          Serial.print("CIV_C_SPLIT_READ 1:"); Serial.println(CIVresultL.datafield[1]);
          Serial.print("CIV_C_SPLIT_READ 2:"); Serial.println(CIVresultL.datafield[2]);
          Serial.print("CIV_C_SPLIT_READ 3:"); Serial.println(CIVresultL.datafield[3]);
          Serial.print("CIV_C_SPLIT_READ 4:"); Serial.println(CIVresultL.datafield[4]);
          Serial.print("CIV_C_SPLIT_READ 5:"); Serial.println(CIVresultL.datafield[5]);
          Serial.print("CIV_C_SPLIT_READ 6:"); Serial.println(CIVresultL.datafield[6]);
          Serial.print("CIV_C_SPLIT_READ 7:"); Serial.println(CIVresultL.datafield[7]);
        #endif
        //   setRXTX(CIVresultL.datafield[1]);  // store it away and do whatever you want with that ...
      }
        
      }
      if (CIVresultL.cmd[1] == CIV_C_MOD_READ[1]) {

        // setModMode(radioModMode_t(CIVresultL.datafield[1]),radioFilter_t(CIVresultL.datafield[2]));
      }
      if ((CIVresultL.cmd[1] == CIV_C_RF_POW[1]) && (CIVresultL.cmd[2] == CIV_C_RF_POW[2])) {  // (this is a 2 Byte command!)

        //setTXPWR(CIVresultL.value);
      }
    }  // Data available
  }    // valid answer received
}

//Abfrage Frequenz,modus .....
void CIV_sendCmds() {
  // ----------------------------------  do a query for frequency, if necessary
  // poll every 500 * 10ms = 5sec until a valid frequency has been received

  if ((freqReceived == false) && ((lpCnt % 500) == 0)) {
    civ.writeMsg(CIV_ADDR_705, CIV_C_F_READ, CIV_D_NIX, CIV_wChk);
    freqPoll++;
    Serial.print("P ");
    civ.writeMsg(CIV_ADDR_705, CIV_C_TX, CIV_D_NIX, CIV_wFast);
    civ.writeMsg(CIV_ADDR_705, CIV_C_MOD_READ, CIV_D_NIX, CIV_wFast);
    civ.writeMsg(CIV_ADDR_705, CIV_C_RF_POW, CIV_D_NIX, CIV_wFast);
    civ.writeMsg(CIV_ADDR_705, CIV_C_VFO_DATA_READ, CIV_D_NIX, CIV_wFast);
    civ.writeMsg (CIV_ADDR_705, CIV_C_SPLIT_READ, CIV_D_NIX , CIV_wFast);
  }
  lpCnt++;
  civ.writeMsg(CIV_ADDR_705, CIV_C_TX, CIV_D_NIX, CIV_wFast);
    civ.writeMsg(CIV_ADDR_705, CIV_C_MOD_READ, CIV_D_NIX, CIV_wFast);
    civ.writeMsg(CIV_ADDR_705, CIV_C_RF_POW, CIV_D_NIX, CIV_wFast);
    civ.writeMsg(CIV_ADDR_705, CIV_C_VFO_DATA_READ, CIV_D_VFOA_READ, CIV_wFast);
    civ.writeMsg (CIV_ADDR_705, CIV_C_SPLIT_READ, CIV_D_NIX , CIV_wFast);
}

void setup_wifi() {
  delay(1000);

  WiFi.mode(WIFI_STA);  //Optional
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("\nConnecting");
  int timeout_counter = 0;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
    timeout_counter++;
    if (timeout_counter >= CONNECTION_TIMEOUT * 5) {
      // ESP.restart();
      Serial.print("no WLAN connect...exit WiFi");
      return;
    }
  }
  timeClient.begin();
  WiFiClient client;  // Deklaration der globalen Variable
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}
void CIV_setup() {
  civ.setupp(true);                // initialize the civ object/module (true means "use BT")
  civ.registerAddr(CIV_ADDR_705);  // tell civ, that this is a valid address to be used
}
#endif  //CIV_ACTIONS_H