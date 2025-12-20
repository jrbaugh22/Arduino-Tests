// Control.cpp
#include "Control.h"
#include "Sensors.h"
#include "PID_tuning.h"
#include "Config.h"
#include "Encoders.h"  // For live setpoints

PID tempPID(&currentTempF, &heatOutput, &setpointTempF, TEMP_KP, TEMP_KI, TEMP_KD, DIRECT);
PID humPID (&currentHum,   &humOutput,  &setpointHum,   HUM_KP,  HUM_KI,  HUM_KD,  DIRECT);

double heatOutput = 0;
double humOutput  = 0;
unsigned long lastPIDTime = 0;

void initControl() {
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_HEATER, OUTPUT);
  pinMode(RELAY_HUMIDIFIER, OUTPUT);
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_GOOD, OUTPUT);
  pinMode(LED_BAD, OUTPUT);

  digitalWrite(LED_POWER, HIGH);
  digitalWrite(LED_BAD, HIGH);

  tempPID.SetMode(AUTOMATIC);
  tempPID.SetOutputLimits(0, 255);
  tempPID.SetSampleTime(PID_SAMPLE_TIME_MS);

  humPID.SetMode(AUTOMATIC);
  humPID.SetOutputLimits(0, 255);
  humPID.SetSampleTime(PID_SAMPLE_TIME_MS);
}

void runPIDControllers() {
  unsigned long now = millis();
  if (now - lastPIDTime < PID_SAMPLE_TIME_MS) return;
  lastPIDTime = now;

  tempPID.Compute();
  if (heatOutput > 5) {
    digitalWrite(RELAY_FAN, HIGH);
    analogWrite(RELAY_HEATER, (int)heatOutput);
  } else {
    digitalWrite(RELAY_FAN, LOW);
    digitalWrite(RELAY_HEATER, LOW);
  }

  humPID.Compute();
  static unsigned long windowStart = 0;
  const unsigned long windowSize = 10000;

  if (now - windowStart > windowSize) windowStart += windowSize;

  //if (waterOk && humOutput > 5) {
    if (humOutput > 5){ //temporary:  allow humidifier
    if (now - windowStart < (humOutput / 255.0) * windowSize)
      digitalWrite(RELAY_HUMIDIFIER, HIGH);
    else
      digitalWrite(RELAY_HUMIDIFIER, LOW);
  } else {
    digitalWrite(RELAY_HUMIDIFIER, LOW);
  }
}

void updateStatusLEDs() {
  bool allGood = (abs(currentTempF - setpointTempF) <= GOOD_TEMP_BAND) &&
                 (abs(currentHum   - setpointHum)   <= GOOD_HUM_BAND); //waterOk;
  digitalWrite(LED_GOOD, allGood ? HIGH : LOW);
  digitalWrite(LED_BAD,  allGood ? LOW  : HIGH);
}