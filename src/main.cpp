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
	SDL_AudioSpec want, have;
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

	for(int i=0; i<tuning.scale.size(); i++) {
		printf("Debug scale %d: %f\n", i, tuning.scale[i]*12);
	}
	printf("Chroma: %f\n", tuning.chroma*12);

	// Display default window
	position.x = 0;
	position.y = 1;
	position.w = 4;
	position.h = 2; // Default octave number

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	draw(position, tuning, notes, renderer);

	PlaybackStructure userdata;

	SDL_zero(want);
	want.callback = audio_callback;
	want.freq = 44100;
	want.format = AUDIO_F32SYS;
	want.channels = 2;
	want.samples = 1024;
	want.userdata = &userdata;

	if( (dev = SDL_OpenAudioDevice(NULL,
				            0,
							&want,
							&have,
							SDL_AUDIO_ALLOW_FORMAT_CHANGE)) <= 0 ){

		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	// Unpausing audio
	//SDL_PauseAudioDevice(dev, 0);

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
			if(e.type == SDL_MOUSEWHEEL) {
				// do something
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

