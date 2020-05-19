
#include "ledrubikcube3.h"
//#include <FastLED.h>
#include <algorithm>
#include <array>
using namespace std;
#define FASTLED_INTERRUPT_RETRY_COUNT 1


edge tmp_edge[4];
edge rubik_matrix[6][4];   // 3D array. 6 side of the cube 4 edge on 1 side, 3 sticker in 1 edge
uint32_t array_index[] = {0, 1, 2, 3};
color* pRubikArray = &rubik_matrix[0][0][0];
color beginnnig_colors[6] = {green, white, red, orange, yellow, blue} ;


uint32_t circularArr(uint32_t arr[], uint32_t index)  {
  int8_t hossz = 4;
  int8_t i = abs(index);
  uint32_t retval = index > -1 ? arr[i % hossz] :  arr[hossz - (i % hossz)] ;
  return hossz;
}

void fastLedInit() {

  FastLED.addLeds<WS2812B , DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
}

void cube_reset() {
  //-----------------fill the matrix with the begining color scheme of the rubik cube-----------------
  FastLED.clear();

  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      for (int h = 0; h < 3; h++)
      {
        rubik_matrix[i][j][h] = beginnnig_colors[i];
      }
    }


  }


}

void upload_cube () {

  int k = 0;
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      for (int h = 0; h < 3; h++)
      {
        leds[k] = rubik_matrix[i][j][h] ;
        k++;
      }
      k--;
    }
    leds[k] = purple ; // only in developer version. the middle of the side never move so its more transparent when we colored them with an other color
    k++;
  }



  FastLED.show();

}


int circular(int index) {
  if (index == -1) index = 3;
  if (index == 4 ) index = 0;
  return index;
}



void copyArray(uint32_t* dst, uint32_t* src,  int len) {
  for (int i = 0; i < len; i++) {
    *dst++ = *src++;
  }
}
/* on a side 1 edge change from destination edge to source edge. the destination edge is a number of the edge in the rubik matrix
  the source is any type of edge even a unique one. example edge edge_turkiz={turkiz,turkiz,turkiz};
  change_edge(3,edge_turkiz,5); change the three color of the 3. edge on the 5. side to turkiz*/
void change_edge(int side, int edge_dst, edge edge_src)
{
  copyArray(rubik_matrix[side][edge_dst], edge_src, 3);
  rubik_matrix[side][circular(edge_dst + 1)][0] = edge_src[2];
  rubik_matrix[side][circular(edge_dst - 1)][2] = edge_src[0];


}


