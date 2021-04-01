#include "Network.h"

void Network::init(char* ssid, char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  this->connect(true);
}

bool Network::getJSON(char* url, DynamicJsonDocument* doc) {
  this->connect(false);

  Serial.print(F("Getting JSON from "));
  Serial.println(url);
  
  bool error = false;
  bool sleep = WiFi.getSleep();
  WiFi.setSleep(false);

  HTTPClient http;
  http.getStream().setTimeout(30);
  http.getStream().flush();
  http.begin(url);
  
  int httpCode = http.GET();
  if (httpCode == 200) {
    int32_t len = http.getSize();
    if (len > 0) {
      DeserializationError jsonError = deserializeJson(*doc, http.getStream());
      if (jsonError) {
        Serial.print(F("JSON deserialize failed: "));
        Serial.println(jsonError.c_str());
        error = true;
      }
    } else {
      Serial.println(F("Length zero response"));
      error = true;   
    }
  } else {
    Serial.print(F("Invalid response code: "));
    Serial.println(httpCode);
    error = true;
  }

  http.end();
  WiFi.setSleep(sleep);
  return !error;
}

bool Network::getData(char* url, char* data, int maxLength) {
  this->connect(false);
  Serial.print(F("Getting data from "));
  Serial.println(url);
  
  bool error = false;
  bool sleep = WiFi.getSleep();
  WiFi.setSleep(false);

  HTTPClient http;
  http.getStream().setTimeout(30);
  http.getStream().flush();
  http.begin(url);

  int httpCode = http.GET();
  
  long n = 0;
  while (http.getStream().available()) {
    data[n++] = http.getStream().read();
    if(n >= maxLength - 2) {
      Serial.println(F("Could not fetch complete response, no buffer space!"));
      error = true;
      break;
    }
  }
  data[n++] = 0;
  
  http.end();
  WiFi.setSleep(sleep);
  return !error;
}

void Network::connect(bool initial) {
  if(WiFi.status() == WL_CONNECTED) {
    return;
  }
  
  Serial.print(F("Connecting to WiFi..."));
  int retry = 0;
  while((WiFi.status() != WL_CONNECTED)) {
    Serial.print(F("."));
    delay(1000);
    if(++retry == 30) {
      if(initial) {
        Serial.println(F(" Cannot connect to WiFi, restarting."));
        delay(100);
        ESP.restart();
      } else {
        Serial.println(F(" Could not connect to WiFi, returning."));
        return;
      }
    }
  }
  Serial.println(F(" connected!"));
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

void Network::getCurrentTimeInfo(struct tm* timeinfo, long offSet) {
  time_t nowSecs = time(nullptr) + (long)TIMEZONE * 3600L + offSet;
  gmtime_r(&nowSecs, timeinfo);
}

void Network::getTime(char *timeStr, long offSet)
{
  struct tm timeinfo;
  getCurrentTimeInfo(&timeinfo, offSet);
  strftime(timeStr, 16, TIME_FORMAT, &timeinfo);
}

void Network::getDate(char *dateStr, long offSet) {
  struct tm timeinfo;
  getCurrentTimeInfo(&timeinfo, offSet);
  strftime(dateStr, 32, DATE_FORMAT, &timeinfo);
}

void Network::getDayName(char *dayNameStr, long offSet) {
  struct tm timeinfo;
  getCurrentTimeInfo(&timeinfo, offSet);
  strcpy(dayNameStr, DAYS[timeinfo.tm_wday]);
}
