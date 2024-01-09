// Touch.h

#ifndef TOUCH_H
#define TOUCH_H

#include <TFT_eSPI.h>  // Stelle sicher, dass du die richtige Bibliothek eingebunden hast

// Externe Deklaration der Touch-Variablen
extern uint16_t touchX;
extern uint16_t touchY;

// Funktion für die Touch-Behandlung
void touchloop();

#endif  // TOUCH_H
