#define LED_PIN 3
#define DUTY_CYCLE_START 5

int newDutyCycle = DUTY_CYCLE_START;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  newDutyCycle += 10;

  if (newDutyCycle > 255) {
    newDutyCycle = DUTY_CYCLE_START;
  }
  
  Serial.print("Setting Duty Cycle to: ");
  Serial.println(newDutyCycle);
  
  analogWrite(LED_PIN, newDutyCycle);

  delay(2000);
  
}
