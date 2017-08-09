void setup() {
  Serial.begin(9600);
}

void loop() {
  printHello();
  
  Serial.print("Meaning of Life: ");
  Serial.println(getMeaningOfLife());

  Serial.print("The largest value between 12 and 32 is: ");
  Serial.println(getMaximumNumber(12, 32));
}

void printHello() {
  Serial.println("Hello");
}

int getMeaningOfLife() {
  return 42;
}

int getMaximumNumber(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

