#ifndef NETWORK_H
#define NETWORK_H

#include "Arduino.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

class Network {
  public:
    void init(char* ssid, char* password);
    void getTime(char *timeStr, int timeZone);
    bool getJSON(char* url, DynamicJsonDocument* doc);
    bool getData(char* url, char* data);

  private:
    void connect();
    void setTime();
    HTTPClient http;
};

#endif // NETWORK_H
