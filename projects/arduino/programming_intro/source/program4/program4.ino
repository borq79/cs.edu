#include <DHT.h>

#define DHT_PIN 2


DHT dht(DHT_PIN, DHT11);
void setup() {
    Serial.begin(9600);
    
    dht.begin();
}

void loop() {
  delay(200);

  float h = dht.readHumidity();
  float t = dht.readTemperature(true);

  Serial.print(t);
  Serial.print(" ");
  Serial.println(h);
}
