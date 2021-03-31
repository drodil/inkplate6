#ifndef SETTINGS_H
#define SETTINGS_H

extern char* WIFI_SSID;
extern char* WIFI_PASSWORD;
extern char* OPENWEATHERMAP_APIKEY;

// Display height and width
extern const unsigned DISPLAY_HEIGHT;
extern const unsigned DISPLAY_WIDTH;

// Date and time formats based on strftime
extern const char* DATE_FORMAT;
extern const char* TIME_FORMAT;

// Timezone
extern const long TIMEZONE;

// Localized day names starting from 0 = sunday
extern const char* DAYS[];
extern const char* DAYS_ABBR[];

// Weather params
extern const float LATITUDE;
extern const float LONGITUDE;
extern const char* LANG;
extern const char* UNITS;

// Calendar URLS to show (ical, for example google calendar)
extern char* CALENDAR_URLS[];
extern int NUM_CALENDAR_URLS;

#endif // SETTINGS_H
