
// Analog Pin 0 is what is used to read the voltage levels from the analog pin of the PCB. 
// Possible values are 0 - 1023 where 0 is the brightest and 1023 is the darkest
#define SENSOR_ANALONG_PIN 0

// Digital Pin used to read the digital output (dark = 0, not-dark = 1) from the PCB.
#define SENSOR_DIGITAL_PIN 8

// We will drive pin 9 based on the input from the photoresitor. When using the analog
// output from the PCB, drive the pin as a PWM so that reflects the brightness of the
// room. When using the digital output of the PCB, drive pin 9 high or low depending
// to turn the LED on or off - acts as a 'trip sensor'
#define LED_PWM_PIN        9

void setup() {
  // Init the serial communications for debugging messages
  Serial.begin(9600);
  
  // Make the digital pin an input so we can read the values from the PCB D0 output
  pinMode(SENSOR_DIGITAL_PIN, INPUT);
  
  // Make the PWM pin output so we can drive the LED
  pinMode(LED_PWM_PIN, OUTPUT);
}

void loop() {
  // *** Uncomment this line if the data line runs from A0 on the PCB to A0 on the Arduino
  //analogSensorPin();
  
  // *** Uncomment this line if the data line runs from D0 on the PCB to pin 8 of the Arduino
  //digitalSensorPin();
  
  // Add this delay so the output is trottled to something reasonable
  delay(1000);
}

void analogSensorPin() {
  // Read the value on the Analog pin - represents the value from the photoresistor PCB. Values are
  // 0 (bright) to 1023 (dark)
  int analogVal = analogRead(SENSOR_ANALONG_PIN);
  
  // The PWM is 0-255, where 255 is 100% duty cycle and 0 is 0% duty cycle. To dim the LED when the
  // photoresitor has the highest resistence (brightest) first divide by 4 (1024 scale to 256 scale)
  // and then subtract that value from 255 (highest value) to convert from the photoresistor scale
  // to the LED scale (0 is the brightest on the photoresistor, but 255 is brightest for the LED).
  analogWrite(LED_PWM_PIN, 255 - (analogVal / 4));
  
  Serial.println(analogVal); 
}
 
void digitalSensorPin() {
  // This pin will either be on or off (1 or 0). You can adjust the screw on the PCB to adjust when the line is driven high or low.
  int digitalVal = digitalRead(SENSOR_DIGITAL_PIN);
  
  // Write the respective value to the digital Arudino pin to turn the LED on or off 
  digitalWrite(LED_PWM_PIN, digitalVal);
  
  Serial.println(digitalVal);   
}
