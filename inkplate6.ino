#include "Inkplate.h"

#include "Network.h"
#include "Secrets.h"

#include "Fonts/Roboto_Light_120.h"
#include "Fonts/Roboto_Light_36.h"
#include "Fonts/Roboto_Light_48.h"

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

  network.init(WIFI_SSID, WIFI_PASSWORD);

  refreshes % fullRefresh == 0 ? display.display() : display.partialUpdate();
  ++refreshes;

  esp_sleep_enable_timer_wakeup(1000L * DELAY_MS);
  (void)esp_deep_sleep_start();
}

void loop() {
  // Nothing here.
}
