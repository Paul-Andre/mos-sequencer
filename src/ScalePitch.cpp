#include "ScalePitch.h"
#include "Tuning.h"

double getPitch(ScalePitch const &note, Tuning const &tuning) {
	int octave = note.scaleDegree / tuning.scale.size();
	int offset = note.scaleDegree % tuning.scale.size();
	if (note.scaleDegree < 0) {
		octave --;
		offset += tuning.scale.size();
	}
	return octave*tuning.gen1Size + tuning.scale[offset] + note.accidentals*tuning.chroma;
}
