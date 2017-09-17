#include <Adafruit_NeoPixel.h>
#include "simon.h"


const int BUTTONS[] = { BUTTON_ONE_PIN, BUTTON_TWO_PIN, BUTTON_THREE_PIN, BUTTON_FOUR_PIN };

Adafruit_NeoPixel strip = Adafruit_NeoPixel(SIZE_OF_NEO_PIXEL_BAR, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  // Initialize the new pixel strip library
  strip.begin();

  // Initialize all pixels to 'off'
  strip.show();

  // So we can debug ... if needed
  Serial.begin(9600);

  // Initialize the buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // The four buttons need pullup resistors so we can tell when the button is pressed
  // Otherwise it falls back to high impedance 
  for(int i =0 ; i < NUM_BUTTONS; i++) {
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }
}

void loop() 
{
}


