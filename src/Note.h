#ifndef NOTE_H
#define NOTE_H
#include "Tuning.h"
#include "ScalePitch.h"

struct Note {
	double start;
	double duration;
	ScalePitch scalePitch;
};

#endif

