#include <CIVmaster.h>
#include "globals.h"
#include "FrequencyDigits.h"
#include "Display_functionen.h"
//-------------------------------------------------------------------------------
// create the civ and ICradio objects in use
// Forward declarations
CIV     civ;    // create the CIV-Interface object
                // ESP -> IC705 (via Bluetooth)

//-------------------------------------------------------------------------------
CIVresult_t CIVresultL;
uint16_t lpCnt = 0;
uint8_t currentBand = NUM_BANDS;  // not defined
bool    freqReceived = false; // initially, no frequency info has been received from the radio
uint8_t freqPoll     = 0;     // number of initial frequency querys in addtion to the broadcast info

/* module wide variables ----------------------------------------------------*/

// for Debugging ...
#ifdef debug
  unsigned long G_timemarker1;
  unsigned long G_timemarker1a;

  unsigned long G_timemarker2;
  unsigned long G_timemarker2a;
#endif



//------------------------------------------------------------
// process the frequency received from the radio
byte get_Band(unsigned long frq){
  byte i;
  for (i=0; i<NUM_BANDS; i++) {
    if ((frq >= lowlimits[i]) && (frq <= uplimits[i])){
      return i;
    }
  }
  return NUM_BANDS; // no valid band found -> return not defined
}
void set_PAbands(unsigned long frequency) {
  unsigned long freq_kHz;
 Serial.print("  set_PAbands: ");     Serial.print(frequency); 
  freq_kHz = frequency/1000;              // frequency is now in kHz
  currentBand = get_Band(freq_kHz);       // get band according the current frequency

#ifdef debug
  // Test-output to serial monitor:
  set_LCD_Curennt_RX(frequency);
  Serial.print("Frequency: ");  Serial.print(freq_kHz);
  Serial.print("  Band: ");     Serial.print(currentBand);  
#endif

}

//Antworten vom Radio
void  CIV_getProcessAnswers() {
  // ----------------------------------  check, whether there is something new from the radio
    CIVresultL = civ.readMsg(CIV_ADDR_705);
    
    if (CIVresultL.retVal<=CIV_NOK) {               // valid answer received !
      #ifdef debug
        Serial.print('.');
      #endif
      if (CIVresultL.retVal==CIV_OK_DAV) {          // Data available

        if ((CIVresultL.cmd[1]==CIV_C_F_SEND[1]) ||  // command CIV_C_F_SEND received
            (CIVresultL.cmd[1]==CIV_C_F_READ[1])) {  // command CIV_C_F_READ received

          freqReceived = true;

          // send the band info to the PA:
          set_PAbands(CIVresultL.value);

        } // command CIV_C_F_SEND or CIV_C_F_READ received  
          // answer to command CIV_C_TX received ...
      if ((CIVresultL.cmd[1]==CIV_C_TX[1]) && 
          (CIVresultL.cmd[2]==CIV_C_TX[2])) { // (this is a 2 Byte command!)
      
        #ifndef fastPTT
       //   setRXTX(CIVresultL.datafield[1]);  // store it away and do whatever you want with that ...
        #endif
        }
       if (CIVresultL.cmd[1]==CIV_C_MOD_READ[1]) {
     

       // setModMode(radioModMode_t(CIVresultL.datafield[1]),radioFilter_t(CIVresultL.datafield[2]));
      } 
      if ((CIVresultL.cmd[1]==CIV_C_RF_POW[1]) && 
          (CIVresultL.cmd[2]==CIV_C_RF_POW[2])) { // (this is a 2 Byte command!)
        
        //setTXPWR(CIVresultL.value);        
      }


      } // Data available
    } // valid answer received
}

//Abfrage Frequenz,modus .....
void  CIV_sendCmds() {
// ----------------------------------  do a query for frequency, if necessary
    // poll every 500 * 10ms = 5sec until a valid frequency has been received
  Serial.print (" freqReceived: "); Serial.println (freqReceived);
  Serial.print (" lpCnt: "); Serial.println (lpCnt);
  Serial.print (" freqPoll: "); Serial.println (freqPoll);
    if ( (freqReceived == false) && ((lpCnt%500)==0) ) { 
      civ.writeMsg (CIV_ADDR_705, CIV_C_F_READ, CIV_D_NIX, CIV_wChk);
      freqPoll++;
      Serial.print ("P ");
      civ.writeMsg (CIV_ADDR_705,CIV_C_TX,CIV_D_NIX,CIV_wFast);
      civ.writeMsg (CIV_ADDR_705,CIV_C_MOD_READ,CIV_D_NIX,CIV_wFast);
      civ.writeMsg (CIV_ADDR_705,CIV_C_RF_POW,CIV_D_NIX,CIV_wFast);
      lpCnt++;
    }
}
void CIV_setup(){
  civ.setupp(true);                   // initialize the civ object/module (true means "use BT")
  civ.registerAddr(CIV_ADDR_705);     // tell civ, that this is a valid address to be used
}
//-----------------------------------------------------------------------------------------
// get the bandnumber matching to the frequency (in kHz)



