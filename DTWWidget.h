#ifndef DTWWIDGET_H
#define DTWWIDGET_H

#include "Widget.h"

class DTWWidget : public Widget {
  public:
    DTWWidget(Inkplate* display, Network* network);
    void draw(bool partial);
};

#endif // DTWWIDGET_H
