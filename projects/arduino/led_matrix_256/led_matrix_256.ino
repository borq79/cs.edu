#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
  #include "IRremote.h"  //Remote control library
#endif

#define PIN 4

int receiver = 12; // Signal Pin of IR receiver to Arduino Digital Pin 12

IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

Adafruit_NeoPixel strip = Adafruit_NeoPixel(256, PIN, NEO_GRB + NEO_KHZ800);

//int sensorPin = A5;    // select the input pin for the potentiometer
//int sensorValue = 0;  // variable to store the value coming from the sensor

  int frame;
  int j;
  uint32_t i;
  uint32_t d=0; 
  uint32_t b=pow(2,7);  
  uint32_t g=pow(2,15);  
  uint32_t r=pow(2,23);
  uint32_t p=b+r;
  uint32_t y=g+r;
  uint32_t a=b+g;
  uint32_t w=b+g+r;


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  
    irrecv.enableIRIn(); // Start the remote control receiver

  Serial.begin(9600); // setup serial printing so you can troubleshoot
  //Serial.print("i=");  //Print something to serial terminal for viewing
  //Serial.print(i);
 strip.begin();
 strip.show(); // Initialize all pixels to 'off'

}

void loop() {

  //colorWipe(strip.Color(255, 0, 0), 10); // Red and time (not to exceed 255 since uint8_t is used below) to next LED
  //rainbow(1);


Serial.print("Frame in loop=");
Serial.print(frame);
Serial.println(); 
delay(200); 
  //sensorValue = analogRead(sensorPin);  //read the analog input value and print it
  //int frame = sensorValue/250;

rainbowCycle(128);
  //colorWipe1(strip.Color(0, 0, 255), 10); // Red and time to next LED
    }
    

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
frame=myRemoteRead();
Serial.print("Frame in Show Routine=");
Serial.print(frame);
Serial.println(); 
//Frame 1
if (frame==1) {
  
uint32_t myMatrixRow1[256] = {d,d,w,w,d,d,d,d,d,d,d,d,d,d,d,d,
                              d,d,d,d,d,d,d,d,d,d,d,w,d,w,d,d,
                              d,d,d,w,d,w,d,d,d,d,d,d,d,d,d,d,
                              d,d,d,d,d,d,d,d,d,w,d,d,d,d,d,d,
                              d,d,d,d,d,d,w,w,w,w,w,d,d,d,d,d,
                              d,d,d,d,d,w,w,w,w,w,d,d,d,d,d,d,
                              d,d,d,d,d,r,r,r,g,r,r,r,d,d,d,d,
                              d,d,d,r,r,r,r,g,r,r,r,r,d,d,d,d,
                              d,d,d,r,r,r,r,g,g,g,r,r,r,r,d,d,
                              d,d,r,r,r,r,g,g,g,r,r,r,r,d,d,d,
                              d,d,d,r,r,r,g,g,g,g,g,r,r,r,d,d,
                              d,d,r,r,g,g,g,g,g,g,g,r,r,d,d,d,
                              d,d,d,r,g,g,g,g,g,g,g,g,g,r,d,d,
                              d,d,d,r,r,r,r,y,r,r,r,r,d,d,d,d,
                              d,d,d,d,d,r,r,r,y,r,r,r,d,d,d,d,
                              d,d,d,d,d,r,r,y,r,r,d,d,d,d,d,d};
    
for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, myMatrixRow1[i]);
      strip.setBrightness(1);
            //delay(50);
            }


      //delay(4000);
}

//Frame 2
else if (frame==2) {

uint32_t myMatrixRow2[256] = {d,d,d,d,d,d,d,d,r,d,d,d,d,d,d,d,
                              d,d,d,d,d,d,d,r,d,d,d,d,d,d,d,d,
                              d,d,d,d,d,d,d,r,w,r,d,d,d,d,d,d,
                              d,d,d,d,d,r,r,w,r,r,d,d,d,d,d,d,
                              d,d,d,d,d,d,d,r,r,r,d,d,d,d,d,d,
                              d,d,d,d,d,d,d,y,d,d,d,d,d,d,d,d,
                              d,d,d,d,d,d,d,d,y,d,d,d,d,d,d,d,
                              d,d,d,d,w,w,w,w,w,w,w,d,d,d,d,d,
                              d,d,d,d,d,w,w,w,w,w,w,w,d,d,d,d,
                              d,d,d,d,w,w,w,w,w,w,w,d,d,d,d,d,
                              d,d,d,d,d,w,w,w,w,w,w,w,d,d,y,d,
                              y,d,y,d,w,w,w,w,w,w,w,d,d,d,d,d,
                              y,y,y,d,d,w,w,w,w,w,w,w,d,d,y,d,
                              d,y,y,y,w,w,w,w,w,w,w,y,y,y,y,d,
                              d,d,d,y,y,y,y,y,y,y,y,y,y,y,d,d,
                              d,d,d,y,y,y,y,y,y,y,y,y,d,d,d,d};

for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, myMatrixRow2[i]);
      strip.setBrightness(1);
            //delay(50);
            }
    

      //delay(1000);
}

//Frame 3
else if (frame==3) {

uint32_t myMatrixRow3[256] = {d,d,d,d,d,d,d,g,g,d,d,d,d,d,d,d,
                              d,d,d,d,d,d,d,g,g,d,d,d,d,d,d,d,
                              d,d,d,d,d,d,b,g,g,g,d,d,d,d,d,d,
                              d,d,d,d,d,d,g,g,b,r,d,d,d,d,d,d,
                              d,d,d,d,d,g,g,g,b,g,b,d,d,d,d,d,
                              d,d,d,d,d,g,b,g,g,g,b,d,d,d,d,d,
                              d,d,d,d,g,g,b,g,g,g,b,b,d,d,d,d,
                              d,d,d,d,g,r,g,b,b,g,g,g,d,d,d,d,
                              d,d,d,g,b,g,g,g,g,b,b,g,g,d,d,d,
                              d,d,d,g,b,p,g,g,g,b,b,g,g,d,d,d,
                              d,d,b,g,p,g,g,b,g,g,g,g,b,b,d,d,
                              d,d,g,g,y,g,b,b,g,r,g,b,b,g,d,d,
                              d,g,g,g,g,b,b,g,g,g,b,b,g,g,g,d,
                              d,d,d,d,d,d,y,y,y,y,d,d,d,d,d,d,
                              d,d,d,d,d,d,y,y,y,y,d,d,d,d,d,d,
                              d,d,d,d,d,d,y,y,y,y,d,d,d,d,d,d};

for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, myMatrixRow3[i]);
      strip.setBrightness(1);
            //delay(50);
            }


      //delay(1000);
}
if (frame<=3){
        if(frame>=0) {
        strip.show();
      }
      }
//delay(5000);
}

int myRemoteRead(){
int remoteframe=3;
Serial.print("remoteframe=");
Serial.print(remoteframe);
Serial.println();  
if (irrecv.decode(&results)) // have we received an IR signal?
    {
//switch(results.value){
if(results.value==0xA3C8EDDB){

//case 0xA3C8EDDB: // + button pressed
remoteframe=1;  //set frame to 1 if remote control + button is pressed
return remoteframe;
//break;
}

else if (results.value==0xE5CFBD7F){
//case 0xE5CFBD7F: // - button pressed
remoteframe=2;  //set frame to 1 if remote control + button is pressed
return remoteframe;
//break;
  }

//frame=3; //set frame to default 3

}
//irrecv.resume(); // receive the next value
return remoteframe;
}
