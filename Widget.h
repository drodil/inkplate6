#include "Inkplate.h"
#include "Network.h"

#ifndef WIDGET_H
#define WIDGET_H

// 0x1000 -> upper_x = 0
// 0x0100 -> upper_y = 0
// 0x0010 -> lower_x = 50% width
// 0x0001 -> lower_y = 50% height

// WIDGET LOCATION DEFINITIONS
// Full screen
#define WIDGET_LOCATION_FULLSCREEN 0x1100

// Half screen locations
#define WIDGET_LOCATION_HALF_LEFT 0x1110
#define WIDGET_LOCATION_HALF_RIGHT 0x0110
#define WIDGET_LOCATION_HALF_UPPER 0x0001
#define WIDGET_LOCATION_HALF_LOWER 0x1000

// Quarter screen locations
#define WIDGET_LOCATION_UPPER_LEFT 0x1111
#define WIDGET_LOCATION_LOWER_LEFT 0x1011
#define WIDGET_LOCATION_UPPER_RIGHT 0x0111
#define WIDGET_LOCATION_LOWER_RIGHT 0x0000

// Widget color schemes
#define WIDGET_COLORSCHEME_LIGHT 7
#define WIDGET_COLORSCHEME_LIGHT_GREY 5
#define WIDGET_COLORSCHEME_DARK_GREY 3
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
    int getScreenWidth();
    int getScreenHeight();

    int getTextColor();

  protected:
    void drawBackground();
  
    Inkplate* display;
    Network* network;
    int location;
    int colorScheme;
};

#endif // WIDGET_H
