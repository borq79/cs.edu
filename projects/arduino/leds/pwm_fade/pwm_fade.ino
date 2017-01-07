#define LED_PIN 3

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);
}


uint8_t pwmDutyCycle = 0;
int8_t addr = 50;

void loop() {

  analogWrite(LED_PIN, pwmDutyCycle);
  delay(1000); 

  pwmDutyCycle += addr;

  if (pwmDutyCycle == 250 || pwmDutyCycle == 0) {
    addr *= -1;
  }
}
