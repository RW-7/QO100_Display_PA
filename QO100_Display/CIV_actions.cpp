//CIV_actions.cpp
/* 
CIV_template - b_CIVactions W.Dilling/DK8RW

Help procedures for the sketch CIV_template

The procedures "CIV_sendCmds()" and "CIV_getProcessAnswers()"
are down at the bottom of this file!

*/
#include "globals.h"
#include "CIV_actions.h"
#include "Display_functionen.h"
#include "z_userprog.h"
#include <CIVmaster.h> // CIVcmds.h wird automatisch zusätzlich eingebunden
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Definition der inline-Funktion
// additional CI-V commands, which are only available for IC 705 (and not officially supported by CIVmasterLib)
 constexpr uint8_t CIV_C_selVFO2_READ[]  = {2,0x26,0x00};  
/*!*/
uint16_t t_curr_last;
CIVresult_t CIVresultLM;
const String retValStr[7] = {
  "CIV_OK",
  "CIV_OK_DAV",

  "CIV_NOK",
  "CIV_HW_FAULT",
  "CIV_BUS_BUSY",
  "CIV_BUS_CONFLICT",
  "CIV_NO_MSG"
};
// Radio database procedures ==================================================================

//---------------------------------------------------------------------------------------------
// Handle the ON/OFF state of the radio

void  setRadioOnOff(radioOnOff_t newState) {

  if (newState>RADIO_NDEF)  // just to be on the safe side, since the data is coming from 
    newState=RADIO_NDEF;    // outside of the system

  if (G_radioOn!=newState){
    G_radioOn=newState;

    #ifdef fastPTT
      if (newState==RADIO_ON) { // enable the transceive function of the command "CIV_C_TXP"
                                // only once after successful connection, therefore .. fire and forget
        civ.writeMsg (civAddr,CIV_C_TXP,CIV_D_TXP_ON,CIV_wFast);
//        delay (5);
//        civ.writeMsg (civAddr,CIV_C_TXP,CIV_D_TXP_EN, CIV_wFast); // obviously not necessary ...
//        civ.writeMsg (civAddr,CIV_C_TXP,CIV_D_TXP_DIS,CIV_wFast);
        #ifdef debug
          Serial.println ("TXPen");
        #endif
      }
    #endif

    //!// put your code here, if you want to react on the change of ON/OFF ...
    BT_Conn_Status((radioOnOffStr[newState]).c_str());

    #ifdef debug
      Serial.print("CONN/DISC?:"); Serial.println(radioOnOffStr[newState]);
    #endif

  }

    //!// or put your code here ...

}
//---------------------------------------------------------------------------------------------
// Handle the RX/TX state of the radio

void setRXTX(uint8_t newState) {

  if (newState>ON)        // just to be on the safe side, since the data is coming from 
    newState=ON;          // outside of the system

  if (G_RXTX!=newState){
    G_RXTX=newState;

    if (G_RXTX==1) { eval_delayMeas; }      // debugging: calculate the time since the input trigger changed and print it out
//    eval_delayMeas;       // debugging: calculate the time since the input trigger changed and print it out

    userPTT(newState);    // call into z_userprog.ino ...

    #ifdef debug
      Serial.println(RXTXstr[newState]);
    #endif
  }
  
}

//---------------------------------------------------------------------------------------------
// Handle new frequency information

void  setFrequency(unsigned long newFrequency) {

  if (G_frequency!=newFrequency){
    G_frequency=newFrequency;

    userFrequency(newFrequency);  // call into z_userprog.ino ...
    
    #ifdef debug
      Serial.print("Freq:"); Serial.println(newFrequency);
    #endif
  }
  
}

void  setVFOMODE(unsigned long newVFO) {

    #ifdef debug
      Serial.print("VFO:"); Serial.println(newVFO);
    #endif 
}

