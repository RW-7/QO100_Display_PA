#ifndef GLOBALS_H
#define GLOBALS_H
#include "defines.h"
int touchcalibrate = 0;


// tables for band selection and bittpattern calculation
uint8_t currentBand = NUM_BANDS;  //
// lower limits[kHz] of the bands:
constexpr unsigned long lowlimits[NUM_BANDS] = {
  1791, 3491, 5291, 6991,  9991, 13991, 18051, 20991, 24881, 27991, 49991
};
// upper limits[kHz] of the bands:
constexpr unsigned long uplimits[NUM_BANDS] = {
  2100, 4000, 5400, 7500, 10200, 14500, 18200, 21600, 25000, 29800, 54100
};

constexpr uint8_t band2BCD [NUM_BANDS+1] = { 
// 160    80    60    40     30     20     17     15     12     10      6  NDEF
  0x01, 0x02, 0x03, 0x03,  0x04,  0x05,  0x06,  0x07,  0x08,  0x09,  0x0A, 0x00
};



enum onOff_t:uint8_t {
	OFF = 0,
	ON  = 1,
  NDEF
};

// get the bandnumber matching to the frequency (in kHz)

byte get_Band(unsigned long frq){
  byte i;
  for (i=0; i<NUM_BANDS; i++) {
    if ((frq >= lowlimits[i]) && (frq <= uplimits[i])){
      return i;
    }
  }
  return NUM_BANDS; // no valid band found -> return not defined
}
#endif //GLOBALS_H