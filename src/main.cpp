#include <Arduino.h>
#include <Monitoring.h>

// Main setup function
void setup() {
  setupMonitoring();
  setupDHTesp();
  wifiConnection();
}

// Main loop
void loop() {
  // do something
}
