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
  this->colorScheme = colorScheme;
}

int Widget::getUpperX() {
  return location & 0x1000 ? 0 : getScreenWidth() / 2;
}

int Widget::getUpperY() {
  return location & 0x0100 ? 0 : getScreenHeight() / 2;
}

int Widget::getLowerX() {
  return location & 0x0010 ? (getScreenWidth() / 2) - 1 : getScreenWidth() - 1;
}

int Widget::getLowerY() {
  return location & 0x0001 ? (getScreenHeight() / 2) - 1 : getScreenHeight() - 1;
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
  if(rot == 2 || rot == 4) {
    return SCREEN_WIDTH;
  }
  return SCREEN_HEIGHT;
}

int Widget::getScreenHeight() {
  uint8_t rot = display->Adafruit_GFX::getRotation();
  if(rot == 2 || rot == 4) {
    return SCREEN_HEIGHT;
  }
  return SCREEN_WIDTH;
}

void Widget::drawBackground() {
  display->fillRect(getUpperX(), getUpperY(), getWidth() - 1, getHeight() -1, colorScheme);
}
