#ifndef ARDUINO_ESP32_DEV
#error "Wrong board selection for this project, please select Inkplate 6 in the boards menu."
#endif

#include "Inkplate.h"

#include "Network.h"
#include "DTWWidget.h"
#include "EventListWidget.h"

#define DELAY_MS 60000

// display object
Inkplate display(INKPLATE_1BIT);

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
    display.setTextColor(BLACK);
    display.setCursor(60, 230);
    display.setTextSize(3);
    display.println(F("Welcome!"));
    display.setCursor(60, 280);
    display.println(F("Connecting to WiFi..."));
    display.display();
    delay(1000);
    network.init(WIFI_SSID, WIFI_PASSWORD);
    display.clearDisplay();
  }
}

void loop() {
  bool partialUpdate = refreshes % fullRefresh != 0;
  if(!partialUpdate) {
    display.clearDisplay();
  }
  ++refreshes;

  EventListWidget elw(&display, &network);
  elw.setColorScheme(WIDGET_COLORSCHEME_DARK);
  elw.setLocation(WIDGET_LOCATION_HALF_LEFT);
  elw.draw(partialUpdate);

  DTWWidget dtw(&display, &network);
  dtw.setColorScheme(WIDGET_COLORSCHEME_LIGHT);
  dtw.setLocation(WIDGET_LOCATION_HALF_RIGHT);
  dtw.draw(partialUpdate);

  partialUpdate ? display.partialUpdate() : display.display();
  delay(DELAY_MS);
}
