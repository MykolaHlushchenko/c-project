#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Start monitoring programm");
}

void loop() {
  float temp = random(200, 300) / 10.0;
  Serial.print("Temperature ");
  Serial.print(temp);
  Serial.println("C");

  delay(2000);
}