#ifndef PLAYBACK_STRUCTURE_H
#define PLAYBACK_STRUCTURE_H

#include <vector>
#define PLAYBACK_STRUCTURE_NOTE_NUMBER 16



struct PlayingNote {
	bool on;
	double freq;
	double phase;
	int passedFrames;
	int id;
};

struct NoteEvent {
	bool on;
	int id;
};

struct PlaybackStructure {
	bool playingBack;
	PlayingNote note[PLAYBACK_STRUCTURE_NOTE_NUMBER];
	int lastOnNote;
	double beatTime;
	std::vector<NoteEvent> *events;
	int eventPtr;
	int passedFrames;
};

void playAudio(PlaybackStructure &data, float* stream, int len);

#endif
