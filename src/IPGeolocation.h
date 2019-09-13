/*
  TFLStatus.cpp - Library to get tube status from the TFL API
  Created by Dushyant Ahuja, July 2019.
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


