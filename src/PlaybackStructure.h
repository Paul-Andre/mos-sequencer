#ifndef PLAYBACK_STRUCTURE_H
#define PLAYBACK_STRUCTURE_H

#include <vector>
#include "Note.h"
#include "Tuning.h"

#define PLAYBACK_STRUCTURE_NOTE_NUMBER 16

enum NoteState {
	Off,
	On,
	Decaying
};


struct PlayingNote {
	NoteState state;
	double freq;
	double phase;
	int passedFrames;
	int id;
	float volume;
};

struct NoteEvent {
	bool on;
	int id;
	int timeStamp;
	double freq;
};

struct PlaybackStructure {
	NoteState state;
	PlayingNote notes[PLAYBACK_STRUCTURE_NOTE_NUMBER];
	int lastOnNote;
	double beatTime;
	std::vector<NoteEvent> *events;
	int eventPtr;
	int passedFrames;
	int onNotes;
};

void resetPlaybackStructure(PlaybackStructure &data);

void playAudio(PlaybackStructure &data, float* stream, int len);

vector<NoteEvent> makeEventStream(vector<Note> const &notes, int beat, Tuning const &tuning);

#endif
