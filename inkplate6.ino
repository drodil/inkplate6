#include "Inkplate.h"

#include "Network.h"
#include "DTWWidget.h"

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

  bool partialUpdate = refreshes % fullRefresh == 0;
  ++refreshes;

  network.init(WIFI_SSID, WIFI_PASSWORD);

  DTWWidget dtw(&display, &network);
  dtw.setColorScheme(WIDGET_COLORSCHEME_LIGHT);
  dtw.setLocation(WIDGET_LOCATION_HALF_LEFT);
  dtw.draw(partialUpdate);

  display.display();
    
  esp_sleep_enable_timer_wakeup(1000L * DELAY_MS);
  (void)esp_deep_sleep_start();
}

void loop() {
  // Nothing here.
}
