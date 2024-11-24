#include <Arduino.h>

const int ledPin = 2;

void setup() {
  // put your setup code here, to run once:
  //int result = myFunction(2, 3);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    delay(200);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    delay(200);
  }
}