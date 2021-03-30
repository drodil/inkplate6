#include "DTWWidget.h"

DTWWidget::DTWWidget(Inkplate* display, Network* network) : Widget(display, network) {
  
}

void DTWWidget::draw(bool partial) {
  this->drawBackground();
}
