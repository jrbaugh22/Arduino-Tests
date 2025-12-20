// CuringCabinet.ino
#include "Config.h"
#include "Sensors.h"
#include "Display.h"
#include "Control.h"
#include "Encoders.h"
#include <EEPROM.h>

// EEPROM addresses — 8 bytes total
#define EEPROM_ADDR_TEMP  0   // 4 bytes for double
#define EEPROM_ADDR_HUM   4   // 4 bytes for double

void setup() {
  Serial.begin(9600);

  // Load last-used setpoints from EEPROM (or use defaults on first flash)
  EEPROM.get(EEPROM_ADDR_TEMP, setpointTempF);
  EEPROM.get(EEPROM_ADDR_HUM,  setpointHum);
  if (isnan(setpointTempF) || setpointTempF < 50 || setpointTempF > 100) setpointTempF = 77.0;
  if (isnan(setpointHum)   || setpointHum   < 20 || setpointHum   > 90) setpointHum   = 50.0;

  initSensors();
  initDisplays();
  initControl();
  initEncoders();

  Serial.print(F("Curing Cabinet started — Temp: "));
  Serial.print(setpointTempF, 1);
  Serial.print(F("°F  Hum: "));
  Serial.print(setpointHum, 1);
  Serial.println(F("% (loaded from EEPROM)"));
}

void loop() {
  // ------------------------------
  // Core system functions
  // ------------------------------
  readAllSensors();
  checkEncoders();
  runPIDControllers();
  update7SegmentDisplays();
  updateStatusLEDs();

  // ------------------------------
  // Critical: continuous multiplex refresh for DIYables displays
  // ------------------------------
  tempDisplay.loop();
  humDisplay.loop();
}
