#include <Adafruit_NeoPixel.h>

// Pin that is attached to the data line of the NeoPixel Array
#define PIN 12

// Number of Pixels in the Array
#define NUMBER_PIXELS 60
#define MAX_CURRENT 1800
#define CURRENT_PER_PIXEL 60


// Knob Control
#define KNOB_PWR  11
#define KNOB_DIAL A0
#define KNOB_READ_DELAY 10
#define KNOB_DELTA_THRESHOLD 20
#define KNOB_MAX 1024
#define KNOB_MIN 0
static float KNOB_RANGE = KNOB_MAX - KNOB_MIN;
static uint16_t KNOB_CONVERSION = round(KNOB_RANGE / 255);


// Button and ISR Configuration
#define DEBOUNCE_TIME 300
#define BUTTON_SINK   3

// How Big the Chase Mode Window is
#define CHASE_WINDOW 4

struct COLOR {
  byte r;
  byte g;
  byte b;
};

#define RED    {0x7F, 0x00, 0x00}
#define ORANGE {0x7F, 0x52, 0x00}
#define YELLOW {0x79, 0x7A, 0x05}
#define GREEN  {0x00, 0x7F, 0x00}
#define BLUE   {0x00, 0x00, 0x7F}
#define PURPLE {0x7E, 0x00, 0x7E}
#define INDIGO {0x20, 0x05, 0x7A}

#define NUM_COLORS (sizeof(colorArray) / sizeof(COLOR))
COLOR colorArray[] = { RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, PURPLE };



// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

enum LIGHT_MODE {
  LIGHT_MODE_START = 0,

  LIGHT_MODE_WIPE = LIGHT_MODE_START,
  LIGHT_MODE_ALL_RED,
  LIGHT_MODE_ALL_GREEN,
  LIGHT_MODE_ALL_BLUE,
  LIGHT_MODE_ALL_PURPLE,
  LIGHT_MODE_SOLID_RAINBOW,
  LIGHT_MODE_CHASE,

  LIGHT_MODE_END = LIGHT_MODE_CHASE
};

volatile byte     lightMode       = LIGHT_MODE_START;
volatile int      lastChange      = 0;
volatile boolean  buttonPressedInterruptFired  = false;
int16_t           lastKnobValue   = 0;
uint16_t          speedValue      = 0;
uint16_t          brightnessValue = 0;

// What direction is the light going
int directionModifier = 1;
uint16_t pixelCounter = 0;
uint8_t colorIndex = 0;

uint16_t interruptsFired = 0;

// Called when the button is pressed
void buttonISR() {
  if ((millis() - lastChange) > DEBOUNCE_TIME) {
    buttonPressedInterruptFired = true;
    lastChange = millis();
    if (lightMode == LIGHT_MODE_END) {
      lightMode = LIGHT_MODE_START;
    } else {
      lightMode++;
    }
    interruptsFired++;
    //detachInterrupt(digitalPinToInterrupt(BUTTON_SINK));
  }
}

