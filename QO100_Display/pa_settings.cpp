// pa_settings.h
#include "defines.h"
#include "globals.h"
#include "pa_settings.h"
#include "CIV_actions.h" 
// Deklaration der globalen Variablen aus der globals.h-Datei
bool HF_ptt_Enable;
bool VHF_ptt_Enable;
bool UHF_ptt_Enable;
extern int bandvoltage;
extern int brightness;
extern const int ledChannel;
extern int corrFact;
void set_PAbands(unsigned long frequency) {
  unsigned long freq_kHz;
  freq_kHz = G_frequency / 1000;       // frequency is now in kHz
  G_currentBand = get_Band(freq_kHz);  // get band according to the current frequency

  #ifdef debug
    // Test-output to serial monitor:
    Serial.print("Frequency: ");  Serial.println(freq_kHz);
    Serial.print("Band: ");     Serial.println(G_currentBand);
    Serial.print("Band:string ");     Serial.println(band2string[G_currentBand]);
    Serial.print("Band:int ");     Serial.println(band2int[G_currentBand]);
  #endif

  // MR Hier stellen we het voltage in
  int bandcode;
  bandcode = G_currentBand;
  int sendDAC;   // calculated value we will send to DAC at the end
  int corrFact;  // because if a load exist like PA we will have a lower, non-linear voltage output as calculated
  corrFact = 0;  // add a little bit more mV, here with R=470Ohm and C=22uF at PIN 27

  switch (bandcode) {
    case 0:  // 160M
      // Manual XPA125B 230mV
      bandvoltage = 230;  // in mV
      // 3
      corrFact = 6;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 1:  // 80M
      // Manual XPA125B 460mV
      bandvoltage = 460;
      // 4
      corrFact = 14;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 2:  // 60M
      // Manual XPA125B 690mV
      bandvoltage = 690;
      // 5
      corrFact = 19;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 3:  // 40M
      // Manual XPA125B 920mV
      bandvoltage = 920;
      // 8
      corrFact = 26;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 4:  // 30M
      // Manual XPA125B 1150mV
      bandvoltage = 1150;
      // 8
      corrFact = 30;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 5:  // 20M
      // Manual XPA125B 1380mV
      bandvoltage = 1380;
      // 9
      corrFact = 35;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 6:  // 17M
      // Manual XPA125B 1610mV
      bandvoltage = 1610;
      // 9
      corrFact = 44;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 7:  // 15M
      // Manual XPA125B 1840mV
      bandvoltage = 1840;
      // 10
      corrFact = 58;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 8:  // 12M
      // Manual XPA125B 2070mV
      bandvoltage = 2070;
      // 12
      corrFact = 55;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 9:  // 10M
      // Manual XPA125B 2300mV
      bandvoltage = 2300;
      // 15
      corrFact = 63;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 10:  // 6M
      // Manual XPA125B 2530mV
      bandvoltage = 2530;
      // 15
      corrFact = 71;
      bandvoltage = bandvoltage + corrFact;  // add corrFact for new bandvoltage for calculate
      break;
    case 11:  // 2M
      bandvoltage = 0;
      break;
    case 12:  // 70CM
      bandvoltage = 0;
      break;
    case 13:  // NDEF
      bandvoltage = 0;
      break;
  }

  if (freq_kHz > 0 && freq_kHz < 60000) {
    digitalWrite(C_RELAIS, LOW);
    HF_ptt_Enable = 1;
    VHF_ptt_Enable = 0;
    UHF_ptt_Enable = 0;
  } else if (freq_kHz > 144000 && freq_kHz < 148000) {
    digitalWrite(C_RELAIS, HIGH);
    HF_ptt_Enable = 0;
    VHF_ptt_Enable = 1;
    UHF_ptt_Enable = 0;
  } else if (freq_kHz > 430000 && freq_kHz < 470000) {
    digitalWrite(C_RELAIS, HIGH);
    HF_ptt_Enable = 0;
    VHF_ptt_Enable = 0;
    UHF_ptt_Enable = 1;
  }

  Serial.print("Bandvoltage in mV for XIEGU-PA with LOAD: ");
  Serial.println(bandvoltage);
  Serial.print("Correct factor in mV                    : ");
  Serial.println(corrFact);

  sendDAC = bandvoltage * 1024 / 3300;  // a value at 1024 is 3V3 output without load

  // check if the value not greater as 1024 because we use a resolution of 10bit = 2^10 = 1024 = 3V3
  if (sendDAC > 1024) {
    sendDAC = 1024;
  }

  Serial.print("Send value to DAC: ");
  Serial.println(sendDAC);

  // set analog voltage for bandswitching on PA for XIEGU protocol, e.g. XPA125B or Micro PA50 at PIN 27
  ledcWrite(ledChannel, sendDAC);  // send to ESP32-DAC

  show_Meters();  //Show frequency in kHz and band in Meters (80m etc) on TFT
}
bool PA_ON_HF(){
  if (ShellyHF != 1 || HF_MIN_QRG_DEFAULT > G_frequency ||  HF_MAX_QRG_DEFAULT < G_frequency){
    return setShellyStatus(shellyHF_URL,"turn=off");
  }else{
    return setShellyStatus(shellyHF_URL,"turn=on");}
}
bool PA_ON_SAT(){
Serial.print("ShellySAT ");
    Serial.println(ShellySAT);
    Serial.print("SAT_MIN_QRG_DEFAULT ");
    Serial.println(SAT_MIN_QRG_DEFAULT);
    Serial.print("G_frequency ");
    Serial.println(G_frequency);
        Serial.print("SAT_MAX_QRG_DEFAULT ");
    Serial.println(SAT_MAX_QRG_DEFAULT);
        Serial.print("G_Sat ");
    Serial.println(G_Sat);
            Serial.print("G_Split ");
    Serial.println(G_Split);
  if (ShellySAT != 1 || SAT_MIN_QRG_DEFAULT > G_SPLIT_frequency ||  SAT_MAX_QRG_DEFAULT < G_SPLIT_frequency ||  G_Sat != 1 ||  G_Split != 1){
    return setShellyStatus(shellySAT_URL,"turn=off");
  }else{
    return setShellyStatus(shellySAT_URL,"turn=on");
  }
}