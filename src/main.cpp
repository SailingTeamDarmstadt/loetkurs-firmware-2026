#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESP32Servo.h>
#include <esp_wifi.h>

#include "htmlPage.hpp"

WebServer server;

constexpr int SERVO1 = 3;
constexpr int SERVO2 = 4;
constexpr int SERVO3 = 5;

const char ssid_p1[] = "ServoControl";
char ssid[sizeof(ssid_p1) + 1 + 6*2 + 5];

DNSServer dnsServer;

ESP32PWM pwm;
Servo servo1;
Servo servo2;
constexpr int servoFreq = 50;
constexpr int minUS = 1000;
constexpr int maxUS = 2000;

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleControl() {
  if (server.hasArg("servo") && server.hasArg("value")) {
    int servoNum = server.arg("servo").toInt();
    int value = server.arg("value").toInt();
    
    Servo *servoPtr;
    if (servoNum == 1) {
      servoPtr = &servo1;
    } else if (servoNum == 2){
      servoPtr = &servo2;
    } else {
      server.send(400, "text/plain", "Bad Request");
      return;
    }
    
    servoPtr->write(value);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleNotFound() {
  // Redirect all unknown requests to root (captive portal behavior)
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void setup() {

  Serial.println("Init");
  // allow for more than one servo board in the same spot
  // uint8_t macAddr[6];
  // esp_wifi_get_mac(WIFI_IF_AP, macAddr);

  // snprintf(ssid, sizeof(ssid), "%s_%02x:%02x:%02x:%02x:%02x:%02x",
  //          macAddr[0], macAddr[1], macAddr[2],
  //          macAddr[3], macAddr[4], macAddr[5]);
  // TODO: hack
  snprintf(ssid, 10, "esp_wifi");

  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
  servo1.setPeriodHertz(servoFreq);
  servo1.attach(SERVO1, minUS, maxUS);
  servo1.write(90);
  servo2.setPeriodHertz(servoFreq);
  servo2.attach(SERVO2, minUS, maxUS);
  servo2.write(90);

  Serial.println("Setting up AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, "");

  Serial.println("Setting up DNS");
  dnsServer.start(53, "*", WiFi.softAPIP());
  Serial.println("Setting up HTTP");
  server.begin(80);
  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.onNotFound(handleNotFound);
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}

