#include "Widget.h"

#include "Settings.h"

// Default screen size for inkplate6
// Might work for inkplate10 if change these to 1200x825
const unsigned SCREEN_WIDTH = 800;
const unsigned SCREEN_HEIGHT = 600;

Widget::Widget(Inkplate* display, Network* network) {
  this->display = display;
  this->network = network;
}

void Widget::setLocation(int location) {
  this->location = location;
}

void Widget::setColorScheme(int colorScheme) {
  int color = colorScheme;
  if(display->getDisplayMode() == INKPLATE_1BIT) {
    color = (colorScheme <= 3) ? BLACK : WHITE; 
  }
  this->colorScheme = color;
}

int Widget::getUpperX() {
  return (location & 0x1000) == 0x1000 ? 0 : getScreenWidth() / 2;
}

int Widget::getUpperY() {
  return (location & 0x0100) == 0x0100 ? 0 : getScreenHeight() / 2;
}

int Widget::getLowerX() {
  int w = (location & 0x0010) == 0x0010 ? (getScreenWidth() / 2) - 1 : getScreenWidth() - 1;
  return getUpperX() + w;
}

int Widget::getLowerY() {
  int h =(location & 0x0001) == 0x0001 ? (getScreenHeight() / 2) - 1 : getScreenHeight() - 1;
  return getUpperY() + h;
}

int Widget::getMidX() {
  return getUpperX() + (getWidth() / 2);
}

int Widget::getMidY() {
  return getUpperY() + (getHeight() / 2);
}

int Widget::getWidth() {
  return getLowerX() - getUpperX() + 1;
}

int Widget::getHeight() {
  return getLowerY() - getUpperY() + 1;
}

int Widget::getScreenWidth() {
  uint8_t rot = display->Adafruit_GFX::getRotation();
  if(rot == 0 || rot == 2) {
    return SCREEN_WIDTH;
  }
  return SCREEN_HEIGHT;
}

int Widget::getScreenHeight() {
  uint8_t rot = display->Adafruit_GFX::getRotation();
  if(rot == 0 || rot == 2) {
    return SCREEN_HEIGHT;
  }
  return SCREEN_WIDTH;
}

int Widget::getTextColor() {
  if(display->getDisplayMode() == INKPLATE_1BIT) {
    return colorScheme == BLACK ? WHITE : BLACK;
  }
  
  if(colorScheme > 3) {
      return BLACK;
  }
  return WHITE;
}

void Widget::drawBackground() {
  display->fillRect(getUpperX(), getUpperY(), getWidth(), getHeight(), colorScheme);
}
