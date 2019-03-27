#include <Arduino.h>
#include <NewTone.h>
#include <Adafruit_NeoPixel.h>
#include <NewPing.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 *display;
void showDistanceOnDisplay(unsigned int distance);
void showAlertOnDisplay();
void initDisplay();

// LED Light Strand Pin
#define LED_PIN      2

#define MAX_ALARM_TIME 5000

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters).
#define DOOR_WIDTH   71  // Door Width in centimeters (28" Door)

#define ALARM 3
float sinVal;
int toneVal;
int alarmDuration = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, LED_PIN, NEO_GRB + NEO_KHZ800);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
boolean triggered = false;

void setup(){
  Serial.begin(115200);       // Open serial monitor at 115200 baud to see ping results.
  pinMode(ALARM, OUTPUT);

  strip.begin();
  strip.show();       // Initialize all pixels to 'off'

  initDisplay();
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(WHITE);
  display->setCursor(15,0);
  display->println("Distance");
  display->setCursor(15, 30);
  display->setTextSize(1);
  display->println("Initializing ...");
  display->display();

  delay(1000);
}


void loop(){
  Serial.print("Alarm delay: "); Serial.println(alarmDuration);
  if(triggered == true && alarmDuration < MAX_ALARM_TIME) {
    alarmDuration += 600;
    alarm();
  } else if(triggered == true) {
    alarmDuration = 0;
    triggered = false;
    alarmOff();
  } else if (triggered == false) {
    delay(50);            // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    unsigned int uS = sonar.ping();             // Send ping, get ping time in microseconds (uS).
    unsigned int distance = uS / US_ROUNDTRIP_CM;
    Serial.println(distance);
    
    if(distance < DOOR_WIDTH && distance > 0) {
      triggered = true;
      showAlertOnDisplay();
    } else {
      showDistanceOnDisplay(distance);
    }
  } 
}

void showDistanceOnDisplay(unsigned int distance) {
  display->clearDisplay();
  display->setTextSize(2);
  display->setCursor(15,0);
  display->println("Distance");
  display->setCursor(15, 30);
  display->setTextSize(3);
  display->print(distance); 
  display->setTextSize(2);
  display->println(" cm");
  display->display(); 
}

void showAlertOnDisplay() {
  display->clearDisplay();
  display->setTextSize(2);
  display->setCursor(20,0);
  display->println("ALERT!");
  display->setCursor(10, 30);
  display->setTextSize(2);
  display->println("Intruder!");
  display->display(); 
}

void alarm(){
  color(225,0,178);    //red
  delay(100);
  color(0,199,225);  //yelow
  delay(100);
  color(4,225,0);
  delay(100);
  color(109,57,1);
  delay(100);
  color(8,0,225);
  delay(100);
  color(242,225,0);
  delay(100);
  for (int x=0; x<180; x++) {
    // convert degrees to radians then obtain sin value
    sinVal = (sin(x*(3.1412/180)));
    // generate a frequency from the sin value
    toneVal = 2000+(int(sinVal*1000));
    NewTone(ALARM, toneVal);
  }
}

void alarmOff() {
  color(0, 0, 0);
  NewTone(ALARM, 0);
}

//helper function enabling us to send a colour in one command
void color (unsigned char red, unsigned char green, unsigned char blue)     // the color generating function
{
  for(int i = 0; i < 10; i++) {
    strip.setPixelColor(i, red, green, blue);
  }
  strip.show();
}


void initDisplay() {
  delay(200);

  display = new Adafruit_SSD1306(4);

   // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  display->display(); // show splashscreen
  delay(1500);
  display->clearDisplay();   // clears the screen and buffer
}
