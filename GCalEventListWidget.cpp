#include "GCalEventListWidget.h"

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
  bool fullDay;
};

Event* events = new Event[32]();

GCalEventListWidget::GCalEventListWidget(Inkplate* display, Network* network) : Widget(display, network) {

}

void GCalEventListWidget::draw(bool partial) {
  if(partial && eventsNum > 0) {
    return;
  }

  DynamicJsonDocument doc(4096);
  int retry = 0;
  bool success = true;
  while(!network->getJSON(GAPPS_SCRIPT_URL, &doc)) {
    Serial.println(F("Failed to fetch events.."));
    delay(500);
    if(++retry > 1) {
      success = false;
      break;
    }
  }

  if(success) {
    int parsedEvents = parseEvents(&doc);
    if(parsedEvents > 0) {
      eventsNum = parsedEvents;
    }
  }
  
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

int GCalEventListWidget::parseEvents(DynamicJsonDocument* doc) {
  struct tm today = {0}, tomorrow = {0};
  network->getCurrentTimeInfo(&today, 0);
  network->getCurrentTimeInfo(&tomorrow, 86400L);
  
  JsonArray arr = doc->as<JsonArray>();
  int i = 0;
  for (JsonArray::iterator it=arr.begin(); it != arr.end(); ++it) {
    JsonObject event = it->as<JsonObject>();
    strncpy(events[i].name, event["name"], 64);

    struct tm eventStart = {0}, eventEnd = {0};
    char start[24];
    char end[24];
    strncpy(start, event[F("start")] | "", 24);
    strncpy(end, event[F("end")] | "", 24);
    parseIsoTime(start, &eventStart);
    parseIsoTime(end, &eventEnd);
    
    if(eventStart.tm_mday == today.tm_mday && eventStart.tm_mon == today.tm_mon && eventStart.tm_year == today.tm_year) {
      strcpy(events[i].date, TODAY);
    } else if(eventStart.tm_mday == tomorrow.tm_mday && eventStart.tm_mon == tomorrow.tm_mon && eventStart.tm_year == tomorrow.tm_year) {
      strcpy(events[i].date, TOMORROW);
    } else {
      char date[16];
      char dateAndDay[32];
      strftime(date, 16, DATE_FORMAT, &eventStart);
      sprintf(dateAndDay, "%s %s", DAYS_ABBR[eventStart.tm_wday], date);
      strcpy(events[i].date, dateAndDay);
    }

    strftime(events[i].startTime, 16, TIME_FORMAT, &eventStart);
    strftime(events[i].endTime, 16, TIME_FORMAT, &eventEnd);
    ++i;
  }
  return i;
}

void GCalEventListWidget::parseIsoTime(char* time, struct tm* ret) {
    // Datetime
    // 012345678901234567890123
    // 2021-04-02T12:00:00.000Z
    char temp[128];
    strncpy(temp, time, 24);
    temp[19] = 'Z';
    temp[13] = temp[16] = '-';
    strptime(temp, "%Y-%m-%dT%H-%M-%SZ", ret);
}
