#ifndef SCALE_PITCH_H
#define SCALE_PITCH_H
#include "Tuning.h"
struct Tuning;

struct ScalePitch {
	int scaleDegree;
	int accidentals;
};

double getPitch(ScalePitch const &scalePitch, Tuning const &tuning);

#endif


