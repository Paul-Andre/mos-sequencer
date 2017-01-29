#include "ScalePitch.h"
#include "Tuning.h"
#include <stdio.h>

double getPitch(ScalePitch const &note, Tuning const &tuning) {
	int octave = note.scaleDegree / (int)tuning.scale.size();
	int offset = note.scaleDegree % (int)tuning.scale.size();
	if (note.scaleDegree < 0) {
		octave --;
		offset += tuning.scale.size();
	}
//	printf("octave: %d\n", octave);
//	printf("offset: %d\n", offset);
	return octave*tuning.gen1Size + tuning.scale[offset] + note.accidentals*tuning.chroma;
}
