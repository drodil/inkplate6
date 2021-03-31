#include "Widget.h"
#include "Settings.h"

#ifndef DTWWIDGET_H
#define DTWWIDGET_H

class DTWWidget : public Widget {
  public:
    DTWWidget(Inkplate* display, Network* network);
    void draw(bool partial);
  private:
    void drawDate();
    void drawTime();
    void drawWeather();

    int textColor;
};

#endif // DTWWIDGET_H
