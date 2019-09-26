#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "IPGeolocation.h"

#define DEBUG

// Replace with your wifi credentials
const char * ssid = "***********";
const char * password = "***********";

// Get an API Key by registering on
// https://ipgeolocation.io

String Key = "*************";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed");
      while(1) {
          delay(1000);
      }
  }
}

void loop() {
  IPGeolocation location(Key);
  IPGeo IPG;
  location.updateStatus(&IPG);
  Serial.println(IPG.city);
  Serial.println(IPG.country);
  Serial.println(IPG.country_code);
  Serial.println(IPG.tz);
  Serial.println(IPG.offset);

  delay(600000);       // Wait for 600 seconds
  // put your main code here, to run repeatedly:
}
