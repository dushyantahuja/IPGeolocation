#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "IPGeolocation.h"

// Replace with your wifi credentials
const char * ssid = "***********";
const char * password = "***********";

// Get and API Key by registering on
// https://ipgeolocation.io



String Key = "*************";


IPGeolocation location(Key);


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
  location.updateStatus();
  Serial.println(location.getTimezone());
  Serial.println(location.getOffset());
  Serial.println(location.getResponse());
  delay(600000);       // Wait for 600 seconds
  // put your main code here, to run repeatedly:
}
