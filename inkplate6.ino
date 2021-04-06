#ifndef ARDUINO_ESP32_DEV
#error "Wrong board selection for this project, please select Inkplate 6 in the boards menu."
#endif

#include "Inkplate.h"

#include "Network.h"
#include "DTWWidget.h"
#include "GCalEventListWidget.h"

#define DELAY_MS 60000

// display object
Inkplate display(INKPLATE_1BIT);

// network object
Network network;

// variable for counting partial refreshes
RTC_DATA_ATTR unsigned refreshes = 0;

// constant to determine when to full update
const int fullRefresh = 10;

// Set up the board
void setup() {
  Serial.begin(115200);
  // This must be called before anything else
  display.begin();

  // Set rotation of the screen 1-4 (1 & 3 = portrait, 2 & 4 = landscape)
  // If changing to portrait, change Widget.cpp resolution as well
  display.setRotation(2);
  display.setTextWrap(false);

  // Partial update only works with 1bit mode
  display.setDisplayMode(INKPLATE_1BIT);

  // Initial screen and network setup
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
  // Only do partial update every fullRefresh interval (minutes)
  if(!partialUpdate) {
    display.clearDisplay();
  }
  ++refreshes;

  // Start drawing widgets
  GCalEventListWidget elw(&display, &network);
  elw.setColorScheme(WIDGET_COLORSCHEME_DARK);
  elw.setLocation(WIDGET_LOCATION_HALF_LEFT);
  elw.draw(partialUpdate);

  DTWWidget dtw(&display, &network);
  dtw.setColorScheme(WIDGET_COLORSCHEME_LIGHT);
  dtw.setLocation(WIDGET_LOCATION_HALF_RIGHT);
  dtw.draw(partialUpdate);

  // Display and delay for next loop
  partialUpdate ? display.partialUpdate() : display.display();
  delay(DELAY_MS);
}
