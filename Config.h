#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ------------------------------
// How close is “good enough” for LEDs?
// ------------------------------
const float GOOD_TEMP_BAND = 5.0;   // ±°F
const float GOOD_HUM_BAND  = 8.0;   // ±%

// ------------------------------
// Pins
// ------------------------------

// Float switch / safety
#define FLOAT_INTERRUPT_PIN   2

// Relays
#define RELAY_FAN             3
#define RELAY_HEATER          4
#define RELAY_HUMIDIFIER      5

// Status LEDs
#define LED_POWER             6
#define LED_GOOD              7
#define LED_BAD               8

// 74HC595 4-digit 7-segment display pins
#define SCLK_PIN   9    // Shift clock
#define RCLK_PIN   10   // Latch clock
#define TEMP_DIO   11   // Data pin for temperature display
#define HUM_DIO    12   // Data pin for humidity display

// Encoder pins
#define TEMP_ENC_CLK  22
#define TEMP_ENC_DT   23
#define TEMP_ENC_SW   24

#define HUM_ENC_CLK   25
#define HUM_ENC_DT    26
#define HUM_ENC_SW    27

// I2C addresses
#define SHT30_ADDR            0x44

// ------------------------------
// Conversion helper
// ------------------------------
inline float celsiusToFahrenheit(float c) { return c * 9.0 / 5.0 + 32.0; }

// ------------------------------
// Externs for live setpoints
// ------------------------------
extern double setpointTempF;
extern double setpointHum;

#endif
