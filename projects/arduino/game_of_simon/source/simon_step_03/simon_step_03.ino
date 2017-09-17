#include <Adafruit_NeoPixel.h>
#include "simon.h"

int sequenceCounter = 0;
uint8_t sequence[MAX_SEQUENCE];

const int BUTTONS[] = { BUTTON_ONE_PIN, BUTTON_TWO_PIN, BUTTON_THREE_PIN, BUTTON_FOUR_PIN };
const COLOR SIMON_COLORS[] = { PURPLE, RED, BLUE, GREEN };

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
  generateSequence();
  sequenceCounter = MAX_SEQUENCE - 1;
  displaySequence(); 
  delay(5000);
}


void generateSequence() {
  randomSeed(analogRead(0));

  Serial.println("Sequence ...");
  for(int i = 0; i < MAX_SEQUENCE; i++) {
    uint8_t randNumber = (uint8_t)random(0, 4);
    sequence[i] = randNumber;
    Serial.print(sequence[i]); Serial.print(",");
  }
  Serial.println("");

  sequenceCounter = 0;
}

void displaySequence() {
  for(int i = 0; i <= sequenceCounter; i++) {
    uint8_t simonIndex = sequence[i];

    Serial.print("Simon Index: "); Serial.println(simonIndex);

    displaySingleSequenceItem(simonIndex);
    delay(DISPLAY_PAUSE);
  }
}


void displaySingleSequenceItem(int simonIndex) {
  COLOR colorToShow = SIMON_COLORS[simonIndex]; 

  strip.setPixelColor(simonIndex, colorToShow.r, colorToShow.g, colorToShow.b);
  strip.show();

  delay(500);

  // Turn the light off
  strip.setPixelColor(simonIndex, 0x00, 0x00, 0x00);
  strip.show();
}

bool readUserInput() {
  int sequenceRemembered = true;
  
  int numberOfUserButtonPresses = 0;
  int expectedUserButtonPresses = sequenceCounter;
  
  while (numberOfUserButtonPresses < expectedUserButtonPresses) {
    int simonIndex = -1;
    
    int buttonPressed = getButtonUserPressed();

    if (buttonPressed >= 0 && buttonPressed < NUM_BUTTONS) {
      displaySingleSequenceItem(buttonPressed);
      if (sequence[numberOfUserButtonPresses] != buttonPressed) {
        sequenceRemembered = false;
        break;
      }

      numberOfUserButtonPresses++;
    }
  }

  return sequenceRemembered;
}

int getButtonUserPressed() {
  int buttonPressed = -1;
  
  for(int i = 0; i < NUM_BUTTONS; i++) {
    int buttonValue = digitalRead(BUTTONS[i]);

    if (buttonValue == LOW) {
      buttonPressed = i;
      break;
    }
  }

  return buttonPressed;
}

void lightUpFromButtonPress(int simonIndex) {
  strip.setPixelColor(simonIndex, 0xFF, 0x00, 0xFF);
  strip.show();

  delay(500);

  // Turn the light off
  strip.setPixelColor(simonIndex, 0x00, 0x00, 0x00);
  strip.show();
}
