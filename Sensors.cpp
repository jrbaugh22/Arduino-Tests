// Sensors.cpp — NON-BLOCKING, display-friendly version
#include "Sensors.h"

Adafruit_SHT31 sht30 = Adafruit_SHT31();
volatile bool waterOk = true;
double currentTempF = 0.0;
double currentHum   = 0.0;

void initSensors() {
  if (!sht30.begin(SHT30_ADDR)) {
    Serial.println(F("SHT30 not found!"));
    while (1);
  }
  pinMode(FLOAT_INTERRUPT_PIN, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(FLOAT_INTERRUPT_PIN), floatSwitchISR, FALLING);
}

void readAllSensors() {
  // Read SHT30
  float t = sht30.readTemperature();
  float h = sht30.readHumidity();

  // Update values if valid
  if (!isnan(t) && !isnan(h)) {
    currentTempF = celsiusToFahrenheit(t);
    currentHum   = h;
  }

  // Optional: call display loop in case sensor read ever blocks
  tempDisplay.loop();
  humDisplay.loop();

  // Float switch (if needed)
  //static bool last = true;
  //bool now = digitalRead(FLOAT_INTERRUPT_PIN);
  //if (!last && now) waterOk = true;
  //last = now;
}
