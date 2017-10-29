#include <Adafruit_NeoPixel.h>

#define PIN 12

#define NUMBER_OF_LEDS 60
#define MAX_CURRENT_PER_LED  60
#define MAX_CURRENT_DRAW_STRIP 1800
static uint32_t POTENTIAL_CURRENT_DRAW = (MAX_CURRENT_PER_LED * NUMBER_OF_LEDS);
static uint32_t BRIGHTNESS_DIVISOR = ((POTENTIAL_CURRENT_DRAW > MAX_CURRENT_DRAW_STRIP) ? ((POTENTIAL_CURRENT_DRAW * 1000) / MAX_CURRENT_DRAW_STRIP) : 1000);
static byte MAX_BRIGHTNESS = (255000 / BRIGHTNESS_DIVISOR) & 255;

#define POTENTIOMETER_STEP_DIVISOR 2
#define RESISTENCE_OFFSET 15

#define KNOB_MAX 1024
#define KNOB_MIN 0
static float KNOB_RANGE = KNOB_MAX - KNOB_MIN;
static uint16_t KNOB_CONVERSION = round(KNOB_RANGE / 255);

#define DEBOUNCE_TIME 500

// Speed Control
#define KNOB_PWR  11
#define KNOB_DIAL A0
// #define KNOB_GND_1  2

#define CHASE_WINDOW 4

// Mode
// #define BUTTON_SUPPLY 8
#define BUTTON_SINK   3

//
// #define KNOB_PWR_1  4
// #define KNOB_GND_1  2
// #define KNOB_DIAL_1 3

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Serial.print("Maximum Brightness due to current limitations: "); Serial.println(MAX_BRIGHTNESS);

  randomSeed(analogRead(1));

  pinMode(KNOB_PWR,  OUTPUT);
  pinMode(KNOB_DIAL, INPUT);
  digitalWrite(KNOB_PWR, HIGH);

  pinMode(BUTTON_SINK, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(BUTTON_SINK), buttonISR, LOW);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


enum LIGHT_MODE {
  LIGHT_MODE_START = 0,

  LIGHT_MODE_WIPE = LIGHT_MODE_START,
  LIGHT_MODE_ALL_RED,
  LIGHT_MODE_ALL_GREEN,
  LIGHT_MODE_ALL_BLUE,
  LIGHT_MODE_SOLID_RAINBOW,
  LIGHT_MODE_CHASE,
  LIGHT_MODE_FADE,
  LIGHT_MODE_PULSE,
  LIGHT_MODE_MEDLEY,

  LIGHT_MODE_END = LIGHT_MODE_MEDLEY
};

volatile byte lightMode = LIGHT_MODE_START;
volatile int lastChange = 0;
volatile boolean interruptFired = false;

// Called when the button is pressed
void buttonISR() {
  if ((millis() - lastChange) > DEBOUNCE_TIME) {
    interruptFired = true;
    lastChange = millis();
    if (lightMode == LIGHT_MODE_END) {
      lightMode = LIGHT_MODE_START;
    } else {
      lightMode++;
    }
    detachInterrupt(digitalPinToInterrupt(BUTTON_SINK));
  }
}


void loop() {

  uint16_t rawValue = analogRead(KNOB_DIAL);
  byte speedValue = (rawValue / KNOB_CONVERSION) & 255;
  byte brightnessValue = (((KNOB_MAX - rawValue) / KNOB_CONVERSION) & 255);

  /*Serial.print("Mode: "); Serial.println(lightMode);
  Serial.print("BUTTON: "); Serial.println(digitalRead(BUTTON_SINK));
  Serial.print("RAW: "); Serial.println(rawValue);
  Serial.print("SPEED: "); Serial.println(speedValue);
  Serial.print("BRIGHTNESS: "); Serial.println(brightnessValue);*/
//delay(500);

interruptFired = false;
attachInterrupt(digitalPinToInterrupt(BUTTON_SINK), buttonISR, LOW);
  // Check if the button was pressed and adjust the mode accordingly
  switch(lightMode) {
    case LIGHT_MODE_WIPE:
      repeatWipe(speedValue);
      break;
    case LIGHT_MODE_ALL_RED:
      setLightsToSingleColor(brightnessValue, 0x00, 0x00);
      break;
    case LIGHT_MODE_ALL_GREEN:
      setLightsToSingleColor(0x00, brightnessValue, 0x00);
      break;
    case LIGHT_MODE_ALL_BLUE:
      setLightsToSingleColor(0x00, 0x00, brightnessValue);
      break;
    case LIGHT_MODE_SOLID_RAINBOW:
      rainbow(speedValue);
      break;
    case LIGHT_MODE_CHASE:
      chase(speedValue);
      break;
    case LIGHT_MODE_FADE:
      break;
    case LIGHT_MODE_PULSE:
      break;
    default:
    case LIGHT_MODE_MEDLEY:
      break;
  }

  // Some example procedures showing how to display to the pixels:


  //rainbow(analogRead(KNOB_DIAL_1) & 255);
  //rainbowCycle(20);
}

