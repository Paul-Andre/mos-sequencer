#ifndef TUNING_H
#define TUNING_H
#include <vector>
#include "ScalePitch.h"
using namespace std;

// both generators are in octaves.
// gen1 is supposed to be the interval of equivalence.
struct Tuning {
	double gen1Size;
	double gen2Size;
	vector<double> scale;
	double chroma;
};

Tuning generateMosScale(double gen1Size, double gen2Size, int noteNumber);
vector<ScalePitch> pitchesInWindow(Tuning const &tuning, double y, double h);

#endif
