#ifndef WAVEFORMS_H
#define WAVEFORMS_H
#include <math.h>
#include <stdlib.h>

float sawFunction (float x);
float triangleFunction (float x);
float squareFunction (float x);
float rectangleFunction (float x, float pulse);
float sineFunction (float x);
float zeroFunction (float x);
float loopModFunction (float x);
float clampFunction (float x);


#endif
