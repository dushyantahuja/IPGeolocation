/*
  TFLStatus.cpp - Library to get Geolocation data from IPGeolocation.io
  Created by Dushyant Ahuja, September 2019.
  Released into the public domain.
*/

#ifndef IPGeolocation_h
#define IPGeolocation_h

#include "Arduino.h"
#include <ESP8266WiFi.h>



class IPGeolocation
{
  public:
    IPGeolocation(String Key); // For Tube Status
    void updateStatus();
    String getTimezone();
    int getOffset();
    String getResponse();
  private:
    String _Key;
    String _TimeZone;
    int _offset;
    String _Response;
};

#endif
