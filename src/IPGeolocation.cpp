#include "Arduino.h"

#ifdef ESP8266
  #include <ESP8266WIFI.h>
#elif defined(ESP32)
  #include <WiFi.h>
#else
  #error "Not a ESP8266 or ESP32 MCU"
#endif

#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "IPGeolocation.h"

IPGeolocation::IPGeolocation(String Key){
  _Key = Key;
  _API = "IPG";
}
IPGeolocation::IPGeolocation(String Key, String API){
  _Key = Key;
  _API = API;
}

String IPGeolocation::getResponse(){
  return _Response;
}

void IPGeolocation::updateStatus(IPGeo *I){
  if(_API == "ABSTRACT"){
    const char *host = "ipgeolocation.abstractapi.com";
    const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
    WiFiClientSecure httpsClient;
    const size_t capacity = 2048;
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
    Link = "/v1/?api_key="+_Key;

    DEBUGPRINT("requesting URL: ");
    DEBUGPRINT(host+Link);

    httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Connection: close\r\n\r\n");

    DEBUGPRINT("request sent");

    int timeout = millis() + 5000;
    while (httpsClient.available() == 0) {
      if (timeout - millis() < 0) {
        DEBUGPRINT(">>> client timeout !");
        httpsClient.stop();
        return;
      }
    }

    while (httpsClient.available()) {  // changed from .connected()
      String _Response = httpsClient.readStringUntil('\n');
      if (_Response == "\r") {
        DEBUGPRINT("headers received");
        DEBUGPRINT(_Response);
        break;
      }
    }

    DEBUGPRINT("reply was:");
    DEBUGPRINT("==========");
    //httpsClient.readStringUntil('\n'); // The API sends an extra line with just a number. This breaks the JSON parsing, hence an extra read
    while(httpsClient.available()){  // changed from .connected()
      _Response = httpsClient.readString();
      DEBUGPRINT(_Response); //Print response
    }

    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, _Response);
    JsonObject timezone = doc["timezone"];
    JsonObject connection = doc["connection"];
    JsonObject flag = doc["flag"];
    I->tz = timezone["name"].as<String>();
    I->abbreviation = timezone["abbreviation"].as<String>();
    I->is_dst = timezone["is_dst"];
    I->offset= timezone["gmt_offset"];
    I->country = doc["country"].as<String>();
    I->country_code = doc["country_code"].as<String>();
    I->city = doc["city"].as<String>();
    I->latitude = doc["latitude"];
    I->longitude = doc["longitude"];
    I->ip_address = doc["ip_address"].as<String>();
    I->isp_name = connection["isp_name"].as<String>();
    I->flag_png = flag["png"].as<String>();

    DEBUGPRINT("Time Zone: ");
    DEBUGPRINT(I->tz);
  }
  else {
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
    I->offset= timezone_offset + ((I->is_dst) ? dst_savings : 0);
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
}
