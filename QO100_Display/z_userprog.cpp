//z_userprog.cpp
/*
CIV_template - z_userprog - adapted for the requirements of Glenn, VK3PE by DK8RW, May 16, 22
    vk3pe is using a "TTGO" ESP32 module.
    Band select BCD outputs set to be active Hi.NOTE: a BCD to Decimal chip will be used also
     to provide 10 band outputs.
    PTT output is active LOW

This is the part, where the user can put his own procedures in

The calls to this user programs shall be inserted wherever it suits - search for //!//
in all files

*/
#include "z_userprog.h"
#include "globals.h"
#include "defines.h"
#include "pa_settings.h"
#include "FrequencyDigits.h"
#include "Display_functionen.h"
// IMPORTANT!

// Deklaration der globalen Variablen aus der globals.h-Datei
extern boolean HF_ptt_Enable;
extern boolean VHF_ptt_Enable;
extern boolean UHF_ptt_Enable;
extern int bandvoltage;
extern int brightness;
extern const int ledChannel;
extern int corrFact;
//=====================================================
// this is called, when the RX/TX state changes ...in Dispaly.cpp
//=====================================================
// void userPTT(uint8_t newState) {

// #ifdef debug
//   Serial.println(newState);  //prints '1' for Tx, '0' for Rx
// #endif
//   tft.setFreeFont(&FreeSansBold9pt7b);  //previous setup text was smaller.
//   //tft.setTextColor(WHITE) ;


//   if (newState) {  // '1' = Tx mode
//     Draw_TX();
//   }  //Tx mode
//   else {
//     Draw_RX();
//   }  //Rx mode

// #ifdef Inv_PTT
//   digitalWrite(PTTpin, !newState);  //--inverted-- output version:  Clr =Tx, Hi =Rx
//   if (HF_ptt_Enable) {
//     digitalWrite(PTTpinHF, !newState);
//   }
//   if (VHF_ptt_Enable) {
//     digitalWrite(PTTpinVHF, !newState);
//   }
//   if (UHF_ptt_Enable) {
//     digitalWrite(PTTpinUHF, !newState);
//   }
// #else
//   digitalWrite(PTTpin, newState);  // Clr =Rx, Hi =Tx
//   if (HF_ptt_Enable) {
//     digitalWrite(PTTpinHF, newState);
//   }
//   if (VHF_ptt_Enable) {
//     digitalWrite(PTTpinVHF, newState);
//   }
//   if (UHF_ptt_Enable) {
//     digitalWrite(PTTpinUHF, newState);
//   }
// #endif
// }


//------------------------------------------------------------
// set the bitpattern in the HW

void set_HW(uint8_t BCDsetting) {



#ifdef debug
  // Test output to control the proper functioning:
  Serial.print(" Pins ");
#endif
}

//-----------------------------------------------------------------------------------------
// get the bandnumber matching to the frequency (in kHz)


#ifdef WIFI
void setup_wifi() {
  delay(1000);

  WiFi.mode(WIFI_STA);  //Optional
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("\nConnecting");
  int timeout_counter = 0;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
    timeout_counter++;
    if (timeout_counter >= CONNECTION_TIMEOUT * 5) {
      // ESP.restart();
      Serial.print("no WLAN connect...exit WiFi");
      return;
    }
  }
  timeClient.begin();
  extern WiFiClient client;  // Deklaration der globalen Variable
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}
#endif

void init_DAC() {
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);
}

