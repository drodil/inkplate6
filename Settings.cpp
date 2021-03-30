#include "Settings.h"

// Authentication to services & WiFi setup
char* WIFI_SSID = "";
char* WIFI_PASSWORD = "";
char* OPENWEATHERMAP_APIKEY = "";

// Date and time formats based on strftime
const char* DATE_FORMAT = "%d.%m.%Y";
const char* TIME_FORMAT = "%H:%M";

// Timezone
const long TIMEZONE = 2L;

// Localized day names starting from 0 = sunday
const char* DAY0 = "Sunnuntai";
const char* DAY1 = "Maanantai";
const char* DAY2 = "Tiistai";
const char* DAY3 = "Keskiviikko";
const char* DAY4 = "Torstai";
const char* DAY5 = "Perjantai";
const char* DAY6 = "Lauantai";

// Weather params (see https://openweathermap.org/api/one-call-api)
const float LATITUDE = 65.0;
const float LONGITUDE = 35.0;
const char* LANG = "fi";
const char* UNITS = "metric";
