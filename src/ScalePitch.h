#ifndef SCALE_PITCH_H
#define SCALE_PITCH_H
#include "Tuning.h"

struct ScalePitch {
	int gen1;
	int gen2;
};

double getPitch(ScalePitch const &scalePitch, Tuning const &tuning);

#endif

