#include "encoder.h"

void setup_encoders(){
pinMode(GREEN_PIN_A, INPUT_PULLUP);
pinMode(GREEN_PIN_B, INPUT_PULLUP);
pinMode(WHITE_PIN_A, INPUT_PULLUP);
pinMode(WHITE_PIN_B, INPUT_PULLUP);
pinMode(RED_PIN_A, INPUT_PULLUP);
pinMode(RED_PIN_B, INPUT_PULLUP);
pinMode(ORANGE_PIN_A, INPUT_PULLUP);
pinMode(ORANGE_PIN_B, INPUT_PULLUP);
pinMode(YELLOW_PIN_A, INPUT_PULLUP);
pinMode(YELLOW_PIN_B, INPUT_PULLUP);
pinMode(BLUE_PIN_A, INPUT_PULLUP);
pinMode(BLUE_PIN_B, INPUT_PULLUP);

attachInterrupt(GREEN_PIN_A,GreenA,RISING);
attachInterrupt(GREEN_PIN_B,GreenB,RISING);
attachInterrupt(WHITE_PIN_A,WhiteA,RISING);
attachInterrupt(WHITE_PIN_B,WhiteB,RISING);
attachInterrupt(RED_PIN_A,RedA,RISING);
attachInterrupt(RED_PIN_B,RedB,RISING);
attachInterrupt(ORANGE_PIN_A,OrangeA,RISING);
attachInterrupt(ORANGE_PIN_B,OrangeB,RISING);
attachInterrupt(YELLOW_PIN_A,YellowA,RISING);
attachInterrupt(YELLOW_PIN_B,YellowB,RISING);
attachInterrupt(BLUE_PIN_A,BlueA,RISING);
attachInterrupt(BLUE_PIN_B,BlueB,RISING);
}



void GreenA(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(GREEN_PIN_A) == 1 && digitalRead(GREEN_PIN_B) == 1  && aFlag) {
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(GREEN_PIN_A) == 1 && digitalRead(GREEN_PIN_B) == 0) bFlag = 1;

    //if(first_turn){
      //  if(encoderPos<-3) {turn = 70; encoderPos=0; Serial.println((char)turn); }

    //}else {
        if(encoderPos == -6) {turn = 70; encoderPos=0; senseOfNewTurn = true; }
   // }
    sei(); //restart interrupts
}

void GreenB(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(GREEN_PIN_A) == 1 && digitalRead(GREEN_PIN_B) == 1  && bFlag) {
    encoderPos ++; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(GREEN_PIN_A) == 0 && digitalRead(GREEN_PIN_B) == 1) aFlag = 1;

    //if(first_turn){
      //  if(encoderPos>3) {turn = 102; encoderPos=0; Serial.println((char)turn); }

    //}else {
        if(encoderPos == 6) {turn = 102; encoderPos=0; senseOfNewTurn = true; }
   // }
    sei(); //restart interrupts
}

void WhiteA(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(WHITE_PIN_A) == 1 && digitalRead(WHITE_PIN_B) == 1  && aFlag) {
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(WHITE_PIN_A) == 1 && digitalRead(WHITE_PIN_B) == 0) bFlag = 1;
    if(encoderPos == -6) {turn = 85; encoderPos=0; senseOfNewTurn = true; }
    sei(); //restart interrupts
}

void WhiteB(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(WHITE_PIN_A) == 1 && digitalRead(WHITE_PIN_B) == 1  && bFlag) {
    encoderPos ++; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(WHITE_PIN_A) == 0 && digitalRead(WHITE_PIN_B) == 1) aFlag = 1;
    if(encoderPos == 6) {turn = 117; encoderPos=0; senseOfNewTurn = true; }
    sei(); //restart interrupts
}

void RedA(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(RED_PIN_A) == 1 && digitalRead(RED_PIN_B) == 1  && aFlag) {
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(RED_PIN_A) == 1 && digitalRead(RED_PIN_B) == 0) bFlag = 1;
    if(encoderPos == -6) {turn = 82; encoderPos=0; senseOfNewTurn = true; }
    sei(); //restart interrupts
}

void RedB(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(RED_PIN_A) == 1 && digitalRead(RED_PIN_B) == 1  && bFlag) {
    encoderPos ++; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(RED_PIN_A) == 0 && digitalRead(RED_PIN_B) == 1) aFlag = 1;
    if(encoderPos == 6) {turn = 114; encoderPos=0; senseOfNewTurn = true; }
    sei(); //restart interrupts
}

void OrangeA(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(ORANGE_PIN_A) == 1 && digitalRead(ORANGE_PIN_B) == 1  && aFlag) {
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(ORANGE_PIN_A) == 1 && digitalRead(ORANGE_PIN_B) == 0) bFlag = 1;
    if(encoderPos == -6) {turn = 76; encoderPos=0; senseOfNewTurn = true; }
    sei(); //restart interrupts
}

void OrangeB(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(ORANGE_PIN_A) == 1 && digitalRead(ORANGE_PIN_B) == 1  && bFlag) {
    encoderPos ++; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(ORANGE_PIN_A) == 0 && digitalRead(ORANGE_PIN_B) == 1) aFlag = 1;
    if(encoderPos == 6) {turn = 108; encoderPos=0; senseOfNewTurn = true; }
    sei(); //restart interrupts
}

void YellowA(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(YELLOW_PIN_A) == 1 && digitalRead(YELLOW_PIN_B) == 1  && aFlag) {
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(YELLOW_PIN_A) == 1 && digitalRead(YELLOW_PIN_B) == 0) bFlag = 1;
    if(encoderPos == -6) {turn = 68; encoderPos=0; senseOfNewTurn = true; }
    sei(); //restart interrupts
}

void YellowB(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(YELLOW_PIN_A) == 1 && digitalRead(YELLOW_PIN_B) == 1  && bFlag) {
    encoderPos ++; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(YELLOW_PIN_A) == 0 && digitalRead(YELLOW_PIN_B) == 1) aFlag = 1;
    if(encoderPos == 6) {turn = 100; encoderPos=0; senseOfNewTurn = true; }
    sei(); //restart interrupts
}

void BlueA(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(BLUE_PIN_A) == 1 && digitalRead(BLUE_PIN_B) == 1  && aFlag) {
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(BLUE_PIN_A) == 1 && digitalRead(BLUE_PIN_B) == 0) bFlag = 1;
    if(encoderPos == -6) {turn = 66; encoderPos=0; senseOfNewTurn = true; }
    sei(); //restart interrupts
}

void BlueB(){
    cli(); //stop interrupts happening before we read pin values
    if(digitalRead(BLUE_PIN_A) == 1 && digitalRead(BLUE_PIN_B) == 1  && bFlag) {
    encoderPos ++; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
    }
    else if (digitalRead(BLUE_PIN_A) == 0 && digitalRead(BLUE_PIN_B) == 1) aFlag = 1;
    if(encoderPos == 6) {turn = 98; encoderPos=0; senseOfNewTurn = true; }
    sei(); //restart interrupts
}
