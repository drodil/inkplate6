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
    char prevTemp[8];
    char prevIcon[4];
    char prevFeelsLike[8];
    char prevWindSpeed[8];
    char prevWindDir[4];
    char prevHumidity[4];
};

#endif // DTWWIDGET_H
