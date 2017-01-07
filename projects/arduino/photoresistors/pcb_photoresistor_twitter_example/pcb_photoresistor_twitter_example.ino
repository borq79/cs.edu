#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <Twitter.h>

// Twitter allows 140 characters
#define MAX_TWITTER_MESSAGE 140

// The buffer that needs to hold the message needs an extra byte to store the NULL character
#define TWITTER_MESSAGE_BUFFER_SIZE (MAX_TWITTER_MESSAGE + 1)


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

// Delay time for twitter library to avoid overwhelming the server
#define ONE_MINUTE 60000

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Fallback, static IP, in case DHCP fails
IPAddress ip(192, 168, 0, 12);  

Twitter twitter("PUT YOUR TWITTER TOKEN KEY HERE");

void setup() {
  // Init the serial communications for debugging messages
  Serial.begin(9600);
  
  // Make the digital pin an input so we can read the values from the PCB D0 output
  pinMode(SENSOR_DIGITAL_PIN, INPUT);
  
  // Make the PWM pin output so we can drive the LED
  pinMode(LED_PWM_PIN, OUTPUT);

  // Attempt to get an IP. If it fails, bind a static IP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP. Going to attempt and bind the fallback static IP");
    Ethernet.begin(mac, ip);
  }
}

void loop() {
  // Data line runs from D0 on the PCB to pin 8 of the Arduino
  bool lightsAreOn = digitalSensorPin() == 0 ? false : true;

  sendReadingsToTwitter(lightsAreOn);
  
  Serial.println("Waiting one minute before getting the next reading ...");
  delay(ONE_MINUTE);
}

 
int digitalSensorPin() {
  // This pin will either be on or off (1 or 0). You can adjust the screw on the PCB to adjust when the line is driven high or low.
  int digitalVal = digitalRead(SENSOR_DIGITAL_PIN);
  
  // Write the respective value to the digital Arudino pin to turn the LED on or off 
  digitalWrite(LED_PWM_PIN, digitalVal);
  
  Serial.println(digitalVal);

  return digitalVal;
}


void sendReadingsToTwitter(bool lightsAreOn) {
  char twitterMessage[TWITTER_MESSAGE_BUFFER_SIZE];
  
  // Create the twitter message - it will crop it to 140 characters (MAX_TWITTER_MESSAGE) and ensure the NULL is in place (why we add 1 to get TWITTER_MESSAGE_BUFFER_SIZE)
  snprintf(twitterMessage, TWITTER_MESSAGE_BUFFER_SIZE, "The lights in the Cary Area Public Library are currently %s", lightsAreOn ? "on" : "off");
  
  // Print for our own sake
  Serial.println(twitterMessage);

  Serial.println("Connecting to the Twitter Arduino proxy ...");
  if (twitter.post(twitterMessage)) {
    int status = twitter.wait();
    if (status == 200) {
      Serial.println("Success.");
    } else {
      Serial.print("Failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("The connection to the Twitter Arduino proxy failed.");
  }

}
