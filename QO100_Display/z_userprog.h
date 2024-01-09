//z_userprog.h
#ifndef Z_USERPROG_H
#define Z_USERPROG_H
#include "globals.h"
#include "pa_settings.h"
void set_HW(uint8_t BCDsetting);
#ifdef WIFI
 void setup_wifi();
#endif
void init_DAC();
void userFrequency(unsigned long newFrequency);
void userSetup();
void userBaseLoop();

#endif //Z_USERPROG_H