#ifndef SETTINGS_H
#define SETTINGS_H

extern char* WIFI_SSID;
extern char* WIFI_PASSWORD;
extern char* OPENWEATHERMAP_APIKEY;

// Date and time formats based on strftime
extern const char* DATE_FORMAT;
extern const char* TIME_FORMAT;

// Timezone
extern const long TIMEZONE;

// Localized day names starting from 0 = sunday
extern const char* DAYS[];
extern const char* DAYS_ABBR[];
extern const char* TODAY;
extern const char* TOMORROW;

// Weather params
extern const float LATITUDE;
extern const float LONGITUDE;
extern const char* LANG;
extern const char* UNITS;
extern const char* FEELS_LIKE;
extern const char* WIND_SPEED;
extern const char* WIND_DIRECTION;

// Calendar URLS to show (ical, for example google calendar)
extern char* CALENDAR_URLS[];
extern int NUM_CALENDAR_URLS;

// Google apps script url for calendar (see Code.gs & appsscript.json)
// Does not work out of the box because of HTTPS redirect not working with HTTPClient
// Got around this with proxy server with small PHP script calling the Apps Script and returning response
extern char *GAPPS_SCRIPT_URL;

#endif // SETTINGS_H