void setLightsToSingleColor(byte r, byte g, byte b) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r & MAX_BRIGHTNESS, g & MAX_BRIGHTNESS, b & MAX_BRIGHTNESS);

    if (interruptFired) { return; }
  }
  strip.show();
}

void repeatWipe(uint8_t wipeSpeed) {
  colorWipe(strip.Color(MAX_BRIGHTNESS, 0, 0), wipeSpeed); // Red
  colorWipe(strip.Color(0, MAX_BRIGHTNESS, 0), wipeSpeed); // Green
  colorWipe(strip.Color(0, 0, MAX_BRIGHTNESS), wipeSpeed); // Blue
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
      if (interruptFired) { return; }
  }
}

void rainbow(uint8_t wait) {
  wait /= 16;
  Serial.print("Rainbow Wait: "); Serial.println(wait);
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255, MAX_BRIGHTNESS));
    }
    strip.show();
    delay(wait);
    if (interruptFired) { return; }
  }
}

void chase(uint8_t speed) {
  uint32_t colorToWipe = Wheel(random(0, 255), MAX_BRIGHTNESS);

  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(i, MAX_BRIGHTNESS));
  }
  strip.show();

  int directionModifier = 1;

  for(int j=0; j<256; j++) {
    colorToWipe = Wheel(random(0, 255), MAX_BRIGHTNESS);
    Serial.print("COLOR: "); Serial.println(colorToWipe);
    int pixelStart = directionModifier > 0 ? 0 : strip.numPixels() - 1;
  //  int pixelStop = directionModifier > 0 ? strip.numPixels() : 0;

    Serial.print("MOD: "); Serial.println(directionModifier);
    Serial.print("pixelStart: "); Serial.println(pixelStart);
  //  Serial.print("pixelStop: "); Serial.println(pixelStop);

    for(int i=pixelStart; (i >= 0 && i < strip.numPixels()); i += directionModifier) {


      for(int k = 0; k < CHASE_WINDOW; k++) {
        int currentPixel = i + (k * directionModifier);
        Serial.print("C: "); Serial.println(currentPixel);
        if (currentPixel < NUMBER_OF_LEDS && currentPixel > 0) {
          strip.setPixelColor(i + k, MAX_BRIGHTNESS, MAX_BRIGHTNESS, MAX_BRIGHTNESS);
        }
      }

      int previousPixel = i - directionModifier;

      Serial.print("P: "); Serial.println(previousPixel);
      if (previousPixel < NUMBER_OF_LEDS && previousPixel > 0) {
        strip.setPixelColor(previousPixel, colorToWipe);
      }

      strip.show();
      delay(speed);
      if (interruptFired) { return; }
    }

    directionModifier *= -1;
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
/*void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}*/

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, byte brightness) {
  if(WheelPos < 85) {
   return strip.Color((WheelPos * 3) & brightness, (255 - WheelPos * 3) & brightness, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color((255 - WheelPos * 3) & brightness, 0, (WheelPos * 3) & brightness);
  } else {
   WheelPos -= 170;
   return strip.Color(0, (WheelPos * 3) & brightness, (255 - WheelPos * 3) & brightness);
  }
}
