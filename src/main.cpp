#include <Arduino.h>
#include <Adafruit_LEDBackpack.h>

void drawDiagonals();
void drawLine (int number, bool isHorisontal);
void drawMatrix();

void drawPixelXY(uint8_t x, uint8_t y);

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

const uint8_t xMap[8] = {0, 7, 6, 5, 4, 3, 2, 1};

void setup() {
  Serial.begin(115200);
  matrix.begin(0x70);
  matrix.setBrightness(10);

  drawPixelXY(0, 0);
  drawPixelXY(7, 0);
  drawPixelXY(0, 7);
  drawPixelXY(7, 7);
  
  matrix.writeDisplay();
  delay(5000);
}

void loop() {
  drawLine(0, true);
  drawLine(7, false);
  drawLine(7, true);
  drawLine(0, false);

  drawDiagonals();
  drawMatrix();
}

void drawPixelXY(uint8_t x, uint8_t y) {
  if (x > 7 || y > 7) {
    return;
  }
  matrix.drawPixel(xMap[x], y, LED_ON);
}

void drawLine (int number, bool isHorisontal) {
  matrix.clear();
  
  for (uint8_t i = 0; i < 8; i++) {
    if (isHorisontal) {
      drawPixelXY(number, i);  
    } else {
      drawPixelXY(i, number); 
    } 
  }
   
  matrix.writeDisplay();
  delay(1000);
}

void drawDiagonals() {
  matrix.clear();
  for (uint8_t i = 0; i < 8; i++) {
        drawPixelXY(7 - i, i);
        drawPixelXY(i, i);
  }
  matrix.writeDisplay();
  delay(1000);  
}

void drawMatrix() {
  matrix.clear();
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = 0; j < 8; j++) {
      if (i % 2 == 0 && j % 2 == 00) {
        drawPixelXY(i, j);  
      }  
      
    }
  }
  matrix.writeDisplay();
  delay(1000);
}
