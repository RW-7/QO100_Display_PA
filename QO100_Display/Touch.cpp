// Touch.cpp
#include "globals.h"
#include "Touch.h"
#include "Display.h"

int value1 = 0;
int value2 = 0;
int value3 = 0;

// Funktion für die Touch-Behandlung
void touchloop() {
  // Füge hier deine Touch-spezifische Logik hinzu
  // Verwende die globalen Variablen touchX und touchY nach Bedarf
  #ifdef useTouch
    bool touched = tft.getTouch(&touchX, &touchY, 600);
    if (tft.getTouch(&touchX, &touchY)) {
    // Überprüfe, welcher Button berührt wurde
    for (int i = 0; i < 3; i++) {
      if (touchX > buttonX[i] && touchX < buttonX[i] + buttonWidth &&
          touchY > buttonY && touchY < buttonY + buttonHeight) {
        // Überprüfe, ob der Button bereits gedrückt wurde
        if (!buttonPressed[i]) {
          // Je nach Button-Nummer setze den entsprechenden Wert und Status
          if (i == 0) {
            G_Sat++;
          } else if (i == 1) {
            G_Split++;
          } else if (i == 2) {
            G_ic705++;
          }

          // Aktualisiere den Button
          drawButton(buttonX[i], buttonY, buttonWidth, buttonHeight, (i == 0) ? "Sat" : ((i == 1) ? "Split" : "Ic705"), true);

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
          drawButton(buttonX[i], buttonY, buttonWidth, buttonHeight, (i == 0) ? "Sat" : ((i == 1) ? "Split" : "Ic705"), false);

          // Setze den Status des Buttons auf nicht gedrückt
          buttonPressed[i] = false;
    
          // Setze den Wert auf 0 zurück
          if (i == 0) {
            G_Sat = 0;
          } else if (i == 1) {
            G_Split = 0;
          } else if (i == 2) {
            G_ic705 = 0;
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

    #endif

    }
  #endif
}
