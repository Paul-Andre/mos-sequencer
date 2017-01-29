#include "PlaybackStructure.h"
#include "waveforms.h"
#include "Note.h"
#include "ScalePitch.h"
#include <string.h>
#include <vector>
#include <algorithm>
using namespace std;

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
	for (int t=0; t<len; t++) {
		if (data.state == On) {
			while (data.passedFrames >= (((data.events))[data.eventPtr]).timeStamp ) {
				NoteEvent *event = &((data.events))[data.eventPtr];
				if (event->on) {
					int nextOffNote = findNextOffNote(data.notes,data.lastOnNote);
					data.lastOnNote = nextOffNote;
					PlayingNote *note = &data.notes[nextOffNote];
					note->state = On;
					note->freq = event->freq;
					note->phase = 0;
					note->volume = 1;
					note->passedFrames = 0;
					note->id = event->id;
					data.onNotes++;
				}
				else{
					for (int i=0; i<PLAYBACK_STRUCTURE_NOTE_NUMBER; i++) {
							//printf("the note should be decaying\n");
						if (data.notes[i].id == event->id && data.notes[i].state == On) {
							data.notes[i].state = Decaying;
						}
					}
				}

				data.eventPtr++;
				if (data.eventPtr>=data.events.size()){
					data.state = Decaying;
				}
			}
		}
		//if (data.state!=Off) {
			double a = 0;
			for (int i=0; i<PLAYBACK_STRUCTURE_NOTE_NUMBER; i++) {
				PlayingNote *note = &data.notes[i];
				if(note->state!=Off) {
					note->phase+=note->freq/44100;
					double p = note->phase;
					a += (
							triangleFunction(p*0.5)*0.4
							+triangleFunction(p)
							+triangleFunction(p*2)
							+triangleFunction(p*3)*0.5
							+triangleFunction(p*4)*0.7
							+triangleFunction(p*8)*0.4
							+triangleFunction(p*16)*0.4*note->volume
						 )
						*0.2*note->volume;
				}
				if (note->state == Decaying) {
					note->volume*=0.9999;
					if (note->volume < 0.000001) {
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
			/*
		}
		else {
			stream[2*t] = 0;
			stream[2*t+1] = 0;
		}
		*/
		data.passedFrames+=1;
	}
}

bool eventCmp(const NoteEvent &a, const NoteEvent &b) {
	return a.timeStamp < b.timeStamp;
}

vector<NoteEvent> makeEventStream(vector<Note> const &notes, int beat, Tuning const &tuning) {
	vector<NoteEvent> events(2*notes.size());
	for(int i=0; i<notes.size(); i++){
		Note const &node = notes[i];
		events[i*2]= (
				(NoteEvent) { 1, i, node.start*beat,
				220.*exp2(getPitch(node.scalePitch, tuning))});
		events[i*2+1]= ((NoteEvent) { 0, i, (node.start+node.duration)*beat, 0});

	}

	std::sort(events.begin(), events.end(), eventCmp);
	for(int i =0; i<events.size(); i++){
		printf("on: %d, id: %d, timestamp: %d\n",events[i].on, events[i].id, events[i].timeStamp);
	}
	return events;
}




