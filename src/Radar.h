#ifndef RADAR_H
#define RADAR_H

#define STEPS_MOTOR 2048

#define IN1 13
#define IN2 14
#define IN3 27
#define IN4 26

const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

void setupMotor();
void wifiConnection();
void setupServerRequest();
void setupRadarPins();
long getDistance();
void motorTask(void* pvParameters);

#endif