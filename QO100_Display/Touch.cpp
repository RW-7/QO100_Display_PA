// Touch.cpp
#include "globals.h"
#include "Touch.h"
#include "Display.h"

unsigned long debounceDelay = 500;
unsigned long debounceDelay1 = 1000;
unsigned long lastDebounceTime = 0;
void touchloop() {
  #ifdef useTouch
    bool touched = tft.getTouch(&touchX, &touchY, 600);
    unsigned long currentTime = millis();
    if (touched && currentTime - lastDebounceTime > debounceDelay) {
      
      lastDebounceTime = currentTime;
      // Überprüfe, welcher Button berührt wurde
      int buttonCount = sizeof(buttonX) / sizeof(buttonX[0]);

      for (int i = 0; i < buttonCount; i++) {
        if (touchX > buttonX[i] && touchX < buttonX[i] + buttonWidth &&
           touchY > buttonY && touchY < buttonY + buttonHeight) {
        // Überprüfe, ob der Button bereits gedrückt wurde
        if (!buttonPressed[i]) {
         
          // Je nach Button-Nummer setze den entsprechenden Wert und Status
          if (i == 0) {
           // G_Sat = 1;
            //userFrequency(G_frequency);
          } else if (i == 1) {
          //  G_Split = 1;
          //  userFrequency(G_frequency);
          } else if (i == 2) {
          //  ShellyHF = true;
         // PaisOn = PA_ON_HF();
          } else if (i == 3) {
           //HF PA Shelly ON OFF G_ic705 = 1;
          } else if (i == 4) {
         //  ShellySAT = true;
         //  PaisOn = PA_ON_SAT();
           
          } else if (i == 5) {
           //HF PA Shelly ON OFF G_ic705 = 1;
          } else if (i == 6) {
           //HF PA Shelly ON OFF G_ic705 = 1;
          }
          buttonPressed[i] = true;
         // if(PaisOn){
          //  touch_button(buttonX[i], buttonY,buttonLabel[i], RED);
         // }
          
 
          // Setze den Status des Buttons auf gedrückt
          
          
        } else {
          // Button wurde wieder losgelassen
          // Setze den Button zurück
          Serial.print( "buttonPressedx False i: " );
          Serial.print( i );
          Serial.print( " buttonPressedx False: " );
          Serial.println( buttonPressed[i] );
          
          //touch_button(buttonX[i], buttonY,buttonLabel[i], GREEN);

          // Setze den Status des Buttons auf nicht gedrückt
          buttonPressed[i] = false;
    
          // Setze den Wert auf 0 zurück
          if (i == 0) {
        //    G_Sat = 0;
          //  userFrequency(G_frequency);
          } else if (i == 1) {
         //   G_Split = 0;
          //  userFrequency(G_frequency);
          } else if (i == 2) {
          //  setShellyStatus(shellyHF_URL,"turn=off");
          //  ShellyHF = false;
           //HF PA Shelly ON OFF G_ic705 = 1;
          } else if (i == 3) {
           
          } else if (i == 4) {
          // ShellySAT = false;
         //  setShellyStatus(shellySAT_URL,"turn=off");
           
          } else if (i == 5) {
           //HF PA Shelly ON OFF G_ic705 = 1;
          } else if (i == 6) {
           //HF PA Shelly ON OFF G_ic705 = 1;
          }
      
        }
      }
    }
  }
    if (touched) {
      #ifdef debug
      
      #endif
    }
  #endif

}