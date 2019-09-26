#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "IPGeolocation.h"

//const char fingerprint[] PROGMEM = "AC 06 70 3C 86 04 60 22 06 BE E5 11 A5 37 DB 7D 86 92 4E 1C"; // fingerprint not required anymore as Insecure connection used

const size_t capacity = JSON_OBJECT_SIZE(10) + JSON_OBJECT_SIZE(17) + 580;
String line;
const char *host = "api.ipgeolocation.io";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
WiFiClientSecure httpsClient;

IPGeolocation::IPGeolocation(String Key){
  _Key = Key;
}

void IPGeolocation::updateStatus(IPGeo *I){
  //httpsClient.setFingerprint(fingerprint);
  httpsClient.setInsecure();
  httpsClient.setTimeout(15000); // 15 Seconds
  delay(1000);

  DEBUG_PRINT("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      DEBUG_PRINT(".");
      r++;
  }

  String Link;

  //GET Data
  Link = "/timezone?apiKey="+_Key;

  DEBUG_PRINT("requesting URL: ");
  DEBUG_PRINT(host+Link);

  httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  DEBUG_PRINT("request sent");

  while (httpsClient.connected()) {
    String _Response = httpsClient.readStringUntil('\n');
    if (_Response == "\r") {
      DEBUG_PRINT("headers received");
      DEBUG_PRINT(_Response);
      break;
    }
  }
  DEBUG_PRINT("reply was:");
  DEBUG_PRINT("==========");
  httpsClient.readStringUntil('\n'); // The API sends an extra line with just a number. This breaks the JSON parsing, hence an extra read
  while(httpsClient.connected()){
    _Response = httpsClient.readString();
    DEBUG_PRINT(_Response); //Print response
  }
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, _Response);
  I->tz = doc["timezone"].as<String>();
  int dst_savings = doc["dst_savings"].as<int>();
  int timezone_offset = doc["timezone_offset"];
  I->offset=dst_savings + timezone_offset;
  I->country = doc["country_name"].as<String>();
  I->country_code = doc["country_code2"].as<String>();
  I->city = doc["city"].as<String>();
  
  DEBUG_PRINT("Time Zone: ");
  DEBUG_PRINT(I->tz);
  DEBUG_PRINT("DST Savings: ");
  DEBUG_PRINT(dst_savings);
}

