#include "PlaybackStructure.h"
#include "waveforms.h"
#include <string.h>

void resetPlaybackStructure(PlaybackStructure &data) {
	for (int i=0; i<PLAYBACK_STRUCTURE_NOTE_NUMBER; i++) {
		data.notes[i].state = Off;
		data.notes[i].passedFrames=0;
		data.notes[i].phase=0;
	}
	data.eventPtr = 0;
	data.passedFrames = 0;
	data.onNotes = 0;
}

int findNextOffNote(PlayingNote const *notes, int lastOnNote) {
	int decaying=-1;
	for (int i=0; i<PLAYBACK_STRUCTURE_NOTE_NUMBER; i++) {
		if (notes[i].state == Off) return i;
		if (notes[i].state == Decaying) decaying = i;
	}
	if (decaying!=-1) return decaying;
	return (lastOnNote+1)%PLAYBACK_STRUCTURE_NOTE_NUMBER;
}


void playAudio(PlaybackStructure &data, float* stream, int len) {
	if (data.state = On) {
		while (data.passedFrames >= ((*(data.events))[data.eventPtr]).timeStamp ) {
			NoteEvent *event = &(*(data.events))[data.eventPtr];
			int nextOffNote = findNextOffNote(data.notes,data.lastOnNote);
			data.lastOnNote = nextOffNote;
			PlayingNote *note = &data.notes[nextOffNote];
			if (event->on) {
				note->state = On;
				note->freq = event->freq;
				note->phase = 0;
				note->volume = 1;
				note->passedFrames = 0;
				note->id = event->id;
				data.onNotes++;
			}
			else{
				note->state = Decaying;
			}

			data.eventPtr++;
			if (data.eventPtr>=data.events->size()){
			   	data.state = Decaying;
			}
		}
	}
	if (data.state!=Off) {
		for (int t=0; t<len; t++) {
			double a;
			for (int i=0; i<PLAYBACK_STRUCTURE_NOTE_NUMBER; i++) {
				PlayingNote *note = &data.notes[i];
				if(note->state!=Off) {
					note->phase+=note->freq/44100;
					double p = note->phase;
					a += (
							triangleFunction(p)
							+triangleFunction(p*2)
							+triangleFunction(p*2)
							+triangleFunction(p*3)*0.5
							+triangleFunction(p*4)*0.5
						 )
						*0.2*note->volume;
				}
				if (note->state == Decaying) {
					note->volume*=0.99999;
					if (note->volume < 0.00001) {
						note->state = Off;
						data.onNotes--;
						if (data.onNotes==0 && data.state == Decaying) {
							data.state = Off;
						}
					} 
				}
			}
			stream[2*t] = a;
			stream[2*t+1] = a;
		}
	}
	else {
		memset(stream, 0, len*2*sizeof(float));
	}
	data.passedFrames+=len;
}

