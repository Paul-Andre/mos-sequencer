#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <SDL2/SDL.h>

#include "Tuning.h"
#include "Note.h"
#include "waveforms.h"
#include "PianoRollPosition.h"
#include "PlaybackStructure.h"
#include "draw.h"

using namespace std;


void audio_callback(void *userdata, Uint8 *stream, int len);

static Uint8 *audio_pos;
static Uint32 audio_len;
double phase;

int main(int argc, char **argv) {
	int k=0;
	PianoRollPosition position;
	Tuning tuning = generateMosScale( 1., 7./12., 7);
	vector<Note> notes;
	notes.push_back({0,0.5, (ScalePitch) {1,-1}});
	notes.push_back({0.5,0.5, (ScalePitch) {2,-1}});
	notes.push_back({1,0.5, (ScalePitch) {1,-1}});
	notes.push_back({1.5,0.5, (ScalePitch) {2,-1}});
	notes.push_back({1,0.5, (ScalePitch) {3,0}});
	notes.push_back({1.5,0.5, (ScalePitch) {3,1}});

	SDL_AudioSpec want, have;
	notes.push_back({0,0.5, (ScalePitch) {0,0}});

	SDL_AudioDeviceID dev;
	bool quit = false;
	SDL_Event e;

	

	// Init SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Sequencer",
			             SDL_WINDOWPOS_UNDEFINED,
						 SDL_WINDOWPOS_UNDEFINED,
						 640, 480,
						 SDL_WINDOW_RESIZABLE);



	// Display default window
	position.x = 0;
	position.y = 1;
	position.w = 4;
	position.h = 2; // Default octave number

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	draw(position, tuning, notes, renderer);

	PlaybackStructure userdata;

	resetPlaybackStructure(userdata);

	int beat = 44100;

	vector<NoteEvent> events = makeEventStream(notes, beat, tuning);
	userdata.events = &events;
	userdata.state = On;

	SDL_zero(want);
	want.callback = audio_callback;
	want.freq = 44100;
	want.format = AUDIO_F32SYS;
	want.channels = 2;
	want.samples = 1024;
	want.userdata = &userdata;

	/*if( (dev = SDL_OpenAudioDevice(NULL,
				            0,
							&want,
							&have,
							SDL_AUDIO_ALLOW_FORMAT_CHANGE)) <= 0 ){

		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}*/

	// Unpausing audio
	SDL_PauseAudioDevice(dev, 0);

	// Event loop
	while(!quit) {
		if(SDL_WaitEventTimeout(&e, 50)){
			if(e.type == SDL_QUIT)
				quit = true;
			if(e.type == SDL_KEYDOWN) {
				SDL_Rect rect;

				rect.x = k*10;
				rect.y = k*10;
				rect.w = 32;
				rect.h = 32;
				k++;

				SDL_RenderDrawRect(renderer, &rect);
				SDL_RenderPresent(renderer);
			}

            const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

			if(e.type == SDL_MOUSEWHEEL && currentKeyStates[SDL_SCANCODE_LCTRL]) {
				position.w += (0.5*e.wheel.y);
			}
			else if (e.type == SDL_MOUSEWHEEL && !currentKeyStates[SDL_SCANCODE_LCTRL]){
				position.h += (0.5*e.wheel.y);

			}

		}
		draw(position, tuning, notes, renderer);
	}

	

	printf("quitting\n");
	SDL_CloseAudioDevice(dev);


}

void audio_callback(void *data_, Uint8 *stream, int len){
	float *out = (float*) stream;
	PlaybackStructure *data = (PlaybackStructure*) data_;

	playAudio(*data, out, len/2/sizeof(float));


}

