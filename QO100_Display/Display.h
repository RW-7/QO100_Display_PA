// Display.h

#ifndef DISPLAY_H
#define DISPLAY_H

#include "globals.h"      // Include the necessary global header file
#include <Arduino.h>      // Include Arduino core library for data types and functions
#include "Display_functionen.h"

#ifdef LCD
#include <TFT_eSPI.h>     // Include the TFT library if LCD is defined

extern TFT_eSPI tft;      // Declare the TFT_eSPI object as external

// Function declarations
void init_TFT(void);

#endif  // LCD

#endif  // DISPLAY_H
