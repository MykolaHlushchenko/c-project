#include <Arduino.h>
#include <lcd128_32_io.h>

const int LM35_pin = 34;
const int SDA_pin = 21;
const int SCL_pin = 22;

// Get LCD object 
lcd Lcd(SDA_pin, SCL_pin);

void initLcd();
float getTemperatureLM35();

void setup() {
  Serial.begin(115200);
  initLcd();
}

void loop() {
  Lcd.Cursor(3, 0);
  char temp[32];
  sprintf(temp, "Temp: %.2f C", getTemperatureLM35());
  Lcd.Display(temp);

  Serial.print("Current temperature: ");
  Serial.println(getTemperatureLM35());
  
  delay(1000);
}

// Initialise LCD 128x32 dots
void initLcd() {
  Lcd.Init();   
  Lcd.Clear(); 
  Lcd.Cursor(0, 0);
  Lcd.Display("ESP WROOM 32");
  Lcd.Cursor(1, 0);
  Lcd.Display("LCD Display 128x32");
  Lcd.Cursor(2, 0);
  Lcd.Display("Temperature");
}

// Get current temperature from LM35
float getTemperatureLM35() {
  int adcVal = analogRead(LM35_pin);
  float voltage = (adcVal / 4095.0) * 3.3;
  float temperature = voltage * 100.0;
  
  return temperature;
}