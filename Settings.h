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
extern const char* DAY0;
extern const char* DAY1;
extern const char* DAY2;
extern const char* DAY3;
extern const char* DAY4;
extern const char* DAY5;
extern const char* DAY6;

// Weather params
extern const float LATITUDE;
extern const float LONGITUDE;
extern const char* LANG;
extern const char* UNITS;

#endif // SETTINGS_H
