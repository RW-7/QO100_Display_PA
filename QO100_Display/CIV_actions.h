// CIV_actions.h

#ifndef CIV_ACTIONS_H
#define CIV_ACTIONS_H

#include "globals.h"
#include <Arduino.h>
#include <CIVmaster.h> // CIVcmds.h wird automatisch zusätzlich eingebunden

#ifdef fastPTT
constexpr uint8_t CIV_C_TXP[] = {1, 0x24};
constexpr uint8_t CIV_D_TXP_ON[] = {3, 0x00, 0x00, 0x01};
constexpr uint8_t CIV_D_TXP_EN[] = {3, 0x00, 0x01, 0x01};
constexpr uint8_t CIV_D_TXP_DIS[] = {3, 0x00, 0x01, 0x00};
#endif

// Forward declarations
enum radioModMode_t : uint8_t;
enum radioFilter_t : uint8_t;
enum radioOnOff_t : uint8_t;


// Function declarations
void setRadioOnOff(radioOnOff_t newState);
void setRXTX(uint8_t newState);
void setFrequency(unsigned long newFrequency);
void setVFOMODE(unsigned long newVFO);
void setTXPWR(unsigned short newTXPWR);

#ifdef modmode
void setModMode(radioModMode_t newModMode, radioFilter_t newRXfilter);
#endif

void CIV_getProcessAnswers();
void CIV_sendCmds();
//void userFrequency(unsigned long newFrequency);
constexpr uint8_t getVFOQRGByte(uint8_t highDigit, uint8_t lowDigit) {
    return (0x00 | (highDigit << 4) | lowDigit);
}

byte get_Band(unsigned long frq);

#endif  // CIV_ACTIONS_H
