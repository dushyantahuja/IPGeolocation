/*
  IPGeolocation.cpp - Library to get Geolocation data from https://IPGeolocation.io or https://app.abstractapi.com/api/ip-geolocation/
  Created by Dushyant Ahuja, September 2019.
  Released into the public domain.
*/

#ifndef IPGeolocation_h
#define IPGeolocation_h

#ifndef DEBUGPRINT
  #ifdef DEBUG
    #define DEBUGPRINT(x)  Serial.println (x)
  #else
    #define DEBUGPRINT(x)
  #endif
#endif

#include "Arduino.h"
#include <ESP8266WiFi.h>

struct IPGeo {
  String tz;
  double offset;
  String city;
  String country;
  String country_code;
  bool is_dst;
  double latitude;
  double longitude;
  String ip_address;
  String isp_name;
  String flag_png;
};

class IPGeolocation
{
  public:
    IPGeolocation(String Key);
    IPGeolocation(String Key, String API); // Use IPG for api.ipgeolocation.io and ABSTRACT for app.abstractapi.com/api/ip-geolocation
    void updateStatus(IPGeo *I);
    String getResponse();
  private:
    String _Key;
    String _Response;
    String _API;
};

#endif
