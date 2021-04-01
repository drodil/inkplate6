#include "Widget.h"

#ifndef GCAL_EVENT_LIST_WIDGET_H
#define GCAL_EVENT_LIST_WIDGET_H

class GCalEventListWidget : public Widget {
public:  
  GCalEventListWidget(Inkplate* display, Network* network);
  void draw(bool partial);

private:
  int eventsNum;

  int parseEvents(DynamicJsonDocument* doc);
  void parseIsoTime(char* time, struct tm* ret);
};

#endif // GCAL_EVENT_LIST_WIDGET_H
