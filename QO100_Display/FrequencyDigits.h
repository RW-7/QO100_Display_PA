// FrequencyDigits.h
#ifndef FrequencyDigits_h
#define FrequencyDigits_h
#include "globals.h"

class FrequencyDigits {
public:
  uint8_t digitArray[9]; // Array for the digits of the frequency (always 9 digits)
  int numDigits;         // Number of digits in the frequency

  // Constructor
  FrequencyDigits();        // Default constructor
  FrequencyDigits(unsigned long frequency);

private:
  // Functions to extract the digits of the frequency
  void extractDigits(unsigned long frequency);
  int countDigits(unsigned long number);
};

// Function that returns a FrequencyDigits array
FrequencyDigits* getFrequencyDigitsArray(unsigned long frequencies[], int numFrequencies);

#endif