void turn_side (int side, int directionn) {
  int x[4] = {0, 1, 5, 4}; // az oldalak sorrandje a bizonyos tengelyeken , order of sides number in the three axis
  int y[4] = {0, 2, 5, 3};
  int z5[4] = {1, 2, 4, 3};
  int z0[4] = {4, 3, 1, 2};
  color tmp_color;
  Serial.println("turn sideon beül vagyunk");
  // change 4 edge on the same side
  for (int i = 0; i < 4; i++) {
    copyArray(tmp_edge[i], rubik_matrix[side][i],  4);   //elõzõ állapot elmentése, save the previous state
  }

  //saját oldalon a 4 él körberogatása, change 4 edge on the same side
  if (directionn == 1) {
    for (int i = 0; i < 4; i++) {
      change_edge(side, i, tmp_edge[circular(i + 1)]);

    }
  }
  //saját oldalon a 4 él körberogatása ellenkező irányba, change 4 edge on the same side reverse direction
  if (directionn == 0) {
    for (int i = 3; i > -1; i--) {
      change_edge(side, i, tmp_edge[circular(i - 1)]);
    }
  }
  //élek forgatása az oldalra merőleges tengely körüli 4 olalon, change 4 edges on the perpendicular axis
  switch (side) {

    //0ás oldal esetén (zöld), 4,3,2,1 oldalakon kell 1 élt kicserélni (z0)
    case 0: for (int i = 0; i < 4; i++) {
        copyArray(tmp_edge[i], rubik_matrix[z0[i]][i], 4);
      }

      if (directionn == 0) {
        for (int i = 0; i < 4; i++) {
          change_edge(z0[i], i, tmp_edge[circular(i - 1)]);
        }
      }
      if (directionn == 1 ) {
        for (int i = 0; i < 4; i++) {
          change_edge(z0[i], i, tmp_edge[circular(i + 1)]);
        }
      }
      break;
    case 1: for (int i = 0; i < 4; i++) {
        copyArray(tmp_edge[i], rubik_matrix[y[i]][0], 4);
        if (y[i] == 5) {
          reverse(tmp_edge[i], tmp_edge[i] + 3); /*a reverse függvényre azért van szükségünk, mert az 5-ös oldal minden éle tükrözve van matematikai okokból.
          we need reverse function, because on the side 5, every edge is inversed, for mathematical reasons*/
        }
      }
      if (directionn == 1) {
        for (int i = 0; i < 4; i++) {
          if (y[i] == 5) {
            reverse(tmp_edge[circular(i - 1)], tmp_edge[circular(i - 1)] + 3);
          }
          change_edge(y[i], 0, tmp_edge[circular(i - 1)]);
        }
      }
      if (directionn == 0) {
        for (int i = 0; i < 4; i++) {
          if (y[i] == 5) {
            reverse(tmp_edge[circular(i + 1)], tmp_edge[circular(i + 1)] + 3);
          }
          change_edge(y[i], 0, tmp_edge[circular(i + 1)]);
        }
      }
      break;
    case 2: for (int i = 0; i < 4; i++) {
        copyArray(tmp_edge[i], rubik_matrix[x[i]][1], 4);
        if (x[i] == 5) {
          reverse(tmp_edge[i], tmp_edge[i] + 3);
        }
      }
      if (directionn == 0) {
        for (int i = 0; i < 4; i++) {
          if (x[i] == 5) {
            reverse(tmp_edge[circular(i - 1)], tmp_edge[circular(i - 1)] + 3);
          }
          change_edge(x[i], 1, tmp_edge[circular(i - 1)]);
        }
      }
      if (directionn == 1) {
        for (int i = 0; i < 4; i++) {
          if (x[i] == 5) {
            reverse(tmp_edge[circular(i + 1)], tmp_edge[circular(i + 1)] + 3);
          }
          change_edge(x[i], 1, tmp_edge[circular(i + 1)]);
        }
      }
      break;

    case 3: for (int i = 0; i < 4; i++) {
        copyArray(tmp_edge[i], rubik_matrix[x[i]][3], 4);
        if (x[i] == 5) {
          reverse(tmp_edge[i], tmp_edge[i] + 3);
        }
      }
      if (directionn == 1) {
        for (int i = 0; i < 4; i++) {
          if (x[i] == 5) {
            reverse(tmp_edge[circular(i - 1)], tmp_edge[circular(i - 1)] + 3);
          }
          change_edge(x[i], 3, tmp_edge[circular(i - 1)]);
        }
      }
      if (directionn == 0) {
        for (int i = 0; i < 4; i++) {
          if (x[i] == 5) {
            reverse(tmp_edge[circular(i + 1)], tmp_edge[circular(i + 1)] + 3);
          }
          change_edge(x[i], 3, tmp_edge[circular(i + 1)]);
        }
      }
      break;
    case 4: for (int i = 0; i < 4; i++) {
        copyArray(tmp_edge[i], rubik_matrix[y[i]][2], 4);
        if (y[i] == 5) {
          reverse(tmp_edge[i], tmp_edge[i] + 3);

        }
      }
      if (directionn == 0) {
        for (int i = 0; i < 4; i++) {
          if (y[i] == 5) {
            reverse(tmp_edge[circular(i - 1)], tmp_edge[circular(i - 1)] + 3);
          }
          change_edge(y[i], 2, tmp_edge[circular(i - 1)]);
        }
      }
      if (directionn == 1) {
        for (int i = 0; i < 4; i++) {
          if (y[i] == 5) {
            reverse(tmp_edge[circular(i + 1)], tmp_edge[circular(i + 1)] + 3);
          }
          change_edge(y[i], 2, tmp_edge[circular(i + 1)]);
        }
      }
      break;
    case 5: for (int i = 0; i < 4; i++) {
        copyArray(tmp_edge[i], rubik_matrix[z5[i]][i], 4);
      }
      if (directionn == 0 ) {
        for (int i = 0; i < 4; i++) {
          change_edge(z5[i], i, tmp_edge[circular(i - 1)]);
        }
      }
      if (directionn == 1 ) {
        for (int i = 0; i < 4; i++) {
          change_edge(z5[i], i, tmp_edge[circular(i + 1)]);
        }
      }
      break;

  }
}

void rubikArray_saveEEPROM() {
  pRubikArray = &rubik_matrix[0][0][0];
  for (uint8_t i = 0; i < EEPROM_SIZE; i++) {
    switch (*pRubikArray) {
      case 32768 :
        EEPROM.write(i, 'g');
        EEPROM.commit();
        pRubikArray++;
        break;
      case 16777215 :
        EEPROM.write(i, 'w');
        EEPROM.commit();
        pRubikArray++;
        break;
      case 16711680 :
        EEPROM.write(i, 'r');
        EEPROM.commit();
        pRubikArray++;
        break;
      case 9849600 :
        EEPROM.write(i, 'o');
        EEPROM.commit();
        pRubikArray++;
        break;
      case 16776960 :
        EEPROM.write(i, 'y');
        EEPROM.commit();
        pRubikArray++;
        break;
      case 255 :
        EEPROM.write(i, 'b');
        EEPROM.commit();
        pRubikArray++;
        break;


    }
  }
}

void printEEPROM() {
  for (uint8_t i = 0; i < EEPROM_SIZE; i++) {
    Serial.println((char)EEPROM.read(i));
  }
}
void loadCube_fromEEPROM() {
  pRubikArray = &rubik_matrix[0][0][0];
  for (uint8_t i = 0; i < EEPROM_SIZE; i++) {
    switch ((char)EEPROM.read(i)) {
      case 'g' :
        *pRubikArray = green;
        pRubikArray++;
        break;
      case 'w' :
        *pRubikArray = white;
        pRubikArray++;
        break;
      case 'r' :
        *pRubikArray = red;
        pRubikArray++;
        break;
      case 'o' :
        *pRubikArray = orange;
        pRubikArray++;
        break;
      case 'y' :
        *pRubikArray = yellow;
        pRubikArray++;
        break;
      case 'b' :
        *pRubikArray = blue;
        pRubikArray++;
        break;


    }
  }

}
