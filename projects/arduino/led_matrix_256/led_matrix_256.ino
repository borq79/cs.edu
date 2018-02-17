#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#include "IRremote.h"  //Remote control library
#endif

#define NUM_OF_PIXELS    256
#define MATRIX_DATA_PIN  4
#define RECEIVER_PIN     12

IRrecv irrecv(RECEIVER_PIN);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_OF_PIXELS, MATRIX_DATA_PIN, NEO_GRB + NEO_KHZ800);

//int sensorPin = A5;    // select the input pin for the potentiometer
//int sensorValue = 0;  // variable to store the value coming from the sensor

int j;
uint32_t i;
uint8_t d = 0;
uint8_t b = pow(2,7);
uint8_t g = pow(2,15);
uint8_t r = pow(2,23);
uint8_t p = b+r;
uint8_t y = g+r;
uint8_t a = b+g;
uint8_t w = b+g+r;



const uint8_t frames[][NUM_OF_PIXELS] = { {
        d,d,w,w,d,d,d,d,d,d,d,d,d,d,d,d,
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
        d,d,d,d,d,r,r,y,r,r,d,d,d,d,d,d
    },

    {   d,d,d,d,d,d,d,d,r,d,d,d,d,d,d,d,
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
        d,d,d,y,y,y,y,y,y,y,y,y,d,d,d,d
    },

    {   d,d,d,d,d,d,d,g,g,d,d,d,d,d,d,d,
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
        d,d,d,d,d,d,y,y,y,y,d,d,d,d,d,d
    }
};

const uint16_t MAX_FRAME = sizeof(frames) / NUM_OF_PIXELS;


void setup() {
    irrecv.enableIRIn(); // Start the remote control receiver

    Serial.begin(9600); // setup serial printing so you can troubleshoot
    
    strip.begin();
    strip.setBrightness(1);
    strip.show(); // Initialize all pixels to 'off'

}

void loop() {
    uint16_t frameNumber = myRemoteRead();
    Serial.print("Frame in Show Routine=");
    Serial.print(frameNumber);
    Serial.println();

    renderFrame(frameNumber);
    delay(200);
}

void renderFrame( uint16_t frameNumber ) {
  // Ensure that there is a valid frame, otherwise we will walk off the memory allocated and then bad things happen
  if (frameNumber >= 0 && frameNumber <= MAX_FRAME) {

    // Setup every pixels
    for(uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, frames[frameNumber][i]);
    }

    // Render the frame
    strip.show();
  }
}

uint16_t myRemoteRead() {
    uint16_t remoteframe=3;
    Serial.print("remoteframe=");
    Serial.print(remoteframe);
    Serial.println();
    if (irrecv.decode(&results)) // have we received an IR signal?
    {
//switch(results.value){
        if(results.value==0xA3C8EDDB) {

//case 0xA3C8EDDB: // + button pressed
            remoteframe=1;  //set frame to 1 if remote control + button is pressed
            return remoteframe;
//break;
        }

        else if (results.value==0xE5CFBD7F) {
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
