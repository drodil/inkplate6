#include "Network.h"

#include "Const.h"

void Network::init(char* ssid, char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  http.getStream().setNoDelay(true);
  http.getStream().setTimeout(5);

  this->connect();
}

bool Network::getJSON(char* url, DynamicJsonDocument* doc) {
  this->connect();
  
  bool error = false;
  bool sleep = WiFi.getSleep();
  WiFi.setSleep(false);

  int httpCode = http.GET();
  if (httpCode == 200) {
    int32_t len = http.getSize();
    if (len > 0) {
      DeserializationError jsonError = deserializeJson(*doc, http.getStream());
      if (jsonError) {
        Serial.print("JSON deserialize failed: ");
        Serial.println(jsonError.c_str());
        error = true;
      }
    } else {
      Serial.println("Length zero response");
      error = true;   
    }
  } else {
    Serial.println(httpCode);
    error = true;
  }
  WiFi.setSleep(sleep);
  return error;
}

bool Network::getData(char* url, char* data) {
  this->connect();
  
  bool error = false;
  bool sleep = WiFi.getSleep();
  WiFi.setSleep(false);

  int httpCode = http.begin(url);
  if (httpCode == 200) {
      long n = 0;
      while (http.getStream().available())
          data[n++] = http.getStream().read();
      data[n++] = 0;
  } else {
      Serial.println(httpCode);
      error = true;
  }
  return error;
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

void Network::getTime(char *timeStr, long offSet)
{
    time_t nowSecs = time(nullptr) + (long)TIMEZONE * 3600L + offSet;

    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);

    strftime(timeStr, 20, TIME_FORMAT, &timeinfo);
}

void Network::getDate(char *dateStr, long offSet) {
    time_t nowSecs = time(nullptr) + (long)TIMEZONE * 3600L + offSet;

    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);

    strftime(dateStr, 20, DATE_FORMAT, &timeinfo);
}

void Network::getDayName(char *dayNameStr, long offSet) {
    time_t nowSecs = time(nullptr) + (long)TIMEZONE * 3600L + offSet;

    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);

    switch(timeinfo.tm_wday) {
      default:
      case 0:
        strcpy(dayNameStr, DAY0);
        break;
      case 1:
        strcpy(dayNameStr, DAY1);
        break;
      case 2:
        strcpy(dayNameStr, DAY2);
        break;
      case 3:
        strcpy(dayNameStr, DAY3);
        break;
      case 4:
        strcpy(dayNameStr, DAY4);
        break;
      case 5:
        strcpy(dayNameStr, DAY5);
        break;
      case 6:
        strcpy(dayNameStr, DAY6);
        break;
    }
}
