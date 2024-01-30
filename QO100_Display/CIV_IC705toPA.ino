/* 
IC705toPA  - W.Dilling/DK8RW

This device is based on a ESP32 D1 board and shall connect to a standalone IC705 via BT in order 
to get the current frequency from the radio.
This frequency info can be used in multiple ways, e.g. control of an external PA (lowpass filter settings)

Tasks: 
- get the current frequency of the radio and send it via 4 HW-lines (binary coded) to the PA


Notes: 
- Please make sure, that in the menu of IC705 the "Transceive" function is set to ON ... otherwise it
  simply doesn't work.

- for a first check it is worth watching the output of the program in Serial Monitor.
  If everything is fine, you can suppress this output by setting the "debug" switch in
  comments (file a_defines.h). This switch is uncommented by default for obvious reasons ;-)

- if you are using external inverting drivers, pls. make sure that the switch "invDriver"
  is uncommented in a_defines.h

- pls check (and correct, if necessary) the HW Pin numbers in use (file a_defines.h)

- This example has been successfully tested with a PA HLVA 1k3Q from RFpower by Rainer, DK1RS

*/

/* includes -----------------------------------------------------------------*/
#include <Arduino.h>
#include "globals.h"
#include "Display.h"
#include "Touch.h"
//==========  General initialization  of  the device  =========================================
/* timer  variables */
unsigned long time_current_baseloop;       // temporary time of the baseloop entry for calculations
unsigned long time_last_baseloop;          // will be updated at the end of every baseloop run
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
    touchloop();
    if (touchcalibrate){
      touch_calibrate();
    }
    time_last_baseloop = time_current_baseloop;
	} // if BASELOOP_TICK
  
} // end loop
