#include "Network.h"

void Network::init(char* ssid, char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  this->connect();
}

DynamicJsonDocument Network::getJSON(char* url) {
  this->connect();
  bool sleep = WiFi.getSleep();
  WiFi.setSleep(false);
  DynamicJsonDocument doc(2000);
  HTTPClient http;

  http.getStream().setNoDelay(true);
  http.getStream().setTimeout(1);

  http.begin(url);
  int httpCode = http.GET();
  if (httpCode == 200)
  {
     int32_t len = http.getSize();
     if (len > 0)
     {
        deserializeJson(doc, http.getStream());
     }
  }
  WiFi.setSleep(sleep);
  return doc;
}

void Network::connect() {
  if(WiFi.status() == WL_CONNECTED) {
    return;
  }
  
  Serial.print("Connecting to WiFi...");
  int retry = 0;
  while((WiFi.status() != WL_CONNECTED)) {
    Serial.print(".");
    delay(1000);
    ++retry;
    if(retry == 20) {
      Serial.println("Cannot connect to WiFi, restarting.");
      delay(100);
      ESP.restart();
    }
  }
  Serial.println(" connected!");

  setTime();
}

void Network::setTime() {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print(F("Waiting for NTP time sync: "));
    time_t nowSecs = time(nullptr);
    while (nowSecs < 8 * 3600 * 2)
    {
        // Print a dot every half a second while time is not set
        delay(500);
        Serial.print(F("."));
        yield();
        nowSecs = time(nullptr);
    }

    Serial.println();

    // Used to store time info
    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);

    Serial.print(F("Current time: "));
    Serial.print(asctime(&timeinfo));
}

void Network::getTime(char *timeStr, int timeZone)
{
    time_t nowSecs = time(nullptr);

    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);

    strncpy(timeStr, asctime(&timeinfo) + 11, 5);

    int hr = 10 * timeStr[0] + timeStr[1] + timeZone;

    hr = (hr % 24 + 24) % 24;

    timeStr[0] = hr / 10 + '0';
    timeStr[1] = hr % 10 + '0';
}
