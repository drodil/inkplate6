#include "EventListWidget.h"

#include "Fonts/Roboto_Light_120.h"
#include "Fonts/Roboto_Light_36.h"
#include "Fonts/Roboto_Light_48.h"

int cmpEvents(const void *a, const void *b)
{
  EventListWidget::Event *entryA = (EventListWidget::Event *)a;
  EventListWidget::Event *entryB = (EventListWidget::Event *)b;
  return (entryA->timeStamp - entryB->timeStamp);
}

EventListWidget::EventListWidget(Inkplate* display, Network* network) : Widget(display, network) {
}

void EventListWidget::draw(bool partial) {
  if(partial) {
    return;
  }

  this->drawBackground();
  eventsNum = 0;
  for(int i = 0; i < NUM_CALENDAR_URLS; i++) {
    char data[2048];
    if(!network->getData(CALENDAR_URLS[i], data)) {
      continue;
    }
    parseEvents(data);
  }
  
  qsort(events, eventsNum, sizeof(EventListWidget::Event), cmpEvents);

  display->setTextColor(getTextColor());
  char* prevDay = "";
  int addition = 0;
  for(int i = 0; i < eventsNum; i++) {
    display->setCursor(getUpperX() + 20, getUpperY() + (i * 20) + addition);
    if(strcmp(prevDay, events[i].date) != 0) {
      display->setFont(&Roboto_Light_48);
      display->println(events[i].date);
      addition += 40;
      display->setCursor(getUpperX() + 20, getUpperY() + (i * 20) + addition);
      strcpy(prevDay, events[i].date);
    }
    display->setFont(&Roboto_Light_36);
    display->print(events[i].startTime);
    display->print(F(" - "));
    display->print(events[i].endTime);
    display->print(F("   "));
    display->println(events[i].name);
    // TODO: Show location if any
    // TODO: Limit event name length or wrap it

    if(getUpperY() + (i * 20) + addition >= getLowerY()) {
      // TODO: Show how many events upcoming..
      break;
    }
  }
}

void EventListWidget::parseEvents(char* data) {
  long i = 0;
  long n = strlen(data);
  while (i < n && strstr(data + i, "BEGIN:VEVENT"))
  {
    i = strstr(data + i, "BEGIN:VEVENT") - data + 12;
    char *end = strstr(data + i, "END:VEVENT");

    if (end == NULL)
      continue;

    char *summary = strstr(data + i, "SUMMARY:") + 8;
    char *location = strstr(data + i, "LOCATION:") + 9;
    char *timeStart = strstr(data + i, "DTSTART:") + 8;
    char *timeEnd = strstr(data + i, "DTEND:") + 6;

    if (summary && summary < end)
    {
      strncpy(events[eventsNum].name, summary, strchr(summary, '\n') - summary);
      events[eventsNum].name[strchr(summary, '\n') - summary] = 0;
    }
    
    if (location && location < end)
    {
      strncpy(events[eventsNum].location, location, strchr(location, '\n') - location);
      events[eventsNum].location[strchr(location, '\n') - location] = 0;
    }
    
    if (timeStart && timeStart < end && timeEnd < end) {
      struct tm eventStart = {0}, eventEnd = {0};
      icalTimeToTm(timeStart, &eventStart);
      icalTimeToTm(timeEnd, &eventEnd);
      
      time_t epoch = mktime(&eventStart) + (time_t)TIMEZONE * 3600L;
      events[eventsNum].timeStamp = epoch;
      
      strftime(events[eventsNum].date, 32, DATE_FORMAT, &eventStart);
      strftime(events[eventsNum].startTime, 16, TIME_FORMAT, &eventStart);
      strftime(events[eventsNum].endTime, 16, TIME_FORMAT, &eventEnd);
    }
    ++eventsNum;
  }
}

void EventListWidget::icalTimeToTm(char* time, struct tm* ret) {
  char temp[128];
  strncpy(temp, time, 16);
  temp[16] = 0;
  memmove(temp + 5, temp + 4, 16);
  memmove(temp + 8, temp + 7, 16);
  memmove(temp + 14, temp + 13, 16);
  memmove(temp + 16, temp + 15, 16);
  temp[4] = temp[7] = temp[13] = temp[16] = '-';
  strptime(temp, "%Y-%m-%dT%H-%M-%SZ", ret);
}
