#include "ScalePitch.h"
#include "Tuning.h"

double getPitch(ScalePitch const &scalePitch, Tuning const &tuning) {
	return note.gen1*tuning.gen1Size + note.gen2*tuning.gen2Size;
}
