
// Analog Pin 0 is what is used to read the voltage levels from the analog pin of the PCB. 
// Possible values are 0 - 1023 where 0 is the brightest and 1023 is the darkest
#define SENSOR_ANALONG_PIN 0

// We will drive 5 PWM pins based on the input from the photoresitor. When using the analog
// output from the PCB, drive the pin as a PWMs so that reflects the brightness of the
// room. 
#define LED_ARRAY_ELEMENT_0 5
#define LED_ARRAY_ELEMENT_1 6
#define LED_ARRAY_ELEMENT_2 9
#define LED_ARRAY_ELEMENT_3 10
#define LED_ARRAY_ELEMENT_4 11

// Sample rate
#define PHOTORESISTOR_SAMPLE_RATE_IN_MS 50

static int ledArray[5] = {LED_ARRAY_ELEMENT_0, LED_ARRAY_ELEMENT_1, LED_ARRAY_ELEMENT_2, LED_ARRAY_ELEMENT_3, LED_ARRAY_ELEMENT_4};
static const int NUM_OF_LEDS_IN_ARRAY = sizeof(ledArray) / sizeof(int); 

static const int LED_MIN_BRIGHTNESS = 0;
static const int LED_MAX_BRIGHTNESS = 255;

static const int PHOTORESISTOR_MIN_VALUE = 0;
static const int PHOTORESISTOR_MAX_VALUE = 1023;

static const int LED_RANGE = (LED_MAX_BRIGHTNESS - LED_MIN_BRIGHTNESS) + 1;
static const int PHOTORESISTOR_RANGE = (PHOTORESISTOR_MAX_VALUE - PHOTORESISTOR_MIN_VALUE) + 1;

static const int  PHOTORESISTOR_STEP = PHOTORESISTOR_RANGE / NUM_OF_LEDS_IN_ARRAY;
static const float PHOTORESISTOR_TO_LED_MULTIPLIER = (float)LED_RANGE / (float)PHOTORESISTOR_STEP;

void setup() {
  // Init the serial communications for debugging messages
  Serial.begin(9600);
  
  Serial.print("Number of LEDs in the Array: "); Serial.println(NUM_OF_LEDS_IN_ARRAY);
  Serial.print("LED Range: "); Serial.println(LED_RANGE);
  Serial.print("Photoresistor Step: "); Serial.println(PHOTORESISTOR_STEP);
  Serial.print("Photoresistor Range: "); Serial.println(PHOTORESISTOR_RANGE);
  Serial.print("Multiplier: "); Serial.println(PHOTORESISTOR_TO_LED_MULTIPLIER);
  
  // Iterate over the array and set the PWM pins to output
  for(int i = 0; i < NUM_OF_LEDS_IN_ARRAY; i++) {
    pinMode(ledArray[i], OUTPUT);
  }
}

void loop() {
  // *** Uncomment this line if the data line runs from A0 on the PCB to A0 on the Arduino
  int photoresistorAnalogValue = readAnalogSensorPin();
  
  updateLedArray(photoresistorAnalogValue);
  
  // Add this delay so the output is trottled to something reasonable
  delay(PHOTORESISTOR_SAMPLE_RATE_IN_MS);
}

int readAnalogSensorPin() {
  // Read the value on the Analog pin - represents the value from the photoresistor PCB. Values are
  // 0 (bright) to 1023 (dark)
  int photoresistorAnalogValue = analogRead(SENSOR_ANALONG_PIN);
  return photoresistorAnalogValue;
}
 
void updateLedArray(int photoresistorAnalogValue) {
  // The PWM is 0-255, where 255 is 100% duty cycle and 0 is 0% duty cycle. The photoresistor is brightest when
  // it is a analog value of 0, and darkest when it is 1023. 
  
  // Invert the value to make the max the brightness value instead of 0 being the brightest
  // Remember, a value of 0 (or min value set above) means no resistance and therefore means the room is bright
  int brightness = PHOTORESISTOR_MAX_VALUE - photoresistorAnalogValue;

  // This will take the 'floor' value of the brightness divided by the photoresistor step so that
  // we know how many LEDs to just drive at full brightness. The reminder is used to determine
  // the duty cycle of the last LED to drive.
  int numberOfLedsToFullBrightness = brightness / PHOTORESISTOR_STEP;
  int remainderOfPhotoresistorBrightnessForLastLED = brightness % PHOTORESISTOR_STEP;
  int lastLEDPWMValue = (int)((float)remainderOfPhotoresistorBrightnessForLastLED * PHOTORESISTOR_TO_LED_MULTIPLIER);

  if (photoresistorAnalogValue < PHOTORESISTOR_MIN_VALUE || photoresistorAnalogValue > PHOTORESISTOR_MAX_VALUE) {
      Serial.print("Photoresistor value is out of expected range: "); Serial.println(photoresistorAnalogValue);
      return;
  }

  Serial.println(" ------------------------------------------------------------------- ");
  Serial.print("Number of LEDs to Drive at 100% duty cycle: "); Serial.println(numberOfLedsToFullBrightness);
  Serial.print("Remainder of last LED brightness: "); Serial.println(remainderOfPhotoresistorBrightnessForLastLED);
  Serial.print("PWM value of the last LED in the array that is lit: "); Serial.println(lastLEDPWMValue);
  Serial.print("Photoresitor analog reading: "); Serial.println(photoresistorAnalogValue);
  Serial.print("Photoresitor brightness value: "); Serial.println(brightness);

  int ledCounter = 0;

  // Any LEDs that need to be driven at 100% duty cycle, do that now
  for(ledCounter = 0; ledCounter < numberOfLedsToFullBrightness; ledCounter++) {
      //Serial.print("LED "); Serial.print(ledCounter); Serial.println(" set to full brightness");
      analogWrite(ledArray[ledCounter], LED_MAX_BRIGHTNESS);
  }

  if (ledCounter < NUM_OF_LEDS_IN_ARRAY) {
      // Now set the duty cycle for the only LED that will be dynamic
      //Serial.print("LED "); Serial.print(ledCounter); Serial.print(" is PWM value: "); + Serial.println(lastLEDPWMValue);
      analogWrite(ledArray[ledCounter], lastLEDPWMValue);

      // Turn off the remaining LEDs
      for (ledCounter++; ledCounter < NUM_OF_LEDS_IN_ARRAY; ledCounter++) {
          //Serial.print("LED "); Serial.print(ledCounter); Serial.println(" set to off");
          analogWrite(ledArray[ledCounter], LED_MIN_BRIGHTNESS);
      }
  }
}
