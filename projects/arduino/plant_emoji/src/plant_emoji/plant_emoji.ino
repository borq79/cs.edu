//Put the following code into arduino
#include <NewTone.h>

long soilhumid;
long humid;
int dataIn = 2;
int load = 3;
int clock = 4;

#define ALARM 6
float sinVal;
int toneVal;
int alarmDuration = 0;
bool triggerAlarm = true;

#define MAX_ALARM_DURATION 10000

byte max7219_reg_noop = 0x00;
byte max7219_reg_digit0 = 0x01;
byte max7219_reg_digit1 = 0x02;
byte max7219_reg_digit2 = 0x03;
byte max7219_reg_digit3 = 0x04;
byte max7219_reg_digit4 = 0x05;
byte max7219_reg_digit5 = 0x06;
byte max7219_reg_digit6 = 0x07;
byte max7219_reg_digit7 = 0x08;
byte max7219_reg_decodeMode = 0x09;
byte max7219_reg_intensity = 0x0a;
byte max7219_reg_scanLimit = 0x0b;
byte max7219_reg_shutdown = 0x0c;
byte max7219_reg_displayTest = 0x0f;

int e = 0;

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

void alarm(){
  for (int x=0; x<180; x++) {
    // convert degrees to radians then obtain sin value
    sinVal = (sin(x*(3.1412/180)));
    // generate a frequency from the sin value
    toneVal = 2000+(int(sinVal*1000));
    NewTone(ALARM, toneVal);
  }
}

void alarmOff() {
  NewTone(ALARM, 0);
}


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
  pinMode(ALARM, OUTPUT);
  soilhumid = 0;
  humid = 500;
}

void loop()
{

  // detect soil humidity
  soilhumid = analogRead(A0);
  Serial.println(soilhumid);

  // lack water
  if (soilhumid > humid) {

    // sad face
    maxSingle(1,0); maxSingle(2,102); maxSingle(3,102); maxSingle(4,0); maxSingle(5,60); maxSingle(6,66); maxSingle(7,66); maxSingle(8,0);
    // buzzer on

    if (triggerAlarm) {
      alarm();
      alarmDuration += 1000;
    }
    

  } else {

    // happy face
    maxSingle(1,0); maxSingle(2,102); maxSingle(3,102); maxSingle(4,0); maxSingle(5,126); maxSingle(6,66); maxSingle(7,36); maxSingle(8,24);// buzzer off
    alarmOff();
    alarmDuration = 0;
    triggerAlarm = true;
  }
  
  delay(1000);

  if (alarmDuration >= MAX_ALARM_DURATION) {
    alarmOff();
    alarmDuration = 0;
    triggerAlarm = false;
  }
}
