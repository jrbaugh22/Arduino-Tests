// Encoders.h
#ifndef ENCODERS_H
#define ENCODERS_H

#include <Encoder.h>

// Pin definitions
#define TEMP_ENC_CLK   22
#define TEMP_ENC_DT    24
#define TEMP_ENC_SW    26

#define HUM_ENC_CLK    28
#define HUM_ENC_DT     30
#define HUM_ENC_SW     32

// External encoder objects (defined in Encoders.cpp)
extern Encoder tempEncoder;
extern Encoder humEncoder;

// External setpoints and mode state (used by Display.cpp and others)
extern double setpointTempF;
extern double setpointHum;

extern bool inSetMode;
extern bool isTempMode;        // <-- THIS LINE WAS MISSING!

// Functions
void initEncoders();
void checkEncoders();

#endif