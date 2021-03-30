#include "DTWWidget.h"

#include "Fonts/Roboto_Light_120.h"
#include "Fonts/Roboto_Light_36.h"
#include "Fonts/Roboto_Light_48.h"

#include "icons.h"

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

int color = WHITE;

DTWWidget::DTWWidget(Inkplate* display, Network* network) : Widget(display, network) {
  
}

void DTWWidget::draw(bool partial) {
  if(colorScheme == WIDGET_COLORSCHEME_LIGHT) {
    color = BLACK;
  }
  this->drawBackground();
  this->drawTime();
  if(!partial) {
    this->drawDate();
    this->drawWeather();
  }
}

void DTWWidget::drawDate() {
  display->setTextColor(color);
  display->setTextSize(1);
  display->setFont(&Roboto_Light_36);
  network->getDayName(currentDay, 0);
  display->setCursor(getMidX() - 30, getUpperY() + 10);
  display->println(currentDay);
  
  network->getDate(currentDate, 0);
  display->setCursor(getMidX() - 30, getUpperY() + 30);
  display->println(currentDate);
}

void DTWWidget::drawTime() {
  display->setTextColor(color);
  display->setFont(&Roboto_Light_48);
  network->getTime(currentTime, 0);
  display->setCursor(getMidX() - 10, getUpperY() + 60);
  display->println(currentTime);
}

void DTWWidget::drawWeather() {
    display->setTextColor(color);
  DynamicJsonDocument doc(2048);
  char url[256];
  sprintf(url, "https://api.openweathermap.org/data/2.5/onecall?lat=%d&lon=%d&exclude=minutely,alerts&units=%s&lang=%s&appid=%s", 
          LATITUDE, LONGITUDE, UNITS, LANG, OPENWEATHERMAP_APIKEY);

  int retries = 0;
  while(!network->getJSON(url, &doc)) {
    Serial.println("Failed to fetch weather data, retrying..");
    delay(1000);
    retries++;
    if(retries > 5) {
      display->setCursor(getMidX() - 30, getUpperY() + 150);
      display->println("Säädatan haku epäonnistui.. :(");
      return;
    }
  }

  for(int i = 0; i < 18; i++) {
    if(strcmp(abbrs[i], doc["current"]["weather"][0]["icon"]) == 0) {
      display->drawBitmap(getMidX(), getMidY(), icons[i], 152, 152, color);
      break;
    }
  }

  display->setFont(&Roboto_Light_36);
  display->setTextSize(1);
  display->setCursor(getMidX() - 20, getUpperY() + 200);
  display->print(doc["current"]["temp"].as<char *>());
  display->println(" °C");

  // TODO: Forecast...
}
