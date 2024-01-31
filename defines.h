#ifndef defines_h
#define defines_h

#define VERSION_STRING "IC705toPA V0_1 22/05/04"

// Global compile switches ===================================================================================

// if defined debug messages on the serial line will be generated
// (active, i.e.uncommented by default)
#define debug
// Speed of the Serial output messages
#define debugBdRate 115200
// Timers
#define BASELOOP_TICK 10
#define NUM_BANDS 16 /* Number of Bands (depending on the radio) */
// some general defines ----------------------------------



//LCD Config
#define useTouch
#define LCD
#define screen_width 480  //placement of text etc must fit withing these boundaries.
#define screen_heigth 320

// all my known colors for ST7789 TFT (but not all used in program)
#define B_DD6USB 0x0004     //   0,   0,   4  my preferred background color !!!   now vk3pe ?
#define BLACK 0x0000        //   0,   0,   0
#define NAVY 0x000F         //   0,   0, 123
#define DARKGREEN 0x03E0    //   0, 125,   0
#define DARKCYAN 0x03EF     //   0, 125, 123
#define MAROON 0x7800       // 123,   0,   0
#define PURPLE 0x780F       // 123,   0, 123
#define OLIVE 0x7BE0        // 123, 125,   0
#define LIGHTGREY 0xC618    // 198, 195, 198
#define DARKGREY 0x7BEF     // 123, 125, 123
#define BLUE 0x001F         //   0,   0, 255
#define GREEN 0x07E0        //   0, 255,   0
#define CYAN 0x07FF         //   0, 255, 255
#define RED 0xF800          // 255,   0,   0
#define MAGENTA 0xF81F      // 255,   0, 255
#define YELLOW 0xFFE0       // 255, 255,   0
#define WHITE 0xFFFF        // 255, 255, 255
#define ORANGE 0xFD20       // 255, 165,   0
#define GREENYELLOW 0xAFE5  // 173, 255,  41
#define PINK 0xFC18         // 255, 130, 198
//*************************************************************


#endif  // #ifndef defines_h
