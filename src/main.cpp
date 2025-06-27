#include <Arduino.h>
#include <lcd128_32_io.h>

const int LM35_pin = 34;
const int SDA_pin = 21;
const int SCL_pin = 22;

// Get LCD object 
lcd Lcd(SDA_pin, SCL_pin);

void initLcd();
float getTemperatureLM35();
void displayMaxTemperute(float currentTemperature);

void setup() {
  Serial.begin(115200);
  initLcd();
}

float maxTemerature(0.00);

void loop() {
  Lcd.Cursor(2, 0);
  char temp[32];
  float currentTemperature = getTemperatureLM35();
  
  sprintf(temp, "Temp: %.2f C", currentTemperature);
  Lcd.Display(temp);

  displayMaxTemperute(currentTemperature);
 
  Serial.print("Current temperature: ");
  Serial.println(currentTemperature);
  
  delay(2000);
}

// Initialise LCD 128x32 dots
void initLcd() {
  Lcd.Init();   
  Lcd.Clear(); 
  Lcd.Cursor(0, 0);
  Lcd.Display("ESP WROOM 32");
  Lcd.Cursor(1, 0);
  Lcd.Display("LCD Display 128x32");
}

// Get current temperature from LM35
float getTemperatureLM35() {
  int adcVal = analogRead(LM35_pin);
  float voltage = (adcVal / 4095.0) * 3.3;
  float temperature = voltage * 100.0;
  
  return temperature;
}

// Display max temperature
void displayMaxTemperute(float currentTemperature) {
  Lcd.Cursor(3, 0);
  char maxTemp[32];

  if (currentTemperature > maxTemerature) {
    maxTemerature = currentTemperature;
    sprintf(maxTemp, "Max: %.2f C", maxTemerature);
    Lcd.Display(maxTemp);
  }
}
