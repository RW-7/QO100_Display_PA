/* includes -----------------------------------------------------------------*/

#include "globals.h"
#include "Display.h"
#include "CIV_actions.h"


/* timer  variables */
unsigned long time_current_baseloop;       // temporary time of the baseloop entry for calculations
unsigned long time_last_baseloop;          // will be updated at the end of every baseloop run
//-----------------------------------------------------------------------------------------

//==========  General initialization  of  the device  =========================================

void setup() {

  #ifdef debug                        // initialize the serial interface (for debug messages)
    Serial.begin(debugBdRate);
    Serial.println("");
    delay(20);
    Serial.println (VERSION_STRING);
  #endif
   CIV_setup(); 
   init_TFT();

 

  time_current_baseloop = millis();
  time_last_baseloop = time_current_baseloop;
  
}

//============================  main  procedure ===============================================

void loop() {

  time_current_baseloop = millis();
  
  if ((time_current_baseloop - time_last_baseloop) > BASELOOP_TICK) {
    // Radio Abfrage Starten
    CIV_getProcessAnswers();
    // Dann Befehle an das Radio schreiben (falls möglich und erforderlich)
    CIV_sendCmds();

    time_last_baseloop = time_current_baseloop;
	} // if BASELOOP_TICK
  
} // end loop
