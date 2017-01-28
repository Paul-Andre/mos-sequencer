#ifndef NOTE_H
#define NOTE_H
#include "Tuning.h"

struct Note {
	double start;
	double duration;
	int gen1;
	int gen2;
};

double getPitch(Note const &note, Tuning const &tuning) {
	return note.gen1*tuning.gen1Size + note.gen2*tuning.gen2Size;
}

#endif

