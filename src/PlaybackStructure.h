#ifndef PLAYBACK_STRUCTURE_H
#define PLAYBACK_STRUCTURE_H

#include <vector>

struct PlayingNote {
	bool on;
	double freq;
	double phase;
	double time;
	int id;
};

struct NoteEvent {
	bool on;
	int id;
};

struct PlaybackStructure {
	bool playingBack;
	PlayingNote note[16];
	int lastOnNote;
	double beatTime;
	std::vector<NoteEvent> events;
	int eventPtr;
};

#endif
