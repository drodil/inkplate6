#include "Widget.h"

#ifndef EVENT_LIST_WIDGET_H
#define EVENT_LIST_WIDGET_H

class EventListWidget : public Widget {
public:
  struct Event
  {
    char name[128];
    char startTime[16];
    char endTime[16];
    char date[32];
    char location[128];
    int timeStamp;
  };
  
  EventListWidget(Inkplate* display, Network* network);
  void draw(bool partial);

private:
  Event events[128];
  int eventsNum;
  int textColor;

  void parseEvents(char* data);
  void icalTimeToTm(char* time, struct tm* ret);
};

#endif // EVENT_LIST_WIDGET_H
