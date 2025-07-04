#ifndef MONITORING_H
#define MONITORING_H

// Define number pin for ESP32
#define DHT_PIN 15

// Setup monitoring
void setupMonitoring();

// Setup DHTesp
void setupDHTesp();

// Get current temperature °С by DHTesp 
float getCurrentTemperatureC();

// Get current temperature °F by DHTesp 
float getCurrentTemperatureF();

// Get current humidity by DHTesp
float getCurrentHumidity();

// Init WiFi connection 
void wifiConnection();

#endif
