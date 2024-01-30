/* 
IC705toPA - a_Defines.h
*/
// globals.h
#ifndef A_DeFINES_H
#define A_DeFINES_H
#include <stdint.h>
#define VERSION_STRING "IC705toPA V0_1 22/05/04"

// Global compile switches ===================================================================================

// if defined, the bit pattern of the output pins is inverted in order to compensate
// the effect of inverting HW drivers (active, i.e.uncommented by default)

#define debug 

// Speed of the Serial output messages
#define debugBdRate 115200

// some general defines ----------------------------------

#define BASELOOP_TICK 10 
#define NUM_BANDS 16   /* Number of Bands (depending on the radio) */

//-------------------------------------------------------------------------------

//LCD Config
#define screen_width  480       //placement of text etc must fit withing these boundaries.
#define screen_heigth 320

// all my known colors for ST7789 TFT (but not all used in program)
#define B_DD6USB 0x0004    //   0,   0,   4  my preferred background color !!!   now vk3pe ?
#define BLACK 0x0000       //   0,   0,   0
#define NAVY 0x000F        //   0,   0, 123
#define DARKGREEN 0x03E0   //   0, 125,   0
#define DARKCYAN 0x03EF    //   0, 125, 123
#define MAROON 0x7800      // 123,   0,   0
#define PURPLE 0x780F      // 123,   0, 123
#define OLIVE 0x7BE0       // 123, 125,   0
#define LIGHTGREY 0xC618   // 198, 195, 198
#define DARKGREY 0x7BEF    // 123, 125, 123
#define BLUE 0x001F        //   0,   0, 255
#define GREEN 0x07E0       //   0, 255,   0
#define CYAN 0x07FF        //   0, 255, 255
#define RED 0xF800         // 255,   0,   0
#define MAGENTA 0xF81F     // 255,   0, 255
#define YELLOW 0xFFE0      // 255, 255,   0
#define WHITE 0xFFFF       // 255, 255, 255
#define ORANGE 0xFD20      // 255, 165,   0
#define GREENYELLOW 0xAFE5 // 173, 255,  41
#define PINK 0xFC18        // 255, 130, 198
//*************************************************************

int touchcalibrate = 0;
int buttonX[6] = {0, 80, 160, 240, 320, 400};
int buttonY = 260;
int buttonWidth = 80;
int buttonHeight = 40;
bool buttonPressed[6] = {false, false, false,false,false,false};
// Debugging ...

#ifdef debug
  #define SET_TIME_MARKER1 G_timemarker1 = micros();
  #define EVAL_TIME_MARKER1 G_timemarker1a = micros();Serial.print("t1:  ");Serial.println(G_timemarker1a-G_timemarker1);
#endif

#ifndef debug
  #define SET_TIME_MARKER1
  #define EVAL_TIME_MARKER1
#endif

#endif //A_DeFINES_H