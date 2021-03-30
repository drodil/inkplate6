#include "Inkplate.h"
#include "Network.h"

#ifndef WIDGET_H
#define WIDGET_H

// WIDGET LOCATION DEFINITIONS
// Full screen
#define WIDGET_LOCATION_FULLSCREEN 0x1100

// Half screen locations
#define WIDGET_LOCATION_HALF_LEFT 0x1110
#define WIDGET_LOCATION_HALF_RIGHT 0x0110

// Quarter screen locations
#define WIDGET_LOCATION_UPPER_LEFT 0x1111
#define WIDGET_LOCATION_LOWER_LEFT 0x1011
#define WIDGET_LOCATION_UPPER_RIGHT 0x0111
#define WIDGET_LOCATION_UPPER_LEFT 0x0110

// Widget color schemes
#define WIDGET_COLORSCHEME_LIGHT 1
#define WIDGET_COLORSCHEME_DARK 0

class Widget {
  public:
    Widget(Inkplate* display, Network* network);
    virtual void draw(bool partial) = 0;
    void setLocation(int location);
    void setColorScheme(int colorScheme);

    int getUpperX();
    int getUpperY();
    int getLowerX();
    int getLowerY();
    int getMidX();
    int getMidY();
    int getWidth();
    int getHeight();

  protected:
    void drawBackground();
  
    Inkplate* display;
    Network* network;
    int location;
    int colorScheme;
};

#endif // WIDGET_H
