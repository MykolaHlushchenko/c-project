#include <Arduino.h>
#include <DHTesp.h>

#define DHT_PIN 15

DHTesp dht;

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.setup(DHT_PIN, DHTesp::DHT11);
  Serial.println("DHT11 ready");
  Serial.println("Humidity Temperature");
}

void loop() {
  delay(dht.getMinimumSamplingPeriod());
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();

  Serial.print("\r                             ");
  Serial.print("\rT: ");
  Serial.print(temperature);
  Serial.print("°C  H: ");
  Serial.print(humidity);
  Serial.print("%");
}
