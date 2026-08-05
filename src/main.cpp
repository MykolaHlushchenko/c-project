#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <TinyGPS++.h>

const char* ap_ssid = "ESP32-GPS-Tracker";
const char* ap_password = "Password123";

// Апаратні піни UART2 вашої ESP32-WROOM-32
#define RXD2 16
#define TXD2 17
#define GPS_BAUDRATE 9600

TinyGPSPlus gps;
WebServer server(80);
DNSServer dnsServer;

const byte DNS_PORT = 53;
unsigned long lastSerialPrint = 0;

void handleRoot();
void processGps();
void printToSerial();

void setup() {
  Serial.begin(115200);
  delay(2000); 
  
  Serial.println("\n=========================================");
  Serial.println("[START SCAN]");
  Serial.println("=========================================");

    Serial2.begin(GPS_BAUDRATE, SERIAL_8N1, RXD2, TXD2);
  delay(500);

  // Configure access point
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  WiFi.setTxPower(WIFI_POWER_11dBm); 
  delay(500);

  if(WiFi.softAP(ap_ssid, ap_password)) {
    Serial.println("-> Wi-Fi мережу створено успішно!");
  }

  dnsServer.start(DNS_PORT, "*", IPAddress(192, 168, 4, 1));
  Serial.println("-> DNS-сервер запущено.");

  server.on("/", handleRoot);
  server.onNotFound([]() {
    server.sendHeader("Location", String("http://192.168.4"), true);
    server.send(302, "text/plain", "");
  });

  server.begin();
  Serial.println("-> WEB-server is running.");
  Serial.println("=========================================\n");
}

void loop() {
  dnsServer.processNextRequest();
  processGps();
  server.handleClient();

  // Show info in seral monitor 
  if (millis() - lastSerialPrint > 3000) {
    printToSerial();
    lastSerialPrint = millis();
  }
}

// Read bytes from GPS
void processGps() {
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }
}

// show data in seral monitor
void printToSerial() {
  Serial.println("--- [DEBUG SERIAL] ---");
  Serial.print("Bytes from GPS: ");
  Serial.println(gps.charsProcessed());
  
  Serial.print("Satellite count: ");
  Serial.println(gps.satellites.value());

  if (gps.location.isValid()) {
    Serial.print("Coordinates: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Speed: "); Serial.print(gps.speed.kmph()); Serial.println(" km/h");
  } else {
    Serial.println("Статус GPS: Сигнал слабкий, пошук супутників під відкритим небом...");
  }
  
  // Якщо за 5 секунд не прийшло жодного байта — б'ємо на сполох
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("🚨 ПОМИЛКА: Дані від GPS модуля взагалі НЕ надходять на ESP32! Перевірте дроти TX/RX.");
  }
  Serial.println("----------------------\n");
}

// ФУНКЦІЯ 2: Виведення інформації у браузер смартфона
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<meta http-equiv='refresh' content='3'>";
  html += "<title>GPS</title>";
  html += "<style>body{font-family:sans-serif; text-align:center; background:#1e1e24; color:#fff; padding:20px;}";
  html += ".box{background:#2a2a35; padding:25px; border-radius:15px; display:inline-block; border:1px solid #444; max-width:90%;}";
  html += ".geo{font-size:1.3em; color:#00ff88; font-family:monospace; background:#111; padding:10px; border-radius:5px; margin:15px 0; word-break:break-all;}";
  html += ".hint{font-size:0.85em; color:#aaa; line-height:1.4;} </style>";
  html += "</head><body>";
  
  html += "<div class='box'>";
  html += "<h2>📡 Standalone GPS Monitor</h2>";
  html += "<p>🟢 Satellites in view: <b>" + String(gps.satellites.value()) + "</b></p>";
  html += "<p style='font-size:0.8em; color:#888;'>Processed symbols from GPS: " + String(gps.charsProcessed()) + "</p>";

  if (gps.location.isValid()) {
    String latStr = String(gps.location.lat(), 6);
    String lngStr = String(gps.location.lng(), 6);
    String coords = latStr + "," + lngStr;
    
    html += "<p>Copy these coordinates for offline maps:</p>";
    html += "<div class='geo'>" + coords + "</div>";
    html += "<p><b>Speed:</b> " + String(gps.speed.kmph(), 1) + "  km/h</p>";
    html += "<p><b>Altitude:</b> " + String(gps.altitude.meters(), 1) + " m</p>";
  } else {
    html += "<p style='color:#ff5555; font-weight:bold;'>Searching for satellites...</p>";
    html += "<p class='hint'>Take the ceramic antenna outside or on a windowsill with the brown side facing the sky.</p>";
  }
  
  html += "</div></body></html>";
  server.send(200, "text/html", html);
}