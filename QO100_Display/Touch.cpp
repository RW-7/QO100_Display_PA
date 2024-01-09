// Touch.cpp

#include "Touch.h"


// Funktion für die Touch-Behandlung
void touchloop() {
  // Füge hier deine Touch-spezifische Logik hinzu
  // Verwende die globalen Variablen touchX und touchY nach Bedarf
  #ifdef useTouch
    bool touched = tft.getTouch(&touchX, &touchY, 600);
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
