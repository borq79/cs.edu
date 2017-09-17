#include <Adafruit_NeoPixel.h>
#include "simon.h"

int sequenceCounter = 0;
uint8_t sequence[MAX_SEQUENCE];

const int BUTTONS[] = { BUTTON_ONE_PIN, BUTTON_TWO_PIN, BUTTON_THREE_PIN, BUTTON_FOUR_PIN };
const COLOR SIMON_COLORS[] = { PURPLE, RED, BLUE, GREEN };
const int SIMON_NOTES[] = { NOTE_D4, NOTE_G4, NOTE_C5, NOTE_A5 };

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

  restartGame();
}


void loop() 
{
  displaySequence();
  sequenceCounter++;
  
  if (readUserInput() == false) {
    restartGame();  
  } else if (sequenceCounter == MAX_SEQUENCE) {
    restartGame(); 
  } else {
    delay(1500);
  }
}

void restartGame() {
  generateSequence();
  sequenceCounter = 0;
  delay(TRANSTION_TIME);
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
  int noteToPlay = SIMON_NOTES[simonIndex];

  strip.setPixelColor(simonIndex, colorToShow.r, colorToShow.g, colorToShow.b);
  strip.show();

 // Serial.print("Note: "); Serial.println(noteToPlay);
  tone(BUZZER_PIN, noteToPlay, 500);
  delay(500);

  // Turn the light off
  strip.setPixelColor(simonIndex, 0x00, 0x00, 0x00);
  strip.show();
}

bool readUserInput() {
  int sequenceRemembered = true;
  
  int numberOfUserButtonPresses = 0;
  int expectedUserButtonPresses = sequenceCounter;

  int startTime = millis();
  int elapsedTime = 0;

Serial.print("N: "); Serial.println(numberOfUserButtonPresses);
Serial.print("E: "); Serial.println(expectedUserButtonPresses);
  while (numberOfUserButtonPresses < expectedUserButtonPresses && elapsedTime < USER_INPUT_TIMEOUT) {
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

    elapsedTime = millis() - startTime;
  }

  if (elapsedTime >= USER_INPUT_TIMEOUT) {
    sequenceRemembered = false;
  }

Serial.print("R: "); Serial.println(sequenceRemembered);
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

