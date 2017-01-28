#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Tuning.h"
#include "Note.h"
#include "waveforms.h"
#include "PianoRollPosition.h"
//#include "draw.h"
#include <math.h>
using namespace std;

#include <SDL2/SDL.h>
#define MUS_PATH ""

void audio_callback(void *userdata, Uint8 *stream, int len);

static Uint8 *audio_pos;
static Uint32 audio_len;

int main(int argc, char **argv) {
	PianoRollPosition postion;
	Tuning tuning = generateMosScale( 1., log2(7./12.), 7);
	vector<Note> notes;


	SDL_AudioSpec want, have;
	SDL_AudioDeviceID dev;
	bool quit = false;
	SDL_Event e;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Sequencer",
			             SDL_WINDOWPOS_UNDEFINED,
						 SDL_WINDOWPOS_UNDEFINED,
						 640, 480,
						 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderPresent(renderer);

	double phase = 0;
	want.callback = audio_callback;
	want.freq = 48000;
	want.format = AUDIO_F32SYS;
	want.channels = 2;
	want.samples = 1024;
	want.userdata = &phase;

	if( dev = SDL_OpenAudioDevice(NULL,
				            0,
							&want,
							&have,
							SDL_AUDIO_ALLOW_FORMAT_CHANGE) < 0 ){

		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	SDL_PauseAudioDevice(dev, 0);

	//----------------------Event loop-------------------------------
	while(!quit) {
		if(SDL_WaitEventTimeout(&e, 50)){
			if(e.type == SDL_QUIT)
				quit = true;
			if(e.type == SDL_KEYDOWN) {
				SDL_Rect rect;

				rect.x = 0;
				rect.y = 0;
				rect.w = 32;
				rect.h = 32;

				SDL_RenderDrawRect(renderer, &rect);
				SDL_RenderPresent(renderer);
			}
			if(e.type == SDL_MOUSEWHEEL) {
				// do something
			}
		}
		//draw(postion, tuning, notes, renderer);
	}

	

	SDL_CloseAudioDevice(dev);

}

void audio_callback(void *userdata, Uint8 *stream, int len){
	float *out = (float*) stream;
	printf("wewe");

	for(int i=0; i<len/sizeof(float); i++){
		*((double*) userdata) += 440. /44100.;
		out[i] = triangleFunction( *((double*) userdata));
	}
}