// add by DL1BZ
void setTXPWR(unsigned short newTXPWR) {

  if (G_TXPWR!=newTXPWR){
    
    G_TXPWR=newTXPWR;

    user_TXPWR(newTXPWR);    // call into z_userprog.ino ...

    #ifdef debug
      Serial.print("TXPWR:"); Serial.println(newTXPWR);
    #endif
  }
}

#ifdef modmode
//---------------------------------------------------------------------------------------------
// Handle modulation mode and RX Filter setting

void  setModMode(radioModMode_t newModMode, radioFilter_t newRXfilter) {

  if (newModMode==radioModMode_t(0x17)) newModMode = MOD_DV;  // ICOM definition of "DV" is 0x17, mine is 0x09 ...

  if (newModMode>MOD_NDEF)    // just to be on the safe side, since the data is coming from 
    newModMode=MOD_NDEF;      // outside of the system

  if (newRXfilter>FIL3)       // just to be on the safe side, since the data is coming from 
    newRXfilter=FIL_NDEF;     // outside of the system

  if ((G_Mod != newModMode) || (G_RXfilter != newRXfilter)) {
    G_Mod = newModMode;G_RXfilter = newRXfilter;


    //!// put your code here, if you want to react on the change ...
    show_Mode(newModMode, newRXfilter);    // call into z_userprog.ino ...
  
    #ifdef debug
      Serial.print ("Mod:  "); Serial.print   (modModeStr[newModMode]);
      Serial.print (" Fil: "); Serial.println (FilStr[newRXfilter]);
    #endif
  
  }

    //!// or put your code here ...
  
}
#endif

//=============================================================================================
// get the answers from the radio

void  CIV_getProcessAnswers() {

  // not necessary in case of fastPTT (necessary at all?)
  #ifndef fastPTT
    // if a query request has taken place recently -> wait a bit in order to give the radio time!
    if ( uint16_t(t_curr_lp-ts_CIVcmdSent) <= t_gapAfterquery ) return;
  #endif
  CIVresultL = civ.readMsg(civAddr);
  
 
// Serial.println((char*)&CIVresultL.retVal);
  //Serial.println((char*)&CIVresultL.address);
  // Serial.println((char*)&CIVresultL.value);
 // Serial.println((char*)&CIVresultL.cmd);
  //Serial.println(CIVresultL.cmd[1]);


  if (CIVresultL.retVal<=CIV_NOK) { //--------------------------------- valid answer received !
                              
    if (CIVresultL.retVal==CIV_OK_DAV) { // Data available ....................................

      setRadioOnOff(RADIO_ON);  // in any case, there was a valid answer -> radio is here !

      // 1. check the "transceive" informations from the radio (i.e. the info which is 
      //    sent by the radio without query) e.g. when turning the VFO knob

      #ifdef fastPTT
        if (CIVresultL.cmd[1]==CIV_C_TXP[1]) {  // TX output power broadcast received
          setRXTX(CIVresultL.datafield[3]);     // store it away and do whatever you want with that ...
        }
      #endif
      
      if (CIVresultL.cmd[1]==CIV_C_F_SEND[1]) {   // operating frequency broadcast
        setFrequency(CIVresultL.value);
      }
    
      #ifdef modmode
      if (CIVresultL.cmd[1]==CIV_C_MOD_SEND[1]) { // ModMode
        setModMode(radioModMode_t(CIVresultL.datafield[1]),radioFilter_t(CIVresultL.datafield[2]));
      } 
      #endif
      
      // by DL1BZ read RF Power
      if ((CIVresultL.cmd[1]==CIV_C_RF_POW[1]) && 
          (CIVresultL.cmd[2]==CIV_C_RF_POW[2])) { // (this is a 2 Byte command!)
        CIVwaitForAnswer  = false;
        setTXPWR(CIVresultL.value);        
      }
          
      // 2. check the answers to the queries initiated by the CIV master !

      // answer to command CIV_C_TX received ...
      if ((CIVresultL.cmd[1]==CIV_C_TX[1]) && 
          (CIVresultL.cmd[2]==CIV_C_TX[2])) { // (this is a 2 Byte command!)
        CIVwaitForAnswer  = false;
        #ifndef fastPTT
          setRXTX(CIVresultL.datafield[1]);  // store it away and do whatever you want with that ...
        #endif
      }

  //!//
  // put your code here in case you have added querys which shall be ansered by the radio 
  // with data

      // answer to query for operating frequency received ...
      if (CIVresultL.cmd[1]==CIV_C_F_READ[1]) {
        CIVwaitForAnswer  = false;
        setFrequency(CIVresultL.value);
      }


      #ifdef modmode
      // answer to query for ModMode received ...
      if (CIVresultL.cmd[1]==CIV_C_MOD_READ[1]) {
        CIVwaitForAnswer  = false;

        setModMode(radioModMode_t(CIVresultL.datafield[1]),radioFilter_t(CIVresultL.datafield[2]));
      } 
      #endif
      
    
      //add by DL1BZ get TXPWR
      if ((CIVresultL.cmd[1]==CIV_C_RF_POW[1]) && 
          (CIVresultL.cmd[2]==CIV_C_RF_POW[2])) { // (this is a 2 Byte command!)
        CIVwaitForAnswer  = false;
        setTXPWR(CIVresultL.value);        
      }

    } //  end Data available
    else { // there has been an answer from the radio saying OK or NOK
      CIVwaitForAnswer  = false;

      if (CIVresultL.retVal==CIV_OK) {
        setRadioOnOff(RADIO_ON);  // valid OK answer -> radio is here !
      }
      else {
        // since the IC9700 answers with "NOK" also in the OFF mode -> do not assume, that the radio is ON!
      }

      //!//
      // put your code here, if necessary, i.e. if you want to process the OK/NOK answer from the radio
      // for a specific command
      
    }

  } // end valid answer received
  else { //------------------------------------------------------------ no answer received
    if ( CIVwaitForAnswer &&                                        // still waiting for answer
         (uint16_t(t_curr_lp - ts_CIVcmdSent) >= t_waitForAnswer)       // -> timeout !
       ) {
       CIVwaitForAnswer = false;
       setRadioOnOff(RADIO_OFF);  // radio is not available, probably off 
    }
  }

}

