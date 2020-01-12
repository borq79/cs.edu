//Put the following code into arduino
#include <NewTone.h>
#include "plant_emoji.h"

long soilhumid;
int dataIn = 2;
int load = 3;
int clock = 4;

float sinVal;
int toneVal;
int alarmDuration = 0;
bool lowOnMoisture = true;


int e = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(dataIn, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(load, OUTPUT);
  pinMode(A0, INPUT);

  maxAll(max7219_reg_scanLimit, 0x07);
  maxAll(max7219_reg_decodeMode, 0x00);
  maxAll(max7219_reg_shutdown, 0x01);
  maxAll(max7219_reg_displayTest, 0x00);

  for (e=1; e<=8; e++) { maxAll(e,0); }
  maxAll(max7219_reg_intensity, 0x0f & 0x0f);
  pinMode(BUZZER_PIN, OUTPUT);
  soilhumid = 0;
}

void loop()
{

  // detect soil humidity
  soilhumid = analogRead(A0);
  Serial.println(soilhumid);

  // lack water
  if (soilhumid > HUMIDITY_THRESHOLD) {

    // sad face
    maxSingle(1,0); maxSingle(2,102); maxSingle(3,102); maxSingle(4,0); maxSingle(5,60); maxSingle(6,66); maxSingle(7,66); maxSingle(8,0);
    
    // Play the low humidity song
    if (lowOnMoisture == false) {
      playMusic(gameover);
      lowOnMoisture = true;
    }
    

  } else {

    // happy face
    maxSingle(1,0); maxSingle(2,102); maxSingle(3,102); maxSingle(4,0); maxSingle(5,126); maxSingle(6,66); maxSingle(7,36); maxSingle(8,24);// buzzer off


    // Indicate, via the Mario theme, we are good on humidity again
    if (lowOnMoisture == true) {
      playMusic(flagpole);
      lowOnMoisture = false;
    }
 
  }
  
  delay(1000);
}

// Borrowed these note arrays from https://github.com/tsukisan/Arduino/tree/master/WiiClassicSoundboard
void playMusic(const int *song) {
  if (song != NULL) {
    int numberOfNotes = song[0];
    
    for(int i = 1; i < (numberOfNotes * 2); i += 2) {
      int note =  song[i];
      int duration = song[i + 1];

      int durationMs = 1000 / duration;
      
      tone(BUZZER_PIN, note, durationMs);
      delay(durationMs * 1.30);
      noTone(BUZZER_PIN); 
    }
  }
}

void putByte(byte data) {
  byte i = 8;
  byte mask;

  while(i > 0) {
    mask = 0x01 << (i - 1); // get bitmask
    digitalWrite( clock, LOW); // tick

    if (data & mask) { // choose bit
      digitalWrite(dataIn, HIGH);// send 1
    }else{
      digitalWrite(dataIn, LOW); // send 0
    }

    digitalWrite(clock, HIGH); // tock
    --i; // move to lesser bit
  }
}

void maxAll(byte reg, byte col) {
  digitalWrite(load, LOW); // begin
  putByte(reg); // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
  digitalWrite(load, LOW);
  digitalWrite(load,HIGH);
}

void maxSingle(byte reg, byte col) {
  digitalWrite(load, LOW); // begin
  putByte(reg); // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
  digitalWrite(load, LOW); // and load da stuff
  digitalWrite(load,HIGH);
}
