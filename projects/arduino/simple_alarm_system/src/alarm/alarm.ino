#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <NewPing.h>
#include <ss_oled.h>
#include "alarm.h"

#define ALARM_ENABLED true

void showDistanceOnDisplay(unsigned int distance);
void showAlertOnDisplay();
void initDisplay();

boolean alarmArmed = false;
int maxDistance = 0;

const COLOR ALARM_COLORS[] = { PURPLE, RED, BLUE, GREEN, ORANGE, WHITE, YELLOW, INDIGO };

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_OF_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


void setup(){
  randomSeed(analogRead(0));
  Serial.begin(115200);       // Open serial monitor at 115200 baud to see ping results.
  pinMode(ALARM, OUTPUT);

  strip.begin();
  strip.show();       // Initialize all pixels to 'off'

  initDisplay();
  delay(1000);
  oledWriteString(0, 0, 0, (char *)"Simple Alarm    ", FONT_NORMAL, 0, 1);
}


void loop(){
  delay(50);            // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  
  unsigned int uS = sonar.ping();             // Send ping, get ping time in microseconds (uS).
  unsigned int distance = uS / US_ROUNDTRIP_CM;
  Serial.print("uS: "); Serial.println(uS);
  Serial.print("Distance: "); Serial.println(distance);

  

  
  if(alarmArmed && ALARM_ENABLED && (distance < DOOR_WIDTH && distance > 0)) {
    Serial.println("Alarm Tripped");
    showAlertOnDisplay();
    playMusic(gameover);
    playMusic(flagpole);
    playMusic(theme);
  } else if(alarmArmed == false && ALARM_ENABLED && (distance >= DOOR_WIDTH)) {
    Serial.println("Alarm Being Armed");
    oledWriteString(0, 0, 3,(char *)"Alarm     ", FONT_STRETCHED, 0, 1);
    oledWriteString(0, 0, 6,(char *)"Armed    ", FONT_STRETCHED, 0, 1);
    alarmArmed = true;
  } else if (alarmArmed == false && ALARM_ENABLED) {
    Serial.println("Alarm Not armed");
    oledWriteString(0, 0, 3,(char *)"Position Alarm  ", FONT_NORMAL, 0, 1);
    String distanceWithUnits = String(distance) + " cm    ";
    oledWriteString(0, 0, 6,distanceWithUnits.c_str(), FONT_NORMAL, 0, 1);
    delay(1000);
  } else if (ALARM_ENABLED == false) {
    Serial.println("Alarm Disabled");
    color(0, 0, 0);
    showDistanceOnDisplay(distance);
  } else {
    Serial.println("Alarm Still Armed");
    oledWriteString(0, 0, 3,(char *)"Alarm     ", FONT_STRETCHED, 0, 1);
    oledWriteString(0, 0, 6,(char *)"Armed    ", FONT_STRETCHED, 0, 1);
  }
}

void showDistanceOnDisplay(unsigned int distance) {
  String distanceWithUnits = String(distance) + " cm    ";
  oledWriteString(0, 0, 3,(char *)"Distance    ", FONT_STRETCHED, 0, 1);
  oledWriteString(0, 0, 6,distanceWithUnits.c_str(), FONT_STRETCHED, 0, 1);
}

void showAlertOnDisplay() {
  oledWriteString(0, 0, 3,(char *)"ALERT!     ", FONT_STRETCHED, 0, 1);
  oledWriteString(0, 0, 6,(char *)"Intruder!  ", FONT_STRETCHED, 0, 1);
}


//helper function enabling us to send a colour in one command
void color (unsigned char red, unsigned char green, unsigned char blue)     // the color generating function
{
  for(int i = 0; i < NUM_OF_PIXELS; i++) {
    strip.setPixelColor(i, red, green, blue);
  }
  strip.show();
}


void initDisplay() {
  int rc = oledInit(OLED_128x64, 0, 0, -1, -1, 400000L);       // Standard HW I2C bus at 400Khz

  if (rc != OLED_NOT_FOUND)
  {
    oledFill(0, 1);
    oledWriteString(0, 0, 0, (char *)"Initializing ...", FONT_NORMAL, 0, 1);
  }
}

// Borrowed these note arrays from https://github.com/tsukisan/Arduino/tree/master/WiiClassicSoundboard
void playMusic(const uint16_t *song) {
  if (song != NULL) {
    uint16_t numberOfNotes = song[0];
    
      
    for(uint16_t i = 1; i < (numberOfNotes * 2); i += 2) {
      uint16_t note =  song[i];
      uint16_t duration = song[i + 1];

      uint16_t durationMs = 1000 / duration;
      
      tone(ALARM, note, durationMs);
      uint8_t randNumber = (uint8_t)random(0, NUM_OF_COLORS);
      COLOR colorToShow = ALARM_COLORS[randNumber];
  
      color(colorToShow.r, colorToShow.g, colorToShow.b);
      //Serial.print("Tone: ");Serial.println(note);
      delay(durationMs * 1.30);
      noTone(ALARM); 
    }
  }
}
/*

    delay(100);*/
