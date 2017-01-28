#ifndef TUNING_H
#define TUNING_H
#include <vector>
using namespace std;

// both generators are in octaves.
// gen1 is supposed to be the interval of equivalence.
struct Tuning {
	double gen1Size;
	double gen2Size;
	int noteNumber;
};

vector<double> generateMosScale(Tuning const &tuning);

#endif
