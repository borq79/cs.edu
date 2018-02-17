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
  byte red;
  byte green;
  byte blue;
};

#define D {0x00, 0x00, 0x00}
#define b {0x00, 0x00, 0xFF}
#define g {0x00, 0xFF, 0x00}
#define r {0xFF, 0x00, 0x00}
#define p {0xFF, 0x00, 0xFF}
#define y {0xFF, 0xFF, 0x00}
#define a {0x00, 0xFF, 0xFF}
#define w {0xFF, 0xFF, 0xFF}


const PROGMEM COLOR frames[][NUM_OF_PIXELS] = { {
        D,D,w,w,D,D,D,D,D,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,D,D,D,D,w,D,w,D,D,
        D,D,D,w,D,w,D,D,D,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,D,D,w,D,D,D,D,D,D,
        D,D,D,D,D,D,w,w,w,w,w,D,D,D,D,D,
        D,D,D,D,D,w,w,w,w,w,D,D,D,D,D,D,
        D,D,D,D,D,r,r,r,g,r,r,r,D,D,D,D,
        D,D,D,r,r,r,r,g,r,r,r,r,D,D,D,D,
        D,D,D,r,r,r,r,g,g,g,r,r,r,r,D,D,
        D,D,r,r,r,r,g,g,g,r,r,r,r,D,D,D,
        D,D,D,r,r,r,g,g,g,g,g,r,r,r,D,D,
        D,D,r,r,g,g,g,g,g,g,g,r,r,D,D,D,
        D,D,D,r,g,g,g,g,g,g,g,g,g,r,D,D,
        D,D,D,r,r,r,r,y,r,r,r,r,D,D,D,D,
        D,D,D,D,D,r,r,r,y,r,r,r,D,D,D,D,
        D,D,D,D,D,r,r,y,r,r,D,D,D,D,D,D
    },

    {   D,D,D,D,D,D,D,D,r,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,r,D,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,r,w,r,D,D,D,D,D,D,
        D,D,D,D,D,r,r,w,r,r,D,D,D,D,D,D,
        D,D,D,D,D,D,D,r,r,r,D,D,D,D,D,D,
        D,D,D,D,D,D,D,y,D,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,D,y,D,D,D,D,D,D,D,
        D,D,D,D,w,w,w,w,w,w,w,D,D,D,D,D,
        D,D,D,D,D,w,w,w,w,w,w,w,D,D,D,D,
        D,D,D,D,w,w,w,w,w,w,w,D,D,D,D,D,
        D,D,D,D,D,w,w,w,w,w,w,w,D,D,y,D,
        y,D,y,D,w,w,w,w,w,w,w,D,D,D,D,D,
        y,y,y,D,D,w,w,w,w,w,w,w,D,D,y,D,
        D,y,y,y,w,w,w,w,w,w,w,y,y,y,y,D,
        D,D,D,y,y,y,y,y,y,y,y,y,y,y,D,D,
        D,D,D,y,y,y,y,y,y,y,y,y,D,D,D,D
    },

    {   D,D,D,D,D,D,D,g,g,D,D,D,D,D,D,D,
        D,D,D,D,D,D,D,g,g,D,D,D,D,D,D,D,
        D,D,D,D,D,D,b,g,g,g,D,D,D,D,D,D,
        D,D,D,D,D,D,g,g,b,r,D,D,D,D,D,D,
        D,D,D,D,D,g,g,g,b,g,b,D,D,D,D,D,
        D,D,D,D,D,g,b,g,g,g,b,D,D,D,D,D,
        D,D,D,D,g,g,b,g,g,g,b,b,D,D,D,D,
        D,D,D,D,g,r,g,b,b,g,g,g,D,D,D,D,
        D,D,D,g,b,g,g,g,g,b,b,g,g,D,D,D,
        D,D,D,g,b,p,g,g,g,b,b,g,g,D,D,D,
        D,D,b,g,p,g,g,b,g,g,g,g,b,b,D,D,
        D,D,g,g,y,g,b,b,g,r,g,b,b,g,D,D,
        D,g,g,g,g,b,b,g,g,g,b,b,g,g,g,D,
        D,D,D,D,D,D,y,y,y,y,D,D,D,D,D,D,
        D,D,D,D,D,D,y,y,y,y,D,D,D,D,D,D,
        D,D,D,D,D,D,y,y,y,y,D,D,D,D,D,D
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
        strip.setPixelColor(i, pixel.red, pixel.green, pixel.blue);
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
