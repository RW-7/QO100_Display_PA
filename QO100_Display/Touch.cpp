// Touch.cpp
#include "defines.h"
#include "globals.h"
#include "Touch.h"
#include "Display.h"
#include "z_userprog.h"

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
          bool PaisOn = true;
          // Je nach Button-Nummer setze den entsprechenden Wert und Status
          if (i == 0) {
            G_Sat = 1;
            userFrequency(G_frequency);
          } else if (i == 1) {
            G_Split = 1;
            userFrequency(G_frequency);
          } else if (i == 2) {
            ShellyHF = true;
          PaisOn = PA_ON_HF();
          } else if (i == 3) {
           //HF PA Shelly ON OFF G_ic705 = 1;
          } else if (i == 4) {
           ShellySAT = true;
           PaisOn = PA_ON_SAT();
           
          } else if (i == 5) {
           //HF PA Shelly ON OFF G_ic705 = 1;
          } else if (i == 6) {
           //HF PA Shelly ON OFF G_ic705 = 1;
          }
          buttonPressed[i] = true;
          if(PaisOn){
            touch_button(buttonX[i], buttonY,buttonLabel[i], RED);
          }
          
 
          // Setze den Status des Buttons auf gedrückt
          
           Serial.print( "G_Sat x " );
          Serial.println( G_Sat );
            Serial.print( "G_Split x " );
          Serial.println( G_Split );
            Serial.print( "G_ic705 x " );
          Serial.println( G_ic705 );
        } else {
          // Button wurde wieder losgelassen
          // Setze den Button zurück
          Serial.print( "buttonPressedx False i: " );
          Serial.print( i );
          Serial.print( " buttonPressedx False: " );
          Serial.println( buttonPressed[i] );
          
          touch_button(buttonX[i], buttonY,buttonLabel[i], GREEN);

          // Setze den Status des Buttons auf nicht gedrückt
          buttonPressed[i] = false;
    
          // Setze den Wert auf 0 zurück
          if (i == 0) {
            G_Sat = 0;
            userFrequency(G_frequency);
          } else if (i == 1) {
            G_Split = 0;
            userFrequency(G_frequency);
          } else if (i == 2) {
            setShellyStatus(shellyHF_URL,"turn=off");
            ShellyHF = false;
           //HF PA Shelly ON OFF G_ic705 = 1;
          } else if (i == 3) {
           
          } else if (i == 4) {
           ShellySAT = false;
           setShellyStatus(shellySAT_URL,"turn=off");
           
          } else if (i == 5) {
           //HF PA Shelly ON OFF G_ic705 = 1;
          } else if (i == 6) {
           //HF PA Shelly ON OFF G_ic705 = 1;
          }
           Serial.print( "G_Sat x " );
          Serial.println( G_Sat );
            Serial.print( "G_Split x " );
          Serial.println( G_Split );
            Serial.print( "G_ic705 x " );
          Serial.println( G_ic705 );
        }
      }
    }
  }
    if (touched) {
      #ifdef debug
        Serial.print( "Data x " );
        Serial.println( touchX );
        Serial.print( "Data y " );
        Serial.println( touchY );
               Serial.print( "G_Sat x " );
          Serial.println( G_Sat );
            Serial.print( "G_Split x " );
          Serial.println( G_Split );
            Serial.print( "G_ic705 x " );
          Serial.println( G_ic705 );
      #endif
    }
  #endif

}