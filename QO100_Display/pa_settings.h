// pa_settings.h

#ifndef PA_SETTINGS_H
#define PA_SETTINGS_H
#include "defines.h"
#include "globals.h"  // Include für pa_globals.h
extern int bandvoltage;
// Funktionen und weitere Deklarationen hier
void set_PAbands(unsigned long frequency);
bool PA_ON_HF();
bool PA_ON_SAT();
#endif // PA_SETTINGS_H
