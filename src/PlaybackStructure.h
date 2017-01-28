#ifndef PLAYBACK_STRUCTURE_H
#define PLAYBACK_STRUCTURE_H

struct PlayingNote {
	char on;
	double freq;
	double phase;
	double time;
	int id;
}

struct PlaybackStructure {
	PlayingNote note[16];
	int lastOnNote;
	double beatTime;
}

#endif
