// Display.cpp
#include "Config.h"
#include "Sensors.h"
#include "Display.h"
#include "Encoders.h"

DIYables_4Digit7Segment_74HC595 tempDisplay(SCLK_PIN, RCLK_PIN, TEMP_DIO);
DIYables_4Digit7Segment_74HC595 humDisplay (SCLK_PIN, RCLK_PIN, HUM_DIO);

// Blink timing
unsigned long lastBlinkTime = 0;
const unsigned long BLINK_INTERVAL = 500;  // 500ms on / 500ms off = 1 second cycle
bool decimalOn = true;

// Track last shown setpoints to avoid unnecessary updates
static double lastTempSetpointShown = -999.0;
static double lastHumSetpointShown  = -999.0;

// Track previous mode for forced refresh on change
static bool prevInSetMode = false;
static bool prevIsTempMode = false;

void initDisplays() {
  tempDisplay.printInt(8888, true);
  humDisplay.printInt(8888, true);
  delay(1000);

  // Initial normal display
  tempDisplay.printFloat(currentTempF, 1, true);
  humDisplay.printFloat(currentHum, 1, true);

  lastTempSetpointShown = -999.0;
  lastHumSetpointShown  = -999.0;
}

void update7SegmentDisplays() {
  unsigned long now = millis();

  bool modeChanged = (inSetMode != prevInSetMode) || (isTempMode != prevIsTempMode);

  // -----------------------------
  // Always update the NON-editing display with current values
  // -----------------------------
  if (inSetMode) {
    if (isTempMode) {
      // Editing temp → humidity shows current humidity steadily
      humDisplay.printFloat(currentHum, 1, true);
    } else {
      // Editing humidity → temp shows current temp steadily
      tempDisplay.printFloat(currentTempF, 1, true);
    }
  } else {
    // Normal mode → both show current values steadily
    tempDisplay.printFloat(currentTempF, 1, true);
    humDisplay.printFloat(currentHum, 1, true);
  }

  // -----------------------------
  // Handle the EDITING display (setpoint with blinking decimal)
  // -----------------------------
  if (inSetMode) {
    // Toggle blink state
    if (now - lastBlinkTime >= BLINK_INTERVAL) {
      decimalOn = !decimalOn;
      lastBlinkTime = now;
      modeChanged = true;  // Force update on blink change
    }

    if (isTempMode) {
      // Only update if setpoint changed, blink changed, or mode just entered/switched
      if (modeChanged || (setpointTempF != lastTempSetpointShown)) {
        tempDisplay.printFloat(setpointTempF, 1, decimalOn);
        lastTempSetpointShown = setpointTempF;
      }
    } else {
      if (modeChanged || (setpointHum != lastHumSetpointShown)) {
        humDisplay.printFloat(setpointHum, 1, decimalOn);
        lastHumSetpointShown = setpointHum;
      }
    }
  } else {
    // Exited set mode → reset tracking so next entry refreshes properly
    lastTempSetpointShown = -999.0;
    lastHumSetpointShown  = -999.0;
    decimalOn = true;
  }

  prevInSetMode = inSetMode;
  prevIsTempMode = isTempMode;

  // Always call loop() for multiplexing
  tempDisplay.loop();
  humDisplay.loop();
}