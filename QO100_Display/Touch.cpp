// Touch.cpp
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
          // Je nach Button-Nummer setze den entsprechenden Wert und Status
          if (i == 0) {
            G_Sat = 1;
            userFrequency(G_frequency);
          } else if (i == 1) {
            userFrequency(G_frequency);
            G_Split = 1;
          } else if (i == 2) {
            G_ic705 = 1;
            userFrequency(G_frequency);
          }

          // Aktualisiere den Button
         
          touch_button(buttonX[i], buttonY,buttonLabel[i], WHITE);

          // Setze den Status des Buttons auf gedrückt
          buttonPressed[i] = true;
           Serial.print( "G_Sat x " );
          Serial.println( G_Sat );
            Serial.print( "G_Split x " );
          Serial.println( G_Split );
            Serial.print( "G_ic705 x " );
          Serial.println( G_ic705 );
        } else {
          // Button wurde wieder losgelassen
          // Setze den Button zurück

          touch_button(buttonX[i], buttonY,buttonLabel[i], RED);

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
            G_ic705 = 0;
            userFrequency(G_frequency);
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