#ifndef RADAR_H
#define RADAR_H

#define STEPS_MOTOR 2048

#define IN1 13
#define IN2 14
#define IN3 27
#define IN4 26

const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

// Set default values for motor
void setupMotor();

// Create access point to wifi
void wifiConnection();

// Setup server request 
void setupServerRequest();

// Setup radar(HC-SR04) pins 
void setupRadarPins();

// Get distance to object in cm
long getDistance();

// Task motor
void motorTask(void* pvParameters);

#endif
