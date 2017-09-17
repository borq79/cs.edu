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
    gameOver();
    restartGame();  
  } else if (sequenceCounter == MAX_SEQUENCE) {
    gameWon();
    restartGame(); 
  } else {
    delay(1500);
  }
}

void restartGame() {
  generateSequence();
  sequenceCounter = 0;
  playMusic(theme);
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

void gameOver() {
  playMusic(gameover);
  delay(TRANSTION_TIME);
}

void gameWon() {
  playMusic(flagpole);
  delay(TRANSTION_TIME);
}

bool readUserInput() {
  int sequenceRemembered = true;
  
  int numberOfUserButtonPresses = 0;
  int expectedUserButtonPresses = sequenceCounter;

  int startTime = millis();
  int elapsedTime = 0;

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

// Borrowed these note arrays from https://github.com/tsukisan/Arduino/tree/master/WiiClassicSoundboard
void playMusic(const int *song) {
  if (song != NULL) {
    int numberOfNotes = song[0];
    
    for(int i = 1; i < (numberOfNotes * 2); i += 2) {
      int note =  song[i];
      int duration = song[i + 1];

      int durationMs = 1000 / duration;

      showLightBasedOnNote(note);
      
      tone(BUZZER_PIN, note, durationMs);
      delay(durationMs * 1.30);
      noTone(BUZZER_PIN); 
    }
  }

  turnOffEntirePixelStrip();
}

void turnOffEntirePixelStrip() {
    for(int i = 0; i < SIZE_OF_NEO_PIXEL_BAR; i++) {
    strip.setPixelColor(i, 0x00, 0x00, 0x00);
    strip.show();
  }
}

void showLightBasedOnNote(int note) {
  int brightness = note / 8;
  for(int i = 0; i < SIZE_OF_NEO_PIXEL_BAR; i++) {
    uint8_t randNumber = (uint8_t)random(0, 128);
    brightness = brightness * randNumber;
    strip.setPixelColor(i, brightness);
  }
  strip.show();
}

