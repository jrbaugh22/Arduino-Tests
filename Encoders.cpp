// Encoders.cpp — NON-BLOCKING, DISPLAY-SAFE VERSION

#include "Encoders.h"
#include "Display.h"
#include "Config.h"
#include <EEPROM.h>
#include <Encoder.h>

// EEPROM addresses
#define EEPROM_ADDR_TEMP  0
#define EEPROM_ADDR_HUM   4

// Encoder objects
Encoder tempEncoder(TEMP_ENC_CLK, TEMP_ENC_DT);
Encoder humEncoder (HUM_ENC_CLK,  HUM_ENC_DT);

// Setpoints (loaded from EEPROM on startup)
double setpointTempF = 77.0;
double setpointHum   = 50.0;

// Edit-mode state
bool inSetMode        = false;
bool isTempMode       = false;          // true = editing temp, false = editing humidity
unsigned long setModeTimer = 0;

// Last encoder positions (separate for each to prevent interference)
long lastTempEncoderPos = 0;
long lastHumEncoderPos  = 0;

// ------------------------------------------------
// Load setpoints from EEPROM on startup
// ------------------------------------------------
void loadSetpointsFromEEPROM() {
  EEPROM.get(EEPROM_ADDR_TEMP, setpointTempF);
  EEPROM.get(EEPROM_ADDR_HUM,  setpointHum);

  // Sanity check: if values are invalid (e.g. first boot), use defaults
  if (setpointTempF < 50.0 || setpointTempF > 100.0) setpointTempF = 77.0;
  if (setpointHum   < 20.0 || setpointHum   > 90.0)  setpointHum   = 50.0;
}

// ------------------------------------------------
// Save setpoints to EEPROM
// ------------------------------------------------
void saveSetpointsToEEPROM() {
  EEPROM.put(EEPROM_ADDR_TEMP, setpointTempF);
  EEPROM.put(EEPROM_ADDR_HUM,  setpointHum);
}

// ------------------------------------------------
// Enter or exit setpoint edit mode
// ------------------------------------------------
void toggleSetMode(bool requestedTempMode) {
  if (inSetMode) {
    // Already in set mode
    if (isTempMode == requestedTempMode) {
      // Pressed the same button again → exit
      inSetMode = false;
    } else {
      // Switched to the other parameter
      isTempMode = requestedTempMode;
      setModeTimer = millis();
      // Reset last position for the newly active encoder
      if (requestedTempMode) {
        lastTempEncoderPos = tempEncoder.read();
      } else {
        lastHumEncoderPos = humEncoder.read();
      }
    }
  } else {
    // Enter set mode for the requested parameter
    inSetMode = true;
    isTempMode = requestedTempMode;
    setModeTimer = millis();
    lastTempEncoderPos = tempEncoder.read();
    lastHumEncoderPos  = humEncoder.read();
  }
}

// ------------------------------------------------
// Initialization
// ------------------------------------------------
void initEncoders() {
  pinMode(TEMP_ENC_SW, INPUT_PULLUP);
  pinMode(HUM_ENC_SW,  INPUT_PULLUP);

  loadSetpointsFromEEPROM();  // Load saved values
}

// ------------------------------------------------
// Main encoder handler — call EVERY loop()
// ------------------------------------------------
void checkEncoders() {

  // ------------------------------
  // Button handling with debounce
  // ------------------------------
  static unsigned long lastTempBtnTime = 0;
  static unsigned long lastHumBtnTime  = 0;
  const unsigned long debounceDelay    = 50;

  bool tempBtnPressed = (digitalRead(TEMP_ENC_SW) == LOW);
  bool humBtnPressed  = (digitalRead(HUM_ENC_SW)  == LOW);

  if (tempBtnPressed && millis() - lastTempBtnTime > debounceDelay) {
    lastTempBtnTime = millis();
    toggleSetMode(true);   // Request temperature edit mode
  }

  if (humBtnPressed && millis() - lastHumBtnTime > debounceDelay) {
    lastHumBtnTime = millis();
    toggleSetMode(false);  // Request humidity edit mode
  }

  // ------------------------------
  // Only handle rotation if in set mode
  // ------------------------------
  if (!inSetMode) return;

  long currentPos;
  long& lastPos = isTempMode ? lastTempEncoderPos : lastHumEncoderPos;
  Encoder& activeEncoder = isTempMode ? tempEncoder : humEncoder;

  currentPos = activeEncoder.read();

  if (currentPos != lastPos) {
    long delta = currentPos - lastPos;
    double step = isTempMode ? 1.0 : 1.0;  // 1°F or 1%

    double oldValue = isTempMode ? setpointTempF : setpointHum;
    double newValue = oldValue + (delta > 0 ? -step : step);

    if (isTempMode) {
      setpointTempF = constrain(newValue, 50.0, 100.0);
    } else {
      setpointHum = constrain(newValue, 20.0, 90.0);
    }

    // Save only if actually changed
    if (abs(newValue - oldValue) >= 0.01) {
      saveSetpointsToEEPROM();
    }

    lastPos = currentPos;
    setModeTimer = millis();  // Reset inactivity timer
  }

  // ------------------------------
  // Auto-exit after 8 seconds of inactivity
  // ------------------------------
  if (millis() - setModeTimer > 8000) {
    inSetMode = false;
  }
}