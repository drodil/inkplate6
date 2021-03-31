#ifndef ARDUINO_ESP32_DEV
#error "Wrong board selection for this project, please select Inkplate 6 in the boards menu."
#endif

#include "Inkplate.h"

#include "Network.h"
#include "DTWWidget.h"
#include "EventListWidget.h"

#define DELAY_MS 60000 * 3

// display object
Inkplate display(INKPLATE_3BIT);

// network object
Network network;

// variable for counting partial refreshes
RTC_DATA_ATTR unsigned refreshes = 0;

// constant to determine when to full update
const int fullRefresh = 5;

void setup() {
  Serial.begin(115200);
  display.begin();
  
  display.setTextWrap(false);

  if (refreshes == 0)
  {
    display.setCursor(5, 230);
    display.setTextSize(2);
    display.println(F("Hello and welcome to Inkplate!"));
    display.setCursor(5, 250);
    display.println(F("Connecting to WiFi..."));
    display.display();
  }

  delay(5000);
  bool partialUpdate = refreshes % fullRefresh == 0;
  ++refreshes;

  network.init(WIFI_SSID, WIFI_PASSWORD);
  display.clearDisplay();

  DTWWidget dtw(&display, &network);
  dtw.setColorScheme(WIDGET_COLORSCHEME_LIGHT);
  dtw.setLocation(WIDGET_LOCATION_HALF_LEFT);
  dtw.draw(partialUpdate);

  EventListWidget elw(&display, &network);
  elw.setColorScheme(WIDGET_COLORSCHEME_DARK);
  elw.setLocation(WIDGET_LOCATION_HALF_RIGHT);
  elw.draw(partialUpdate);

  display.display();
    
  esp_sleep_enable_timer_wakeup(1000L * DELAY_MS);
  (void)esp_deep_sleep_start();
}

void loop() {
  // Nothing here.
}
