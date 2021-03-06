#include "Settings.h"

// Authentication to services & WiFi setup
char* WIFI_SSID = "";
char* WIFI_PASSWORD = "";
char* OPENWEATHERMAP_APIKEY = "";

// Date and time formats based on strftime
const char* DATE_FORMAT = "%d.%m.%Y";
const char* TIME_FORMAT = "%H:%M";

// Timezone
const long TIMEZONE = 3L;

// Localized day names starting from 0 = sunday
const char* DAYS[] = {"Sunnuntai", "Maanantai", "Tiistai", "Keskiviikko", "Torstai", "Perjantai", "Lauantai"};
const char* DAYS_ABBR[] = {"Su", "Ma", "Ti", "Ke", "To", "Pe", "La"};
const char* TODAY = "Tanaan";
const char* TOMORROW = "Huomenna";

// Weather params (see https://openweathermap.org/api/one-call-api)
const float LATITUDE = 64.9129;
const float LONGITUDE = 25.5035;
const char* LANG = "fi";
const char* UNITS = "metric";
const char* FEELS_LIKE = "Tuntuu kuin";
const char* WIND_SPEED = "Tuulen nopeus";
const char* WIND_DIRECTION = "Tuulen suunta";
const char* HUMIDITY = "Kosteus";

char *CALENDAR_URLS[1] = {};
int NUM_CALENDAR_URLS = 0;

char *GAPPS_SCRIPT_URL = "";
