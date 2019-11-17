# IPGeolocation

Get API Key by registering on 
https://ipgeolocation.io/documentation/timezone-api.html

A small library for an ESP8266 to pull timezone information using IPGeolocation.io
Please note, this version changes the way the library works. There is now a struct that you need to use - as the class does not save any values anymore. I felt this was more extensible. Sorry if I broke something.

The new struct is:
```C++
struct IPGeo {
  String tz;
  int offset;
  String city;
  String country;
  String country_code;
  bool is_dst;
  double latitude;
  double longitude;
};
```
Example Code

```C++
#include <ESP8266WiFi.h>
#include "IPGeolocation.h"


// Replace with your wifi credentials
const char * ssid = "****";
const char * password = "****";

// Get an API Key by registering on
// https://ipgeolocation.io

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
  Serial.println(location.getResponse());

  delay(600000);       // Wait for 600 seconds
  // put your main code here, to run repeatedly:
}
```



