#ifndef ENCODER_H_INCLUDED
#define ENCODER_H_INCLUDED

#include "Arduino.h"


/*  char    |direction  |ascii  |side   |color
 *------------------------------------------------
 *  F       |0          |70     |0      |green
 *  f       |1          |102    |0      |green
 *  U       |0          |85     |1      |white
 *  u       |1          |117    |1      |white
 *  R       |0          |82     |2      |red
 *  r       |1          |114    |2      |red
 *  L       |0          |76     |3      |orange
 *  l       |1          |108    |3      |orange
 *  D       |0          |68     |4      |yellow
 *  d       |1          |100    |4      |yellow
 *  B       |0          |66     |5      |blue
 *  b       |1          |98     |5      |blue
 *
 */

#define GREEN_PIN_A 32
#define GREEN_PIN_B 33
#define WHITE_PIN_A 25
#define WHITE_PIN_B 26
#define RED_PIN_A 27    //14
#define RED_PIN_B 12
#define ORANGE_PIN_A 13
#define ORANGE_PIN_B 15
#define YELLOW_PIN_A 16
#define YELLOW_PIN_B 17
#define BLUE_PIN_A 18
#define BLUE_PIN_B 19

static volatile byte aFlag = 0;
static volatile byte bFlag = 0;
extern volatile   int8_t encoderPos ;
extern volatile   int8_t oldEncPos ;
extern volatile uint8_t turn;
extern volatile bool senseOfNewTurn ;


void GreenA();
void GreenB();
void WhiteA();
void WhiteB();
void RedA();
void RedB();
void OrangeA();
void OrangeB();
void YellowA();
void YellowB();
void BlueA();
void BlueB();
void setup_encoders();

#endif // ENCODER_H_INCLUDED
