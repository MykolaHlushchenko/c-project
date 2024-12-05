#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>


WebServer server(80);

const bool AUTO = false;
const int LED_PIN = 2;
const char* SSID = "******";
const char* PASSWORD = "******";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
  <html>
    <head>
      <title>ESP Input Form</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        * {
          box-sizing: border-box;
          body {
            background-color: #cccfff;
            .container {
              border: 2px solid #524f4f;
              padding: 10px;
              display: grid;
              justify-content: center;
              margin: 0 auto;
              width: 50%;

              .header-container h3 {
                font: 18px Arial, sans-serif;
              }

              .button {
                background: #4a666b;
                border: none;
                width: 200px;
                height: 50px;
                opacity: 0.6;
                transition-duration: 0.4s;
                cursor: pointer;
                text-transform: uppercase;
                font: 14px Arial, sans-serif;
              }

              .button:hover {
                opacity: 1;
                color: #ffffff;
              }

              .slider-container {
                p {
                  text-transform: uppercase;
                  font: 14px Arial, sans-serif;
                }

                .slider {
                  -webkit-appearance: none;
                  width: 100%;
                  opacity: 0.6;
                  outline: none;
                  height: 25px;
                  background: #ffffff;
                  -webkit-transition: .2s;
                  transition: opacity .2s;
                }

                .slider::-webkit-slider-thumb {
                  -webkit-appearance: none;
                  appearance: none;
                  width: 25px;
                  height: 25px;
                  background: #4a666b;
                  cursor: pointer;
                }

                .slider:hover {
                  opacity: 1;
                }
              }    
            }
          }
        }
      </style>
    </head>
  <body>

    <div class="container">
      <div class="header-container">
        <h3>ESP32 LED WEB SERVER</h3>
      </div>
      
      <div class="button-container">
        <button class="button" onclick="controlLed('enable')">Enable</button>
        <button class="button" onclick="controlLed('disable')">Disable</button>
         <button class="button" onclick="controlLed('auto')">Auto</button>
      </div>

      <div class="slider-container">
        <p>Custom range slider:</p>
        <input type="range" onchange="updateSliderPWM(this)" min="0" max="255" value="0" step="1" class="slider" id="myRange">
        <p>VALUE: <span id="currentValue"></span></p>
      </div>
    </div>


    <script>
      alert('Test ESP32');
      document.getElementById("currentValue").innerHTML = 0;

      function controlLed(action) {
        fetch('/' + action);
        var slider = document.getElementById("myRange");
       
        if (action == "disable") {
          slider.value = 0;
          document.getElementById("currentValue").innerHTML = slider.value;
        }

        if (action == "enable") {
          slider.value = 255;
          document.getElementById("currentValue").innerHTML = slider.value; 
        }

        if (action == "auto") {
          slider.value = 128;
          document.getElementById("currentValue").innerHTML = slider.value; 
          var xhr = new XMLHttpRequest();
          xhr.open("GET", "/auto?valueAuto="+1, true);
          xhr.send();
        }
      }

      function updateSliderPWM(element) {
        var sliderValue = document.getElementById("myRange").value;
        document.getElementById("currentValue").innerHTML = sliderValue;
        console.log(sliderValue);
        var xhr = new XMLHttpRequest();
          xhr.open("GET", "/slider?value="+sliderValue, true);
          xhr.send();
      }
    </script>
  </body>
  </html>)rawliteral";

void ledBrightnes();
void wifiConnection();
void handleRoot() {
  server.send(200, "text/html", index_html);
}

void enableLed() {
  server.send(200, "text/plain", "LED ON");
  analogWrite(LED_PIN, 255);
}

void disableLed() {
  server.send(200, "text/plain", "LED OFF");
  analogWrite(LED_PIN, 0);
}

void sliderRange() {
  if (server.hasArg("value")) {
    analogWrite(LED_PIN, server.arg("value").toInt());
    server.send(200, "text/plain", "SLIDER OK");
  }
}

void autoStart() {
  server.send(200, "text/plain", "AUTO ON");
  //ledBrightnes();
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("----- ESP32 Program -----");
  wifiConnection();
}

void loop() {
  
  if (server.hasArg("valueAuto")) {
    ledBrightnes();
  }  
  
  server.handleClient();
}

void ledBrightnes() {
      
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {   
    // changing the LED brightness with PWM
    analogWrite(LED_PIN, dutyCycle);
    delay(2);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    // changing the LED brightness with PWM
    analogWrite(LED_PIN, dutyCycle);
    delay(2);
  }
}

// wifi Connection method
void wifiConnection () {
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Wi connection...");

  }
  Serial.println("Connected to wifi");
  Serial.print("IP Address");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/enable", enableLed);
  server.on("/disable", disableLed);
  server.on("/slider", sliderRange);
  server.on("/auto", autoStart);
  server.begin();

}
