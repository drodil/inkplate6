#include "EventListWidget.h"

#include "Fonts/Roboto_Light_36.h"
#include "Fonts/Roboto_Light_48.h"
#include "Fonts/FreeMono9pt7b.h"

#include "Util.h"

struct Event
{
  char name[64];
  char startTime[16];
  char endTime[16];
  char date[32];
  char location[64];
  int timeStamp;
  bool fullDay;
};

char* data = new char[65536];
Event* events = new Event[32]();

int cmpEvents(const void *a, const void *b)
{
  Event *entryA = (Event *)a;
  Event *entryB = (Event *)b;
  return (entryA->timeStamp - entryB->timeStamp);
}

EventListWidget::EventListWidget(Inkplate* display, Network* network) : Widget(display, network) {

}

void EventListWidget::draw(bool partial) {
  if(partial && eventsNum > 0) {
    return;
  }
  
  Serial.println("Starting to draw events");

  int parsedEvents = 0;
  for(int i = 0; i < NUM_CALENDAR_URLS; i++) {
    if(!network->getData(CALENDAR_URLS[i], data, 65536)) {
      Serial.print("Failed to fetch calendar: ");
      Serial.println(CALENDAR_URLS[i]);
      continue;
    }
    parsedEvents += parseEvents(data);
  }

  if(parsedEvents > 0) {
    eventsNum = parsedEvents;
  }
  
  qsort(events, eventsNum, sizeof(Event), cmpEvents);
  this->drawBackground();
  display->setTextColor(getTextColor());
  display->setTextSize(1);
  char prevDay[32];
  int addition = 60;
  // TODO: Show no events text
  Serial.print(F("Number of events: "));
  Serial.println(eventsNum);
  
  for(int i = 0; i < eventsNum; i++) {
    if(strcmp(prevDay, events[i].date) != 0) {
      if(i > 0) {
        addition += 40;
      }
      display->setCursor(getUpperX() + 20, getUpperY() + (i * 20) + addition);
      display->setFont(&Roboto_Light_36);
      display->println(events[i].date);
      strcpy(prevDay, events[i].date);
      addition += 25;
    }
    display->setCursor(getUpperX() + 20, getUpperY() + (i * 20) + addition);
    display->setFont(&FreeMono9pt7b);
    if(events[i].fullDay) {
      display->print(F(":: "));
    } else {
      display->print(events[i].startTime);
      display->print(F("-"));
      display->print(events[i].endTime);
    }
    display->print(F(" "));
    display->println(replaceUmlauts(events[i].name));
    // TODO: Show location if any
    // TODO: Limit event name length or wrap it

    if(getUpperY() + (i * 20) + addition >= getLowerY() - 50) {
      // TODO: Show how many events upcoming..
      break;
    }
  }
}

int EventListWidget::parseEvents(char* data) {
  int parsedEvents = 0;
  long i = 0;
  long n = strlen(data);
  
  while (i < n && strstr(data + i, "BEGIN:VEVENT"))
  {
    i = strstr(data + i, "BEGIN:VEVENT") - data + 12;
    char *end = strstr(data + i, "END:VEVENT");

    if (end == NULL) {
      Serial.println("END NOT FOUND");
      continue;
    }

    char *summary = strstr(data + i, "SUMMARY:") + 8;
    char *location = strstr(data + i, "LOCATION:") + 9;
    char *timeStart = strstr(data + i, "DTSTART") + 8;
    char *timeEnd = strstr(data + i, "DTEND") + 6;

    if(summary == NULL || timeStart == NULL || timeEnd == NULL) {
      continue;
    }

    if (summary && summary < end)
    {
      strncpy(events[parsedEvents].name, summary, strchr(summary, '\n') - summary);
      events[parsedEvents].name[strchr(summary, '\n') - summary] = 0;
    }
    
    if (location && location < end)
    {
      strncpy(events[parsedEvents].location, location, strchr(location, '\n') - location);
      events[parsedEvents].location[strchr(location, '\n') - location] = 0;
    }
    
    if (timeStart && timeStart < end && timeEnd < end) {
      struct tm eventStart = {0}, eventEnd = {0};
      icalTimeToTm(timeStart, &eventStart, &events[parsedEvents].fullDay);
      time_t epoch = mktime(&eventStart) + (time_t)TIMEZONE * 3600L;
      time_t nowSecs = (long) time(nullptr) + (long)TIMEZONE * 3600L;
      // Skip past events
      if(epoch < nowSecs) {
        continue;
      }
      
      icalTimeToTm(timeEnd, &eventEnd, &events[parsedEvents].fullDay);
      
      eventStart.tm_hour += TIMEZONE;
      eventEnd.tm_hour += TIMEZONE;
      
      events[parsedEvents].timeStamp = epoch;

      char date[16];
      char dateAndDay[32];
      strftime(date, 16, DATE_FORMAT, &eventStart);
      sprintf(dateAndDay, "%s %s", DAYS_ABBR[eventStart.tm_wday], date);
      strcpy(events[parsedEvents].date, dateAndDay);

      strftime(events[parsedEvents].startTime, 16, TIME_FORMAT, &eventStart);
      strftime(events[parsedEvents].endTime, 16, TIME_FORMAT, &eventEnd);
    }
    ++parsedEvents;
  }
  return parsedEvents;
}

void EventListWidget::icalTimeToTm(char* time, struct tm* ret, bool* fullDay) {
  char temp[128];
  strncpy(temp, time, 44);
  char* dateStart = strstr(temp, "VALUE=DATE");
  char* separatorStart = strstr(temp, ":");
  if (dateStart != NULL) {
    // Date
    // VALUE=DATE:20211118
    temp[8] = 0;
    memmove(temp, dateStart + 11, 8);
    memmove(temp + 5, temp + 4, 16);
    memmove(temp + 8, temp + 7, 16);
    temp[4] = temp[7] = '-';
    *fullDay = true;
    strptime(temp, "%Y-%m-%dT00-00-00Z", ret);
  } else {
    if(separatorStart != NULL) {
      memmove(temp, separatorStart + 1, 16);
    }
    // Datetime
    // 20210402T120000Z
    temp[16] = 0;
    memmove(temp + 5, temp + 4, 16);
    memmove(temp + 8, temp + 7, 16);
    memmove(temp + 14, temp + 13, 16);
    memmove(temp + 16, temp + 15, 16);
    temp[4] = temp[7] = temp[13] = temp[16] = '-';
    *fullDay = false;
    strptime(temp, "%Y-%m-%dT%H-%M-%SZ", ret);
  }
}
