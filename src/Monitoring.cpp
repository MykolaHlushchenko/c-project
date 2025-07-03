#include <Monitoring.h>
#include <Arduino.h>
#include <DHTesp.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <env.h>

// Define number pin for ESP32
#define DHT_PIN 15

DHTesp dhtSensor;
AsyncWebServer server(80);

// Setup monitoring
void setupMonitoring() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Weather monitoring");
}

// Setup DHTesp
void setupDHTesp() {
    dhtSensor.setup(DHT_PIN, DHTesp::DHT11);
    Serial.println("Setup DHTesp");
}

// Get current temperature °С by DHTesp 
float getCurrentTemperatureC() {
    return dhtSensor.getTemperature();
}

// Get current temperature °F by DHTesp 
float getCurrentTemperatureF() {
  return getCurrentTemperatureC() * 9.0 / 5.0 + 32.0;
}

// Get current humidity by DHTesp
float getCurrentHumidity() {
    return dhtSensor.getHumidity();
}

// Html variable
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head><title>ESP32 Test</title></head>
  <link rel="stylesheet" href="/style.css">
  <body>
    <h1>Weather Monitoring</h1>
    <hr class="separator">
    <p>Temperature : <span id="tempC"></span> &degC</p>
    <p>Temperature : <span id="tempF"></span> &degF</p>
    <p>Humidity: <span id="hum"></span> %</p>
  </body>
  <script>
      async function fetchData() {
        const response = await fetch('/data');
        const data = await response.json();
        let temperature = data.temperatureC;
        document.getElementById('tempC').textContent = temperature;
        document.getElementById('tempF').textContent = data.temperatureF;
        document.getElementById('hum').textContent = data.humidity;
        
        let body = document.querySelector("body");

        if (temperature <= -20) {
          body.style.backgroundColor = "#5499c7";
        } else if (temperature <= -10) {
          body.style.backgroundColor = "#5dade2";
        } else if (temperature <= 0) {
          body.style.backgroundColor = "#cacfd2";
        } else if (temperature <= 10) {
          body.style.backgroundColor = "#f4d03f";
        } else if (temperature <= 20) {
          body.style.backgroundColor = "#f5b041";
        } else if (temperature <= 30) {
          body.style.backgroundColor = "#eb984e";
        } else {
          body.style.backgroundColor = "#dc7633";
        } 
      }
      setInterval(fetchData, 2000);
      window.onload = fetchData;
    </script>
</html>
)rawliteral";

// Style css variable
const char style_css[] PROGMEM = R"rawliteral(
body {
  background-color: #cacfd2;
  font-family: sans-serif;
  text-align: center;
  padding-top: 50px;
  p, span {
    font-size: 40px;
    font-weight: 800;
    color: #FFFFFF;
  }
  hr.separator {
    border: 2px dotted #424949;
  }  
}
h1 {
  color: #424949;
  font-weight: bold;
  font-size: 48px;
}
)rawliteral";

// Init WiFi connection
void wifiConnection() {
  Serial.begin(115200);
  WiFi.softAP(SSID_LOGIN, PASSWORD);
  Serial.println(WiFi.softAPIP());
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/css", style_css);
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"temperatureC\":" + (String)getCurrentTemperatureC() 
      + ", \"temperatureF\":" + (String)getCurrentTemperatureF() 
      + ", \"humidity\":" + (String)getCurrentHumidity() +"}";
    request->send(200, "application/json", json);
  });

  server.begin();
  Serial.println("Server started");
} 