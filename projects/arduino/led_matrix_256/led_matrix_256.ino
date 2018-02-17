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

struct COLOR {
  byte r;
  byte g;
  byte b;
};

#define D {0x00, 0x00, 0x00}
#define B {0x00, 0x00, 0xFF}
#define G {0x00, 0xFF, 0x00}
#define R {0xFF, 0x00, 0x00}
#define P {0xFF, 0x00, 0xFF}
#define Y {0xFF, 0xFF, 0x00}
#define A {0x00, 0xFF, 0xFF}
#define W {0xFF, 0xFF, 0xFF}


const PROGMEM COLOR frames[][NUM_OF_PIXELS] = { {
        D,D,W,W,D,D,D,D,D,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,D,D,D,D,W,D,W,D,D,
        D,D,D,W,D,W,D,D,D,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,D,D,W,D,D,D,D,D,D,
        D,D,D,D,D,D,W,W,W,W,W,D,D,D,D,D,
        D,D,D,D,D,W,W,W,W,W,D,D,D,D,D,D,
        D,D,D,D,D,R,R,R,G,R,R,R,D,D,D,D,
        D,D,D,R,R,R,R,G,R,R,R,R,D,D,D,D,
        D,D,D,R,R,R,R,G,G,G,R,R,R,R,D,D,
        D,D,R,R,R,R,G,G,G,R,R,R,R,D,D,D,
        D,D,D,R,R,R,G,G,G,G,G,R,R,R,D,D,
        D,D,R,R,G,G,G,G,G,G,G,R,R,D,D,D,
        D,D,D,R,G,G,G,G,G,G,G,G,G,R,D,D,
        D,D,D,R,R,R,R,Y,R,R,R,R,D,D,D,D,
        D,D,D,D,D,R,R,R,Y,R,R,R,D,D,D,D,
        D,D,D,D,D,R,R,Y,R,R,D,D,D,D,D,D
    },

    {   D,D,D,D,D,D,D,D,R,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,R,D,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,R,W,R,D,D,D,D,D,D,
        D,D,D,D,D,R,R,W,R,R,D,D,D,D,D,D,
        D,D,D,D,D,D,D,R,R,R,D,D,D,D,D,D,
        D,D,D,D,D,D,D,Y,D,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,D,Y,D,D,D,D,D,D,D,
        D,D,D,D,W,W,W,W,W,W,W,D,D,D,D,D,
        D,D,D,D,D,W,W,W,W,W,W,W,D,D,D,D,
        D,D,D,D,W,W,W,W,W,W,W,D,D,D,D,D,
        D,D,D,D,D,W,W,W,W,W,W,W,D,D,Y,D,
        Y,D,Y,D,W,W,W,W,W,W,W,D,D,D,D,D,
        Y,Y,Y,D,D,W,W,W,W,W,W,W,D,D,Y,D,
        D,Y,Y,Y,W,W,W,W,W,W,W,Y,Y,Y,Y,D,
        D,D,D,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,D,D,
        D,D,D,Y,Y,Y,Y,Y,Y,Y,Y,Y,D,D,D,D
    },

    {   D,D,D,D,D,D,D,G,G,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,G,G,D,D,D,D,D,D,D,
        D,D,D,D,D,D,B,G,G,G,D,D,D,D,D,D,
        D,D,D,D,D,D,G,G,B,R,D,D,D,D,D,D,
        D,D,D,D,D,G,G,G,B,G,B,D,D,D,D,D,
        D,D,D,D,D,G,B,G,G,G,B,D,D,D,D,D,
        D,D,D,D,G,G,B,G,G,G,B,B,D,D,D,D,
        D,D,D,D,G,R,G,B,B,G,G,G,D,D,D,D,
        D,D,D,G,B,G,G,G,G,B,B,G,G,D,D,D,
        D,D,D,G,B,P,G,G,G,B,B,G,G,D,D,D,
        D,D,B,G,P,G,G,B,G,G,G,G,B,B,D,D,
        D,D,G,G,Y,G,B,B,G,R,G,B,B,G,D,D,
        D,G,G,G,G,B,B,G,G,G,B,B,G,G,G,D,
        D,D,D,D,D,D,Y,Y,Y,Y,D,D,D,D,D,D,
        D,D,D,D,D,D,Y,Y,Y,Y,D,D,D,D,D,D,
        D,D,D,D,D,D,Y,Y,Y,Y,D,D,D,D,D,D
    }
};

const uint16_t MAX_FRAME = ((sizeof(frames) / sizeof(COLOR)) / NUM_OF_PIXELS);


void setup() {
    irrecv.enableIRIn(); // Start the remote control receiver

    Serial.begin(115200); // setup serial printing so you can troubleshoot
    
    strip.begin();
    strip.setBrightness(1);
    strip.show(); // Initialize all pixels to 'off'

    Serial.print("Total number of frames: "); Serial.println(MAX_FRAME);
}

void loop() {
    uint16_t frameNumber = myRemoteRead();
    Serial.print("Frame in Show Routine=");
    Serial.print(frameNumber);
    Serial.println();

    renderFrame(frameNumber);
    delay(200);
}

void renderFrame( uint8_t frameNumber ) {
  // Ensure that there is a valid frame, otherwise we will walk off the memory allocated and then bad things happen
  if (frameNumber >= 0 && frameNumber < MAX_FRAME) {

    COLOR pixel = {0, 0, 0};
    
    // Setup every pixels
    for(uint8_t i = 0; i < strip.numPixels(); i++) {
        pixel = frames[frameNumber][i];
        strip.setPixelColor(i, pixel.r, pixel.g, pixel.b);
    }

    // Render the frame
    strip.show();
  }
}

uint8_t myRemoteRead() {
    uint8_t remoteFrame = MAX_FRAME; // Start off with an invalid frame to ease debugging
    uint32_t remoteValue = 0;
    
    if (irrecv.decode(&results))  {  // have we received an IR signal?
      remoteValue = results.value;
      
      switch(remoteValue) {
        case 0xA3C8EDDB: // + button pressed
          remoteFrame = 0;
          break;
  
        case 0xE5CFBD7F: // - button pressed
          remoteFrame = 1;
          break;
  
        default: // Go here if none of the above buttons are pressed
          remoteFrame = 2;
          break;
      }

      irrecv.resume(); // Receive the next value
    }

    Serial.print("remoteValue = ");
    Serial.print(remoteValue);
    Serial.println();
    
    Serial.print("remoteFrame = ");
    Serial.print(remoteFrame);
    Serial.println();
    
    return remoteFrame;
}
