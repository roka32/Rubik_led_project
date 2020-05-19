#ifndef LEDRUBIKCUBE3_H_INCLUDED
#define LEDRUBIKCUBE3_H_INCLUDED

#include <Arduino.h>
#include <FastLED.h>
#include <EEPROM.h>

/*  char    |direction  |ascii  |side   |color
  ------------------------------------------------
    F       |0          |70     |0      |green
    f       |1          |102    |0      |green
    U       |0          |85     |1      |white
    u       |1          |117    |1      |white
    R       |0          |82     |2      |red
    r       |1          |114    |2      |red
    L       |0          |76     |3      |orange
    l       |1          |108    |3      |orange
    D       |0          |68     |4      |yellow
    d       |1          |100    |4      |yellow
    B       |0          |66     |5      |blue
    b       |1          |98     |5      |blue

*/

#define NUM_LEDS 54
#define DATA_PIN 12
#define BRIGHTNESS 7
#define COLOR_ORDER GRB
static CRGB  leds[NUM_LEDS];

#define EEPROM_SIZE 72




typedef  uint32_t color;
typedef color edge[3];
/*void print_valami();
  static color blue =11;
  static color purple =5;
  static color black =8;
  const int alma = 3;
  static edge ele = {blue,purple,black};*/

static color green = CRGB::Green; //green
static color white = CRGB::White; //white
static color red = CRGB::Red; //red
static color orange = 9849600; //orange
static color yellow = CRGB::Yellow; //yellow
static color blue = CRGB::Blue; //blue
static color black = CRGB::Black; //blue
static color purple = CRGB::Purple; //purple


void cube_reset();
void loadCube_fromEEPROM();
void upload_cube (); // refresh the full cube colors, on the leds with the rubik_matrix
void fastLedInit();
void rubikArray_saveEEPROM();
void printEEPROM();


void turn_side (int side, int directionn); // one turn on the cube change 8 edge
void copyArray(uint32_t* src, uint32_t* dst, uint32_t len);
uint32_t circularArr(uint32_t arr[], uint32_t index);
uint8_t osszeadas(uint8_t a, uint8_t b);
void copyArray(uint32_t* dst, uint32_t* src,  int len);
void change_edge(int side, int edge_dst, edge edge_src);
int circular(int index);



#endif // LEDRUBIKCUBE_H_INCLUDED
