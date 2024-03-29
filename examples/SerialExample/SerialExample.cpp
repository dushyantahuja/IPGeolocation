#include <Arduino.h>
#include <ESP8266WiFi.h>

#define DEBUG
#include "IPGeolocation.h"


// Replace with your wifi credentials
const char * ssid = "****";
const char * password = "****";

// Get an API Key by registering on
// https://ipgeolocation.io
// OR
// https://app.abstractapi.com/api/ip-geolocation/


String Key = "****";

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
  IPGeolocation location(Key,"ABSTRACT");
  IPGeo IPG;
  location.updateStatus(&IPG);
  Serial.println(IPG.city);
  Serial.println(IPG.country);
  Serial.println(IPG.country_code);
  Serial.println(IPG.tz);
  Serial.println(IPG.offset);
  Serial.println(location.getResponse());
}

void loop() {
  
}