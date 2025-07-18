#include <AccelStepper.h>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <env.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <Radar.h>

AccelStepper stepper(AccelStepper::FULL4WIRE, IN1, IN3, IN2, IN4);

AsyncWebServer server(80);

bool motorActive = false;
int direction = 1;
int angle = 0;
// // Поточний напрямок
// bool forward = true;

// Set default values for motor
void setupMotor() {
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(100);
  stepper.moveTo(STEPS_MOTOR/2);
}

// Create access point to wifi
void wifiConnection() {
  Serial.begin(115200);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS init failed.");
    return;
  }

  WiFi.softAP(SSID_LOGIN, PASSWORD);
  Serial.println(WiFi.softAPIP());
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server.begin();
  Serial.println("Server started");  
}

void setupServerRequest() {
      server.on("/motor", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      String state = request->getParam("value")->value();
      if (state == "on") {
        motorActive = true;
      } else if (state == "off") {
        motorActive = false;
      }
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    int counter = 0;
    float distance = getDistance();
    counter = round(distance);
    int angle = map(stepper.currentPosition(), 0, 1024, 0, 180);
    String json = "{\"value\":" + String(counter) + ", \"angle\":" + String(angle) + "}";
    request->send(200, "application/json", json);
  });
}

void setupRadarPins() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

// Get distance to object in cm
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;

  //Serial.print("Distance: ");
  //Serial.print(distance);
  //Serial.println(" cm");

  if (distance > 300) {
    return 300;
  }
  
  return distance;
}

// TASK
void motorTask(void* pvParameters) {
  while (true) {
    if (motorActive) {
      if (stepper.distanceToGo() == 0) {
        direction = !direction;
        stepper.moveTo(direction ? 1024 : 0);
      }  
      stepper.run(); 
    } else {
      stepper.stop();
      stepper.setCurrentPosition(stepper.currentPosition());
      stepper.moveTo(stepper.currentPosition());
      stepper.run();  
    }    

    vTaskDelay(1);
  }
}