#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "IPGeolocation.h"

const char fingerprint[] PROGMEM = "AC 06 70 3C 86 04 60 22 06 BE E5 11 A5 37 DB 7D 86 92 4E 1C"; // fingerprint

const size_t capacity = JSON_OBJECT_SIZE(10) + JSON_OBJECT_SIZE(17) + 580;
String line;
const char *host = "api.ipgeolocation.io";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
WiFiClientSecure httpsClient;

IPGeolocation::IPGeolocation(String Key){
  _Key = Key;
}

void IPGeolocation::updateStatus(){
  //httpsClient.setFingerprint(fingerprint);
  httpsClient.setInsecure();
  httpsClient.setTimeout(15000); // 15 Seconds
  delay(1000);

  //Serial.print("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }

  String Link;

  //GET Data
  Link = "/timezone?apiKey="+_Key;

  Serial.print("requesting URL: ");
  Serial.println(host+Link);

  httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      Serial.println(line);
      break;
    }
  }

  Serial.println("reply was:");
  Serial.println("==========");
  httpsClient.readStringUntil('\n'); // The API sends an extra line with just a number. This breaks the JSON parsing, hence an extra read
  while(httpsClient.connected()){
    line = httpsClient.readString();
    Serial.println(line); //Print response
  }
  DynamicJsonDocument doc(capacity);
  _Response = line;
  deserializeJson(doc, line);
  _TimeZone = doc["timezone"].as<String>();
  Serial.print("Time Zone: ");
  Serial.println(_TimeZone);
  int dst_savings = doc["dst_savings"].as<int>();
  int timezone_offset = doc["timezone_offset"];
  _offset=dst_savings + timezone_offset;
  Serial.print("DST Savings: ");
  Serial.println(dst_savings);
}

String IPGeolocation::getTimezone(){
  return _TimeZone;
}
int IPGeolocation::getOffset(){
  return _offset;
}

String IPGeolocation::getResponse(){
  return _Response;
}
