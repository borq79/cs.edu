/**
 * Electronic Matrix — I Love You (Project E.M.I.L.Y)
 * by Waren Gonzaga
 * Version: 1.0.6
 *
 * A velentine project for saying I Love You!
 * Check this project on Instructables.com
 * Link: https://www.instructables.com/id/Electronic-Matrix-I-Love-You
 *
 * Contribute to improve this project on gist!
 * Link: https://gist.github.com/WarenGonzaga/e94f2412187e7b72cd4e33dd1bfff337
 *
 * This project is made possible by the following.
 * Hive Electronics
 * ElexHub
 * JAG Electronics
 * Connected Cities
 * EasyElectronics
 * E-Gics
 *
 * Having Trouble?
 * Please email me here: warengonzaga.dev@gmail.com
 * or Just send a quick message to my facebook page.
 * Link: https://facebook.com/warengonzagaofficialpage
 * Follow me on twitter: @waren_gonzaga
 * Visit my website: https://warengonzaga.com/
 *
 * Licensed under Creative Commons BY-NC-SA
 * Copyright © 2017 Waren Gonzaga
 */

#include <Arduino.h>


// Part # KWM-30881CVB
// KWM Column: 1   2   3   4   5   6   7   8
// KWM Pin:    13  3   4   10  6   11  15  16
// Nano Pin:   A5  D7  D6  D4  D5  A3  A6  A7

// KWM Row:    1   2   3   4   5   6   7   8
// KWM Pin:    9   14  8   12  1   7   2   5
// Nano Pin:   D4  D2  A1  D3  D9  A0  D8  D10


// 2-dimensional array of column pin numbers:
const int col[8] = {
  A5, 7, 6, 4, 5, A3, A6, A7
};

// 2-dimensional array of row pin numbers:
const int row[8] = {
  4, 2, A1, 3, 9, A0, 8, 10
};


typedef bool charMapType[8][8];

int animationSpeed = 100; // the animation speed

/* Don't modify below */

/**
 *  The LED Swithes based on Binary Format.
 */

// A
const charMapType charBlank = {
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}
};

// B
const charMapType animation0 = {
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}
};

// C
const charMapType animation1 = {
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}
};

// D
const charMapType animation2 = {
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}
};

// E
const charMapType animation3 = {
  {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}
};

// F
const charMapType animation4 = {
  {0, 0, 1, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 1, 0, 0}
};

// G
const charMapType animation5 = {
  {0, 1, 1, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 1, 1, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 1, 1, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 1, 1, 0}
};

//==== Heart Animation ===
// H
const charMapType heart0 = {
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}
};

// I
const charMapType heart1 = {
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}
};

// J
const charMapType heart2 = {
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 1, 1, 0}, {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}, {0, 1, 1, 1, 1, 1, 1, 0}, {0, 0, 1, 1, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}
};

// K
const charMapType heart3 = {
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 1, 1, 0}, {1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 0}, {0, 0, 1, 1, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}
};

// L
const charMapType heart4 = {
  {0, 1, 1, 0, 0, 1, 1, 0}, {1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1}, {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},{0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}
};

//==== You Animation ===
// M
const charMapType animation6 = {
  {0, 1, 1, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}
};

// N
const charMapType animation7 = {
  {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0, 0, 0}
};

// O
const charMapType animation8 = {
  {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 1, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 1, 0, 0}
};

// P
const charMapType animation9 = {
  {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 1, 1, 0}
};

// Q
const charMapType animation10 = {
  {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 1, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 1, 1, 0}
};

// character map for the anmation
const charMapType *charMap[17] = {
  &charBlank,   //a
  &animation0,  //b
  &animation1,  //c
  &animation2,  //d
  &animation3,  //e
  &animation4,  //f
  &animation5,  //g
  &heart0,      //h
  &heart1,      //i
  &heart2,      //j
  &heart3,      //k
  &heart4,      //l
  &animation6,  //m
  &animation7,  //n
  &animation8,  //o
  &animation9,  //p
  &animation10  //q
};

// animation sequence
char str[] = "ABCDEFGGGGGGGGGGGGGGGGGGGGFEDCBAAAAAAAAAAHHHHHHIIIIIJJJJJKKKKKLLLLLKKKKKJJJJJKKKKKLLLLLKKKKKJJJJJIIIIIHHHHHAAAAAAAAAAMNOPQQQQQQQQQQQQQQQQQQQQPONMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

// 2-dimensional array of pixels:
int pixels[8][8];
int count = 1000;
int strLen = sizeof(str);
int ptrChar = 0;

void setup() {
  // initialize the I/O pins as outputs
  // iterate over the pins:
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    // initialize the output pins:
    pinMode(col[thisPin], OUTPUT);
    pinMode(row[thisPin], OUTPUT);
    // take the col pins (i.e. the cathodes) high to ensure that
    // the LEDS are off:
    digitalWrite(col[thisPin], HIGH);
  }

  setupChar();

}

void loop() {

  // animating the screen:
  refreshScreen();

  if(count-- == 0){
    count = animationSpeed;
    setupChar();
  }

}

void setupChar(){
  char c = str[ptrChar];
  int offset = c - 'A';

  const charMapType *cMap = charMap[offset];

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      bool v = (*cMap)[x][y];

      if(v){
        pixels[x][y] = LOW;
      }else{
        pixels[x][y] = HIGH;
      }
    }
  }

  ptrChar++;
  if(ptrChar>=strLen-1){
    ptrChar = 0;
  }

}

void refreshScreen() {
  // iterate over the rows (anodes):
  for (int thisRow = 0; thisRow < 8; thisRow++) {
    // take the row pin (cathod) high:
    digitalWrite(row[thisRow], LOW);
    // iterate over the columns (anodes):
    for (int thisCol = 0; thisCol < 8; thisCol++) {
      // get the state of the current pixel;
      int thisPixel = pixels[thisRow][thisCol];
      // when the row is LOW and the column is HIGH,
      // the LED where they meet turns on:
      digitalWrite(col[thisCol], thisPixel);
      // turn the pixel off:
      if (thisPixel == LOW) {
        digitalWrite(col[thisCol], LOW);
      }
    }
    // take the row pin low to turn off the whole row:
    digitalWrite(row[thisRow], HIGH);
  }
}
