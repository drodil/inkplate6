#include "Widget.h"

#include "Settings.h"

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
  // TODO: Support rotation
  return location & 0x1000 ? 0 : DISPLAY_WIDTH / 2;
}

int Widget::getUpperY() {
  // TODO: Support rotation
  return location & 0x0100 ? 0 : DISPLAY_HEIGHT / 2;
}

int Widget::getLowerX() {
  // TODO: Support rotation
  return location & 0x0010 ? (DISPLAY_WIDTH / 2) - 1 : DISPLAY_WIDTH - 1;
}

int Widget::getLowerY() {
  // TODO: Support rotation
  return location & 0x0001 ? (DISPLAY_HEIGHT / 2) - 1 : DISPLAY_HEIGHT - 1;
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

void Widget::drawBackground() {
  display->fillRect(getUpperX(), getUpperY(), getWidth() - 1, getHeight() -1, colorScheme);
}
