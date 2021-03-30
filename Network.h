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
    DynamicJsonDocument getJSON(char* url); 

  private:
    void connect();
    void setTime();
};

#endif // NETWORK_H
