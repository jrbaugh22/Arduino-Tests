// PID_tuning.h
#ifndef PID_TUNING_H
#define PID_TUNING_H

#include <PID_v1.h>

extern PID tempPID;
extern PID humPID;
extern double heatOutput;
extern double humOutput;

const double TEMP_KP = 80.0;
const double TEMP_KI = 4.0;
const double TEMP_KD = 300.0;

const double HUM_KP = 30.0;
const double HUM_KI = 2.0;
const double HUM_KD = 100.0;

const unsigned long PID_SAMPLE_TIME_MS = 1000;

#endif