//=============================================================================================
// send commands to the radio

void  CIV_sendCmds() {

  // do the RXTX poll (independent whether the radio is connected or not) .....................
  // by this, also the radioON/OFF state can be checked
  if (uint16_t(t_curr_lp-ts_RXTX_sent) >= t_RXTXquery) { // it's time to ask the radio

    if (CIVwaitForAnswer==false) { // ask only, if we currently are not waiting for the radio
      civ.writeMsg (civAddr,CIV_C_TX,CIV_D_NIX,CIV_wFast);
      CIVwaitForAnswer  = true;

/*!*/
//Serial.print(t_curr_lp); Serial.print("  ");Serial.println(uint16_t(t_curr_lp-t_curr_last));
//t_curr_last = t_curr_lp;


        
      ts_CIVcmdSent     = t_curr_lp;  // store the time of the query
      ts_RXTX_sent      = t_curr_lp; 
    }

  }
      
  //-------------------------------------------------------------------------------------------
  // the other queries shall take place only if the radio is connected
  if (G_radioOn==RADIO_ON) {


  //!//
  // put your code here in case you add querys...
  // please don't forget: if you add a query command for another radio parameter,
  // you need to add also the same command in the answer section above and a query timer
  // in the main tab!


    // slow poll of the frequency, just to be on the safe side .................................
    if (uint16_t(t_curr_lp - ts_f_sent) >= t_slowQuery) {  // ask the radio

      if (CIVwaitForAnswer==false) { // ask only, if we currently are not waiting for the radio
        civ.writeMsg (civAddr,CIV_C_F_READ,CIV_D_NIX,CIV_wFast);
        CIVwaitForAnswer  = true;
        ts_CIVcmdSent     = t_curr_lp;  // store the time of the query
        ts_f_sent         = t_curr_lp; 
      }

    }
    // if (uint16_t(t_curr_lp - ts_25_sent) >= t_slowQuery) {  // ask the radio

    //   if (CIVwaitForAnswer==false) { // ask only, if we currently are not waiting for the radio
    //     CIVresultLM = civ.writeMsg(civAddr,CIV_C_selVFO_READ,CIV_D_NIX,CIV_wFast);
    //     if (CIVresultLM.retVal<=CIV_NOK) {  // valid answer received 
    //       if (CIVresultLM.retVal==CIV_OK_DAV) {// Data available
    //         Serial.print(" value: elVFO1: "); Serial.println(CIVresultLM.value);
    //         Serial.print(" datafield1: elVFO1: "); Serial.println(CIVresultLM.datafield[1]);
    //         Serial.print(" datafield2: elVFO1: "); Serial.println(CIVresultLM.datafield[2]);
    //       }
    //     }
    //     CIVwaitForAnswer  = true;
    //     ts_CIVcmdSent     = t_curr_lp;  // store the time of the query
    //     ts_25_sent       = t_curr_lp;
    //   }

    // }
    // if (uint16_t(t_curr_lp - ts_26_sent) >= t_slowQuery) {  // ask the radio

    //  if (CIVwaitForAnswer==false) { // ask only, if we currently are not waiting for the radio
    //      CIVresultLM = civ.writeMsg (civAddr,CIV_C_selVFO2_READ,CIV_D_NIX,CIV_wFast);
    //     if (CIVresultLM.retVal<=CIV_NOK) {  // valid answer received 
    //       if (CIVresultLM.retVal==CIV_OK_DAV) {// Data available
    //         Serial.print(" value: elVFO2"); Serial.println(CIVresultLM.value);
    //         Serial.print(" datafield1: elVFO2"); Serial.println(CIVresultLM.datafield[1]);
    //         Serial.print(" datafield2: elVFO2"); Serial.println(CIVresultLM.datafield[2]);
    //       }
    //     }
    //     CIVwaitForAnswer  = true;
    //     ts_CIVcmdSent     = t_curr_lp;  // store the time of the query
    //     ts_26_sent       = t_curr_lp;
    //   }

    // }
    #ifdef modmode
    // slow poll of the ModMode, just to be on the safe side ..................................
    if (uint16_t(t_curr_lp - ts_Mod_sent) >= t_slowQuery) {  // ask the radio

      if (CIVwaitForAnswer==false) { // ask only, if we currently are not waiting for the radio
        civ.writeMsg (civAddr,CIV_C_MOD_READ,CIV_D_NIX,CIV_wFast);
        CIVwaitForAnswer  = true;
        ts_CIVcmdSent     = t_curr_lp;  // store the time of the query
        ts_Mod_sent       = t_curr_lp;
      }
      
    }
    #endif

    // add by DL1BZ slow poll of the TXPWR, just to be on the safe side ..................................
    if (uint16_t(t_curr_lp - ts_TXPWR_sent) >= t_slowQuery) {  // ask the radio

      if (CIVwaitForAnswer==false) { // ask only, if we currently are not waiting for the radio
        civ.writeMsg (civAddr,CIV_C_RF_POW,CIV_D_NIX,CIV_wFast);
        CIVwaitForAnswer  = true;
        ts_CIVcmdSent     = t_curr_lp;  // store the time of the query
        ts_TXPWR_sent     = t_curr_lp;
      }

    }

  } // if RADIO_ON

}

byte get_Band(unsigned long frq) {
  byte i;
  for (i = 0; i < NUM_BANDS; i++) {
    //for (i=1; i<NUM_BANDS; i++) {
    if ((frq >= lowlimits[i]) && (frq <= uplimits[i])) {
      return i;
    }
  }
  return NUM_BANDS;  // no valid band found -> return not defined
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
void setTRXPWR (){

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

