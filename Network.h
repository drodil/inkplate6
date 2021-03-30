#include "Arduino.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "Settings.h"

#ifndef NETWORK_H
#define NETWORK_H

class Network {
  public:
    void init(char* ssid, char* password);
    void getTime(char *timeStr, long offSet);
    void getDate(char *dateStr, long offSet);
    void getDayName(char *dayNameStr, long offSet);
    bool getJSON(char* url, DynamicJsonDocument* doc);
    bool getData(char* url, char* data);

  private:
    void connect();
    void setTime();
    HTTPClient http;
};

#endif // NETWORK_H
