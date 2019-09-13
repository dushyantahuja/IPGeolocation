# IPGeolocation
A small library for an ESP8266 to pull timezone information using IPGeolocation.io

Get API Key by registering on 

https://ipgeolocation.io/documentation/timezone-api.html

Very simple to use - 

String Key = "**************";
IPGeolocation location(Key);

And in your loop:

  location.updateStatus();
  Serial.println(location.getTimezone());
  Serial.println(location.getOffset());
  Serial.println(location.getResponse());




