#include "DTWWidget.h"

#include "Fonts/Roboto_Light_120.h"
#include "Fonts/Roboto_Light_36.h"
#include "Fonts/Roboto_Light_48.h"

char currentTime[16] = "00:00";
char currentDate[16] = "dd.mm.yyyy";

DTWWidget::DTWWidget(Inkplate* display, Network* network) : Widget(display, network) {
  
}

void DTWWidget::draw(bool partial) {
  this->drawBackground();
  this->drawDateTime();
  if(!partial) {
    this->drawWeather();
  }
}

void DTWWidget::drawDateTime() {
  if(colorScheme == WIDGET_COLORSCHEME_DARK) {
    display->setTextColor(WHITE);
  } else {
    display->setTextColor(BLACK);
  }
  
  display->setTextSize(1);
  display->setFont(&Roboto_Light_36);
  network->getDate(currentDate, 0);
  display->setCursor(getMidX() - 30, getUpperY() + 20);
  display->println(currentDate);
  
  display->setFont(&Roboto_Light_48);
  network->getTime(currentTime, 0);
  display->setCursor(getMidX() - 10, getUpperY() + 60);
  display->println(currentTime);
}

void DTWWidget::drawWeather() {
  // TODO.
}
