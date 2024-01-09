
// FrequencyDigits.cpp
#include "globals.h"
#include "FrequencyDigits.h"
FrequencyDigits::FrequencyDigits() {
  // Default constructor, can be left empty
}

FrequencyDigits::FrequencyDigits(unsigned long frequency) {
  numDigits = countDigits(frequency);
  extractDigits(frequency);
}

void FrequencyDigits::extractDigits(unsigned long frequency) {
  // Set digitArray to 0
  for (int i = 0; i < 9; i++) {
    digitArray[i] = 0;
  }

  // Fill the array with the digits of the frequency
  unsigned long tempFrequency = frequency;
  for (int i = 0; i < numDigits; i++) {
    digitArray[i] = static_cast<uint8_t>(tempFrequency % 10);
    tempFrequency /= 10;
  }
}

int FrequencyDigits::countDigits(unsigned long number) {
  int count = 0;
  while (number > 0) {
    number /= 10;
    count++;
  }
  return count;
}

FrequencyDigits* getFrequencyDigitsArray(unsigned long frequencies[], int numFrequencies) {
  FrequencyDigits* freqDigitsArray = new FrequencyDigits[numFrequencies];

  for (int i = 0; i < numFrequencies; i++) {
    freqDigitsArray[i] = FrequencyDigits(frequencies[i]);
  }

  return freqDigitsArray;
}