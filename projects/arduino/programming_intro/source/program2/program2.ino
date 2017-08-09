#define READ_PIN A0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(READ_PIN, INPUT);
  
}

void loop() {
  int brightness = analogRead(READ_PIN);
  
  Serial.print("Light Intensity is: ");
  Serial.println(brightness);
  
  delay(2000);
  
}
