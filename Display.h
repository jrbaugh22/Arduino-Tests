// Display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <DIYables_4Digit7Segment_74HC595.h>

// Extern declarations — allows other files to access these
extern DIYables_4Digit7Segment_74HC595 tempDisplay;
extern DIYables_4Digit7Segment_74HC595 humDisplay;

void initDisplays();
void update7SegmentDisplays();

#endif
