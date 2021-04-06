#include "DTWWidget.h"

#include "Fonts/Roboto_Light_36.h"
#include "Fonts/Roboto_Light_48.h"
#include "Fonts/FreeSans12pt7b.h"

#include "icons.h"
#include "Util.h"

// Current time & date and day
char currentTime[16] = "00:00";
char currentDate[16] = "dd.mm.yyyy";
char currentDay[16] = "";

// Weather icon abbrs
char abbrs[32][18] = {"01d", "01n", 
                      "02d", "02n", 
                      "03d", "03n", 
                      "04d", "04n", 
                      "09d", "09n", 
                      "10d", "10n", 
                      "11d", "11n", 
                      "13d", "13n", 
                      "50d", "50n"};
                      
// Icon array to map with abbrs
const uint8_t *icons[18] = {icon_01d, icon_01n, 
                            icon_02d, icon_02n, 
                            icon_03d, icon_03n, 
                            icon_04d, icon_04n, 
                            icon_09d, icon_09n, 
                            icon_10d, icon_10n, 
                            icon_11d, icon_11n, 
                            icon_13d, icon_13n, 
                            icon_50d, icon_50n};

DTWWidget::DTWWidget(Inkplate* display, Network* network) : Widget(display, network) {
  
}

void DTWWidget::draw(bool partial) {
  if(!partial) {
    this->drawBackground();
  }
  // Only update time if partial update
  this->drawTime();
  if(!partial) {
    this->drawDate();
    this->drawWeather();
  }
}

void DTWWidget::drawDate() {
  Serial.println(F("Drawing date"));
  display->setTextColor(getTextColor());
  display->setTextSize(1);
  display->setFont(&Roboto_Light_36);
  network->getDayName(currentDay, 0);
  int red = 7 * strlen(currentDay);
  display->setCursor(getMidX() - red, getUpperY() + 50);
  display->println(currentDay);
  
  network->getDate(currentDate, 0);
  display->setCursor(getMidX() - 90, getUpperY() + 85);
  display->println(currentDate);
}

void DTWWidget::drawTime() {
  Serial.println(F("Drawing time"));
  display->fillRect(getUpperX(), getUpperY() + 90, getWidth(), getUpperY() + 140, colorScheme);
  display->setTextColor(getTextColor());
  display->setFont(&Roboto_Light_48);
  display->setTextSize(2);
  network->getTime(currentTime, 0);
  display->setCursor(getMidX() - 120, getUpperY() + 190);
  display->println(currentTime);
}

void DTWWidget::drawWeather() {
  Serial.println(F("Drawing weather"));
  display->setTextColor(getTextColor());
  display->setTextSize(1);

  DynamicJsonDocument doc(2048);
  char* url = new char[256];
  sprintf(url, "https://api.openweathermap.org/data/2.5/onecall?lat=%f&lon=%f&exclude=minutely,hourly,daily,alerts&units=%s&lang=%s&appid=%s", 
          LATITUDE, LONGITUDE, UNITS, LANG, OPENWEATHERMAP_APIKEY);

  int retry = 0;
  bool success = true;
  while(!network->getJSON(url, &doc)) {
    Serial.println(F("Failed to fetch weather.."));
    delay(500);
    if(++retry > 1) {
      success = false;
      break;
    }
  }
  delete url;

  if(success) {
    strncpy(this->prevDescription, doc[F("current")][F("weather")][0][F("description")] | "", 30);
    *this->prevDescription = toupper(*this->prevDescription);
    strncpy(this->prevIcon, doc[F("current")][F("weather")][0][F("icon")] | "50d", 4);
    sprintf(this->prevTemp, "%.01f", doc[F("current")][F("temp")] | 0.00);
    sprintf(this->prevFeelsLike, "%.01f", doc[F("current")][F("feels_like")] | 0.00);
    sprintf(this->prevWindSpeed, "%.01f", doc[F("current")][F("wind_speed")] | 0.00);
    sprintf(this->prevWindDir, "%d", doc[F("current")][F("wind_deg")] | 0);
    sprintf(this->prevHumidity, "%d", doc[F("current")][F("humidity")] | 0);
  }
  
  for(int i = 0; i < 18; i++) {
    if(strcmp(abbrs[i], prevIcon) == 0) {
      display->drawBitmap(getMidX() - 130, getUpperY() + 200, icons[i], 152, 152, getTextColor());
      break;
    }
  }

  display->setFont(&Roboto_Light_48);
  display->setTextSize(1);
  display->setCursor(getMidX() + 15, getUpperY() + 290);
  display->print(prevTemp);
  display->println(F("°C"));

  display->setFont(&FreeSans12pt7b);
  int red = 5 * strlen(prevDescription);
  display->setCursor(getMidX() - red, getUpperY() + 355);
  display->println(replaceUmlauts(prevDescription));
  
  display->setCursor(getMidX() - 112, getUpperY() + 410);
  display->print(FEELS_LIKE);
  display->println(F(":"));
  display->setCursor(getMidX() + 55, getUpperY() + 410);
  display->print(prevFeelsLike);
  display->println(F("°C"));

  display->setCursor(getMidX() - 112, getUpperY() + 440);
  display->print(WIND_SPEED);
  display->println(F(":"));
  display->setCursor(getMidX() + 55, getUpperY() + 440);
  display->print(prevWindSpeed);
  display->println(F("m/s"));

  display->setCursor(getMidX() - 112, getUpperY() + 470);
  display->print(WIND_DIRECTION);
  display->println(F(":"));
  display->setCursor(getMidX() + 55, getUpperY() + 470);
  display->println(prevWindDir);
  
  display->setCursor(getMidX() - 112, getUpperY() + 500);
  display->print(HUMIDITY);
  display->println(F(":"));
  display->setCursor(getMidX() + 55, getUpperY() + 500);
  display->println(prevHumidity);
  display->println(F("%"));
}
