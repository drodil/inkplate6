#include "Widget.h"

#ifndef EVENT_LIST_WIDGET_H
#define EVENT_LIST_WIDGET_H

class EventListWidget : public Widget {
public:  
  EventListWidget(Inkplate* display, Network* network);
  void draw(bool partial);

private:
  int eventsNum;

  int parseEvents(char* data);
  void icalTimeToTm(char* time, struct tm* ret, bool* fullDay);
};

#endif // EVENT_LIST_WIDGET_H
