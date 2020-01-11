#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "IPGeolocation.h"

//const char fingerprint[] PROGMEM = "AC 06 70 3C 86 04 60 22 06 BE E5 11 A5 37 DB 7D 86 92 4E 1C"; // fingerprint not required anymore as Insecure connection used


IPGeolocation::IPGeolocation(String Key){
  _Key = Key;
}

String IPGeolocation::getResponse(){
  return _Response;
}

void IPGeolocation::updateStatus(IPGeo *I){
  const char *host = "api.ipgeolocation.io";
  const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
  WiFiClientSecure httpsClient;
  const size_t capacity = JSON_OBJECT_SIZE(10) + JSON_OBJECT_SIZE(17) + 580;
  //httpsClient.setFingerprint(fingerprint);
  httpsClient.setInsecure();
  httpsClient.setTimeout(15000); // 15 Seconds
  delay(1000);

  DEBUGPRINT("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      DEBUGPRINT(".");
      r++;
  }

  String Link;

  //GET Data
  Link = "/timezone?apiKey="+_Key;

  DEBUGPRINT("requesting URL: ");
  DEBUGPRINT(host+Link);

  httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  DEBUGPRINT("request sent");

  while (httpsClient.connected()) {
    String _Response = httpsClient.readStringUntil('\n');
    if (_Response == "\r") {
      DEBUGPRINT("headers received");
      DEBUGPRINT(_Response);
      break;
    }
  }
  DEBUGPRINT("reply was:");
  DEBUGPRINT("==========");
  httpsClient.readStringUntil('\n'); // The API sends an extra line with just a number. This breaks the JSON parsing, hence an extra read
  while(httpsClient.connected()){
    _Response = httpsClient.readString();
    DEBUGPRINT(_Response); //Print response
  }
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, _Response);
  I->tz = doc["timezone"].as<String>();
  I->is_dst = doc["is_dst"];
  int dst_savings = doc["dst_savings"].as<int>();
  double timezone_offset = doc["timezone_offset"];
  I->offset= ((I->is_dst) ? dst_savings : 0) + timezone_offset;
  I->country = doc["geo"]["country_name"].as<String>();
  I->country_code = doc["geo"]["country_code2"].as<String>();
  I->city = doc["geo"]["city"].as<String>();
  I->latitude = doc["geo"]["latitude"];
  I->longitude = doc["geo"]["longitude"];

  DEBUGPRINT("Time Zone: ");
  DEBUGPRINT(I->tz);
  DEBUGPRINT("DST Savings: ");
  DEBUGPRINT(dst_savings);
}
