#include "PlaybackStructure.h"

void resetPlaybackStructure(PlaybackStructure &data) {
	for (int i=0; i<PLAYBACK_STRUCTURE_NOTE_NUMBER; i++) {
		data.notes[i].on=false;
		data.notes[i].time=0;
	}
	data.eventPtr = 0;
	data.passedFrames = 0;
}

void playAudio(PlaybackStructure &data, float* stream, int len) {
	if (data.playingBack) {}
}


		
		
		  
	

