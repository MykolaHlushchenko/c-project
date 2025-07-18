#include <Arduino.h>
#include <Radar.h>

void setup() {
  Serial.begin(115200);
  wifiConnection();
  setupMotor();
  setupServerRequest();
  setupRadarPins();
    
   xTaskCreatePinnedToCore(
    motorTask,
    "Motor Task",
    4096,            
    NULL,            
    1,               
    NULL,            
    0                // use core 0 for avoid problem when wifi enabled
  );
}

void loop() {
  // do something
}
