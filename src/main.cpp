#include <Arduino.h>
#include <WiFi.h>

void setup() {
  // Setup Wifi scanner
  Serial.begin(115200);

  // Enable Wifi station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup mode DONE");
}

void loop() {
  Serial.print("\r");
  // Search in a Loop wifi networks

   Serial.println("Scan start");
   delay(5000);

   int count = WiFi.scanNetworks();

   Serial.println("Scan done");
   if (count == 0) {
      Serial.println("Any networks found !");
   } else {
      Serial.print(count);

      Serial.println(" Networks found ");

      for (int i = 0; i < count; ++i)
      {
        // Show Server Set Identifier
        Serial.print("SSID [");
        Serial.print(WiFi.SSID(i));
        Serial.println("]");

        // Show Received Signal Strengh Indicator
        Serial.print("RSSI [");
        Serial.print(WiFi.RSSI(i));
        Serial.println("]");

        // Show MAC address
        Serial.print("BSSID [");
        Serial.print(WiFi.BSSIDstr(i));
        Serial.println("]");

        // Show Encryption
        Serial.print("ENCRIPTION [");
        Serial.print(WiFi.encryptionType(i));
        Serial.println("]");
        Serial.println();
        delay(10);
      }
    }

    WiFi.scanDelete();
    delay(5000);
    Serial.println();
}
