#define MAX_ALARM_TIME      5000
#define ALARM               3
#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters).
#define DOOR_WIDTH   71  // Door Width in centimeters (28" Door)
// LED Light Strand Pin
#define LED_PIN      2
#define NUM_OF_PIXELS 10
#define ALARM_SETUP_TIME_WINDOW 15000

struct COLOR {
  byte r;
  byte g;
  byte b;
};

#define RED    {0x7F, 0x00, 0x00} 
#define GREEN  {0x00, 0x7F, 0x00}
#define BLUE   {0x00, 0x00, 0x7F}
#define PURPLE {0x7F, 0x00, 0x7F}
#define ORANGE {0x7F, 0x52, 0x00}
#define WHITE  {0x7F, 0x7F, 0x7F}
#define YELLOW {0x79, 0x7A, 0x05}
#define INDIGO {0x20, 0x05, 0x7A}

#define NUM_OF_COLORS 8

#define NOTE_H   0
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978


// Borrowed these note arrays from https://github.com/tsukisan/Arduino/tree/master/WiiClassicSoundboard
const uint16_t theme[] = {11,                                                  // Array for Theme song
  NOTE_E4, 8, NOTE_E4, 8, NOTE_H, 8, NOTE_E4, 8, NOTE_H, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_H, 8, NOTE_G4, 8, NOTE_H, 3, NOTE_G3, 8};
const uint16_t life[] = {6,                                                    // Array for 1-up sound effect
  NOTE_E5, 10, NOTE_G5, 10, NOTE_E6, 10, NOTE_C6, 10, NOTE_D6, 10, NOTE_G6, 10};
const uint16_t flagpole[] = {27,                                               // Array for Flag pole sound effect & song
  NOTE_G2, 10, NOTE_C3, 10, NOTE_E3, 10, NOTE_G3, 10, NOTE_C4, 10, NOTE_E4, 10, NOTE_G4, 3, NOTE_E4, 3, NOTE_GS2, 10, NOTE_C3, 10, 
  NOTE_DS3, 10, NOTE_GS3, 10, NOTE_C4, 10, NOTE_DS4, 10, NOTE_GS4, 3, NOTE_DS4, 3, NOTE_AS2, 10, NOTE_D3, 10, NOTE_F3, 10, 
  NOTE_AS3, 10, NOTE_D4, 10, NOTE_F4, 10, NOTE_AS4, 3, NOTE_B4, 10, NOTE_B4, 10, NOTE_B4, 10, NOTE_C5, 2};
const uint16_t death[] = {17,                                                  // Array for Death sound effect & song
  NOTE_C4, 32, NOTE_CS4, 32, NOTE_D4, 16, NOTE_H, 4, NOTE_H, 2, NOTE_B3, 8, NOTE_F4, 8, NOTE_H, 8, NOTE_F4, 8, NOTE_F4, 6, 
  NOTE_E4, 6, NOTE_D4, 6, NOTE_C4, 8, NOTE_E3, 8, NOTE_H, 8, NOTE_E3, 8, NOTE_C3, 8};
const uint16_t gameover[] = {15,                                               // Array for Game over song
  NOTE_C4, 8, NOTE_H, 8, NOTE_H, 8, NOTE_G3, 8, NOTE_H, 4, NOTE_E3, 4, NOTE_A3, 6, NOTE_B3, 6, NOTE_A3, 6, NOTE_GS3, 6, NOTE_AS3, 6, 
  NOTE_GS3, 6, NOTE_G3, 8, NOTE_F3, 8, NOTE_G3, 4};
