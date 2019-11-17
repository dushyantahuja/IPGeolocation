/*
  IPGeolocation.cpp - Library to get Geolocation data from IPGeolocation.io
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
  int offset;
  String city;
  String country;
  String country_code;
  bool is_dst;
  double latitude;
  double longitude;
};

class IPGeolocation
{
  public:
    IPGeolocation(String Key);
    void updateStatus(IPGeo *I);
    String getResponse();
  private:
    String _Key;
    String _Response;
};

#endif
