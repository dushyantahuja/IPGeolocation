/*
  IPGeolocation.cpp - Library to get Geolocation data from IPGeolocation.io
  Created by Dushyant Ahuja, September 2019.
  Released into the public domain.
*/

#ifndef IPGeolocation_h
#define IPGeolocation_h

#ifdef DEBUG
  #ifndef DEBUG_PRINT(x)
    #define DEBUG_PRINT(x)  Serial.println (x)
  #endif
#else
  #ifndef DEBUG_PRINT(x)
    #define DEBUG_PRINT(x)
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
};

class IPGeolocation
{
  public:
    IPGeolocation(String Key); 
    void updateStatus(IPGeo *I);
  private:
    String _Key;
    String _Response;
};

#endif
