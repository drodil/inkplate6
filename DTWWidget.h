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

    char prevDescription[32];
    char prevTemp[5];
    char prevIcon[4];
    char prevFeelsLike[5];
    char prevWindSpeed[5];
    char prevWindDir[3];
};

#endif // DTWWIDGET_H