void setup() {
//  Serial.begin(9600);
//  Serial.println("Starting light box ...");
  randomSeed(analogRead(1));

  // Configure the pins
  pinMode(KNOB_PWR,  OUTPUT);
  pinMode(KNOB_DIAL, INPUT);
  digitalWrite(KNOB_PWR, HIGH);
  pinMode(BUTTON_SINK, INPUT_PULLUP);

  buttonPressedInterruptFired = false;
  attachInterrupt(digitalPinToInterrupt(BUTTON_SINK), buttonISR, FALLING);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {
  boolean knobTurned = didTheKnobGetTurned();
  boolean buttonPushed = buttonPressedInterruptFired;

  if (buttonPressedInterruptFired) {
    buttonPressedInterruptFired = false;
    //attachInterrupt(digitalPinToInterrupt(BUTTON_SINK), buttonISR, LOW);
  }

  if (knobTurned) {
    lastKnobValue = getLatestKnobValue();
    speedValue = (lastKnobValue / KNOB_CONVERSION);
    brightnessValue = (((KNOB_MAX - lastKnobValue) / KNOB_CONVERSION));

    if (speedValue > 255) { speedValue = 255; }
    if (speedValue < 1) { speedValue = 1; }
    if (brightnessValue > 255) { brightnessValue = 255; }
    if (brightnessValue < 1) { brightnessValue = 1; }
  }

  /*if (knobTurned || buttonPushed) {
    Serial.print("Knob: "); Serial.println(knobTurned);
    Serial.print("Button: "); Serial.println(buttonPushed);
    Serial.print("Mode Changed to: "); Serial.println(lightMode);
    Serial.print("Interrupts Fired: "); Serial.println(interruptsFired);
    //Serial.print("BUTTON: "); Serial.println(digitalRead(BUTTON_SINK));
    Serial.print("Knob Value: "); Serial.println(lastKnobValue);
    Serial.print("Speed: "); Serial.println(speedValue);
    Serial.print("Brightness: "); Serial.println(brightnessValue);
  }*/

  // Check if the button was pressed and adjust the mode accordingly
  switch(lightMode) {
    case LIGHT_MODE_WIPE:
    //  Serial.println("Wipe Mode Enabled");
      //repeatWipe(speedValue);
      colorWipe(speedValue);
      break;
    case LIGHT_MODE_ALL_RED:
      //Serial.println("All Red Mode Enabled");
      setLightsToSingleColor(brightnessValue, 0x00, 0x00);
      break;
    case LIGHT_MODE_ALL_GREEN:
      //Serial.println("All Green Mode Enabled");
      setLightsToSingleColor(0x00, brightnessValue, 0x00);
      break;
    case LIGHT_MODE_ALL_BLUE:
      //Serial.println("All Blue Mode Enabled");
      setLightsToSingleColor(0x00, 0x00, brightnessValue);
      break;
    case LIGHT_MODE_ALL_PURPLE:
      //Serial.println("All Purple Mode Enabled");
      setLightsToSingleColor(brightnessValue, 0x00, brightnessValue);
      break;
    case LIGHT_MODE_SOLID_RAINBOW:
      //Serial.println("Rainbow Mode Enabled");
      rainbow(speedValue, 0xFF);
      break;
    default:
    case LIGHT_MODE_CHASE:
      //Serial.println("Chase Mode Enabled");
      chase(speedValue);
      break;
  }
}

boolean shouldInterruptRoutine() {
  boolean knobTurned = didTheKnobGetTurned();
  boolean interrupted =  knobTurned || buttonPressedInterruptFired;
//  if (interrupted) { Serial.print("Interrupt: "); Serial.print(knobTurned); Serial.print(" : "); Serial.println(buttonPressedInterruptFired); }
  return interrupted;
}

boolean didTheKnobGetTurned() {
    boolean wasAChange = false;

    int16_t knobValue = getLatestKnobValue();

    if (abs(lastKnobValue - knobValue) > KNOB_DELTA_THRESHOLD) {
        wasAChange = true;
    }

    return wasAChange;
}

uint16_t getLatestKnobValue() {
  // Read the dial 3 times and find out what the average is to get rid of any outliers
  uint16_t rawValue1 = analogRead(KNOB_DIAL);
  //Serial.print(rawValue1); Serial.print("\t");
  delay(KNOB_READ_DELAY);
  uint16_t rawValue2 = analogRead(KNOB_DIAL);
  //Serial.print(rawValue2); Serial.print("\t");
  delay(KNOB_READ_DELAY);
  uint16_t rawValue3 = analogRead(KNOB_DIAL);
  //Serial.print(rawValue3); Serial.print("\t");

  uint16_t average = (rawValue1 + rawValue2 + rawValue3) / 3;
  //Serial.println(average);

  return average;
}

COLOR getNextColor() {
  COLOR color = colorArray[colorIndex];
  colorIndex++;
  if (colorIndex >= NUM_COLORS) { colorIndex = 0; }
//      Serial.print("Color Index:  "); Serial.println(colorIndex);
  return color;
}

void setLightsToSingleColor(byte r, byte g, byte b) {

  uint16_t totalBrightness = (uint16_t)r + (uint16_t)g + (uint16_t)b;
  uint16_t divisor = ((CURRENT_PER_PIXEL * NUMBER_PIXELS) / MAX_CURRENT);
  uint16_t maxBrightness = 765;

  if (divisor != 0) {
    maxBrightness /= divisor;
  }

  //Serial.print("T: "); Serial.println(totalBrightness);
  //Serial.print("M: "); Serial.println(maxBrightness);
  if (totalBrightness > maxBrightness) {
    uint8_t deltaPerPixel = (uint8_t)(((totalBrightness - maxBrightness) / 3) & 255);
    //Serial.print("D: "); Serial.println(deltaPerPixel);
    if (r > deltaPerPixel) { r -= deltaPerPixel; }
    if (g > deltaPerPixel) { g -= deltaPerPixel; }
    if (b > deltaPerPixel) { b -= deltaPerPixel; }
  }

  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

// Fill the dots one after the other with a color
void colorWipe(uint8_t wait) {
  COLOR color = getNextColor();
  for(uint8_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color.r, color.g, color.b);
      strip.show();
      if (shouldInterruptRoutine()) { return; }
      delay(wait);
    //  if (shouldInterruptRoutine()) { return; }
  }
}

void rainbow(uint8_t speed, uint8_t brightness) {
  COLOR color = getNextColor();

  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color.r, color.b, color.g);
  }
  strip.show();
  delay(speed * 4);
}

void chase(uint8_t speed) {
    COLOR color = getNextColor();

    int pixelStart = directionModifier > 0 ? 0 : strip.numPixels() - 1;

  //  Serial.print("MOD: "); Serial.println(directionModifier);
  //  Serial.print("pixelStart: "); Serial.println(pixelStart);
  //  Serial.print("pixelStop: "); Serial.println(pixelStop);

    for(uint16_t pixelCounter = pixelStart; (pixelCounter >= 0 && pixelCounter < strip.numPixels()); pixelCounter += directionModifier) {
      for(int k = 0; k < CHASE_WINDOW; k++) {
        int currentPixel = pixelCounter + (k * directionModifier);
  //      Serial.print("C: "); Serial.println(currentPixel);
        if (currentPixel < NUMBER_PIXELS && currentPixel >= 0) {
          strip.setPixelColor(currentPixel, 0x7F, 0x7F, 0x7F);
        }
      }

      int previousPixel = pixelCounter - directionModifier;

  //    Serial.print("P: "); Serial.println(previousPixel);
      if (previousPixel < NUMBER_PIXELS && previousPixel >= 0 /*&& i >= CHASE_WINDOW && i <= (NUMBER_PIXELS - CHASE_WINDOW)*/) {
        strip.setPixelColor(previousPixel, color.r, color.g, color.b);
      }

      strip.show();
      delay(speed);
      if (shouldInterruptRoutine()) { return; }
    }

    directionModifier *= -1;
}
