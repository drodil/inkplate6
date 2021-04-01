#include "DTWWidget.h"

#include "Fonts/Roboto_Light_36.h"
#include "Fonts/Roboto_Light_48.h"
#include "Fonts/FreeSans12pt7b.h"

#include "icons.h"
#include "Util.h"

char currentTime[16] = "00:00";
char currentDate[16] = "dd.mm.yyyy";
char currentDay[16] = "";

char abbrs[32][18] = {"01d", "01n", 
                      "02d", "02n", 
                      "03d", "03n", 
                      "04d", "04n", 
                      "09d", "09n", 
                      "10d", "10n", 
                      "11d", "11n", 
                      "13d", "13n", 
                      "50d", "50n"};
                      
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
  display->setCursor(getMidX() - 90, getUpperY() + 50);
  display->println(currentDay);
  
  network->getDate(currentDate, 0);
  display->setCursor(getMidX() - 90, getUpperY() + 90);
  display->println(currentDate);
}

void DTWWidget::drawTime() {
  Serial.println(F("Drawing time"));
  display->fillRect(getUpperX(), getUpperY() + 100, getWidth(), getUpperY() + 150, colorScheme);
  display->setTextColor(getTextColor());
  display->setFont(&Roboto_Light_48);
  display->setTextSize(2);
  network->getTime(currentTime, 0);
  display->setCursor(getMidX() - 120, getUpperY() + 200);
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
  }
  
  for(int i = 0; i < 18; i++) {
    if(strcmp(abbrs[i], prevIcon) == 0) {
      display->drawBitmap(getMidX() - 152, getUpperY() + 220, icons[i], 152, 152, getTextColor());
      break;
    }
  }

  display->setFont(&Roboto_Light_48);
  display->setTextSize(1);
  display->setCursor(getMidX(), getUpperY() + 315);
  display->print(prevTemp);
  display->println(F("°C"));

  display->setFont(&FreeSans12pt7b);
  display->setCursor(getMidX() - 120, getUpperY() + 380);
  display->println(replaceUmlauts(prevDescription));
  
  display->setCursor(getMidX() - 120, getUpperY() + 430);
  display->print(FEELS_LIKE);
  display->println(F(":"));
  display->setCursor(getMidX() + 50, getUpperY() + 430);
  display->print(prevFeelsLike);
  display->println(F("°C"));

  display->setCursor(getMidX() - 120, getUpperY() + 460);
  display->print(WIND_SPEED);
  display->println(F(":"));
  display->setCursor(getMidX() + 50, getUpperY() + 460);
  display->print(prevWindSpeed);
  display->println(F("m/s"));

  display->setCursor(getMidX() - 120, getUpperY() + 490);
  display->print(WIND_DIRECTION);
  display->println(F(":"));
  display->setCursor(getMidX() + 50, getUpperY() + 490);
  display->println(prevWindDir);
}
