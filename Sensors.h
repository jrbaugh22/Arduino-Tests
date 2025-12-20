// Sensors.h
#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Adafruit_SHT31.h>
#include "Config.h"
#include "Display.h"  // needed for tempDisplay.loop() calls

// Shared sensor object and values
extern Adafruit_SHT31 sht30;        // Only defined in Sensors.cpp
extern volatile bool waterOk;
extern double currentTempF;
extern double currentHum;

void initSensors();
void readAllSensors();

#endif