//=========================================================================================
// this is called, whenever there is new frequency information ...
// this is available in the global variable "G_frequency" ...
void userFrequency(unsigned long newFrequency) {
  #ifndef usePA
    set_PAbands(G_frequency);
  #endif
    Serial.print( "userFrequency G_Sat x " );
          Serial.println( G_Sat );
    set_LCD_Curennt_RX(G_frequency);
    
  if (G_Split == 1){
    #ifdef debug
      Serial.print ("userFrequency:"); Serial.println (G_frequency);
      Serial.print ("G_RXTX:"); Serial.println (G_RXTX);
      Serial.print ("G_QO100:"); Serial.println (G_QO100);
    #endif
    civ.writeMsg (civAddr, CIV_D_Splitt_ON, CIV_D_NIX , CIV_wFast);
  }else{
    civ.writeMsg (civAddr, CIV_D_Splitt_OFF, CIV_D_NIX , CIV_wFast);
  } 
  // if RX QO100 // Set Band & Mod
  if (G_RXTX==0) {

    G_SPLIT_frequency = G_frequency + OFFSET_SPLIT_RXTX; // TX Frequenz für Upconverter subVFO
    set_LCD_Curennt_TX(G_SPLIT_frequency);
    G_Dipslay_RX_frequency = G_frequency + OFFSET_Dipslay_RX;
    set_LCD_SAT_RX(G_Dipslay_RX_frequency);
    G_Dipslay_TX_frequency = G_SPLIT_frequency + OFFSET_Dipslay_TX;
    set_LCD_SAT_TX(G_Dipslay_TX_frequency);

    FrequencyDigits freqDigits = FrequencyDigits(G_SPLIT_frequency);
    #ifdef debug
    Serial.println ("RX");
    Serial.print ("G_SPLIT_frequency:"); Serial.println (G_SPLIT_frequency);
    Serial.print ("G_Dipslay_RX_frequency:"); Serial.println (G_Dipslay_RX_frequency); 
    Serial.print ("G_Dipslay_TX_frequency:"); Serial.println (G_Dipslay_TX_frequency);
    Serial.print("1 HZ:"); Serial.println(freqDigits.digitArray[0]);
    Serial.print("10HZ:"); Serial.println(freqDigits.digitArray[1]);
    Serial.print("100Hz:"); Serial.println(freqDigits.digitArray[2]);
    Serial.print("1KHZ:"); Serial.println(freqDigits.digitArray[3]);
    Serial.print("10KHZ:"); Serial.println(freqDigits.digitArray[4]);
    Serial.print("100KHZ:"); Serial.println(freqDigits.digitArray[5]);
    Serial.print("1MHZ:"); Serial.println(freqDigits.digitArray[6]);
    Serial.print("10MHZ:"); Serial.println(freqDigits.digitArray[7]);
    Serial.print("100MHZ:"); Serial.println(freqDigits.digitArray[8]);
    #endif
    uint8_t CIV_T_SetVFOQRG[] = {
        5, // Statisch
        getVFOQRGByte(freqDigits.digitArray[1], freqDigits.digitArray[0]),
        getVFOQRGByte(freqDigits.digitArray[3], freqDigits.digitArray[2]),
        getVFOQRGByte(freqDigits.digitArray[5], freqDigits.digitArray[4]),
        getVFOQRGByte(freqDigits.digitArray[7], freqDigits.digitArray[6]),
        getVFOQRGByte(0, freqDigits.digitArray[8])
    };
    //Set Frequenz
     if (G_Split == 1){
      civ.writeMsg(civAddr, CIV_C_unSelVFO_READ, CIV_T_SetVFOQRG, CIV_wFast);
     }
  }

}


//=========================================================================================
// this will be called in the setup after startup
void userSetup() {

  Serial.println(VERSION_USER);

  // set the used HW pins (see defines.h!) as output and set it to 0V (at the Input of the PA!!) initially

  pinMode(PTTpin, OUTPUT);     //PTT out pin
  pinMode(PTTpinHF, OUTPUT);   //PTTHF out pin
  pinMode(PTTpinVHF, OUTPUT);  //PTTVHF out pin
  pinMode(PTTpinUHF, OUTPUT);  //PTTUHF out pin

  pinMode(C_RELAIS, OUTPUT);  // Coax Relais HF / VHF-UHF
  digitalWrite(PTTpin, LOW);  //set 'Rx mode' > high
  digitalWrite(PTTpinHF, LOW);
  digitalWrite(PTTpinVHF, LOW);
  digitalWrite(PTTpinUHF, LOW);





  //init_DAC(); // initialize analog output

 // userPTT(0);  // initialize the "RX" symbol in the screen



#ifdef WIFI
  setup_wifi();
#endif
}

//-------------------------------------------------------------------------------------
// this will be called in the baseloop every BASELOOP_TICK[ms]
void userBaseLoop() {
}
