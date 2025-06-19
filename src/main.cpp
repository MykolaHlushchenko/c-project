#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial MySerial(2);
int number = 0;

void setup() {
  Serial.begin(115200);
  MySerial.begin(9600, SERIAL_8N1, 16, 17); // RX, TX

  Serial.println("ESP32-A: Ready Ok");
  
}

void loop() {
 
  String message = "Hello from ESP32 A -> " +  String(number);
  MySerial.println(message);
  Serial.println("Sended: " + message);
  number++;
  delay(2000);
}