#ifndef CIV_ACTIONS_H
#define CIV_ACTIONS_H
/* includes -----------------------------------------------------------------*/
#include "globals.h"
#include <CIVmaster.h>  // CIVcmds.h is automatically included in addition
#include "Display.h"
//-------------------------------------------------------------------------------
// create the civ and ICradio objects in use

CIV civ;  // create the CIV-Interface object
          // ESP -> IC705 (via Bluetooth)

//-------------------------------------------------------------------------------

CIVresult_t CIVresultL;
bool freqReceived = false;  // initially, no frequency info has been received from the radio
uint8_t freqPoll = 0;       // number of initial frequency querys in addtion to the broadcast info
uint16_t lpCnt = 0;
constexpr uint8_t CIV_C_VFO_DATA_READ[]  = {1,0x26};  
constexpr uint8_t CIV_D_VFOA_READ[]  = {1,0x00}; 
constexpr uint8_t CIV_D_VFOB_READ[]  = {1,0x01}; 
constexpr uint8_t CIV_C_SPLIT_READ[]  = {1,0x0F}; 
//------------------------------------------------------------
// process the frequency received from the radio

void set_PAbands(unsigned long frequency) {
  unsigned long freq_kHz;

  freq_kHz = frequency / 1000;       // frequency is now in kHz
  currentBand = get_Band(freq_kHz);  // get band according the current frequency
  set_LCD_Curennt_RX(frequency);
#ifdef debug
  // Test-output to serial monitor:
  Serial.print("Frequency: ");
  Serial.print(freq_kHz);
  Serial.print("  Band: ");
  Serial.print(currentBand);
  Serial.print("  Band LCD: ");
  Serial.println(band2string[currentBand]);
#endif
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
        set_PAbands(CIVresultL.value);

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

void CIV_setup() {
  civ.setupp(true);                // initialize the civ object/module (true means "use BT")
  civ.registerAddr(CIV_ADDR_705);  // tell civ, that this is a valid address to be used
}
#endif  //CIV_ACTIONS_H