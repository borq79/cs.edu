#define NUM_BUTTONS       4
#define BUTTON_ONE_PIN    6
#define BUTTON_TWO_PIN    8
#define BUTTON_THREE_PIN  10
#define BUTTON_FOUR_PIN   12

#define NEO_PIXEL_PIN          2
#define SIZE_OF_NEO_PIXEL_BAR  8

#define MAX_SEQUENCE       5
#define DISPLAY_PAUSE      200
#define BUZZER_PIN         13

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

