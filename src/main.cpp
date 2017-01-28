#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Tuning.h"
#include "Note.h"
#include "PianoRollPosition.h"
#include <math.h>
using namespace std;

#include <SDL2/SDL.h>
#define MUS_PATH ""

void audio_callback(void *userdata, Uint8 *stream, int len);

static Uint8 *audio_pos;
static Uint32 audio_len;

int main(int argc, char **argv) {
	PianoRollPosition postion;
	Tuning tuning = {
		1.,
		log2(7./12.),
		7,
	};
	vector<double> scale = generateMosScale(tuning);
	vector<Note> notes;


	bool quit = false;
	SDL_Event e;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Sequencer",
			             SDL_WINDOWPOS_UNDEFINED,
						 SDL_WINDOWPOS_UNDEFINED,
						 640, 480,
						 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	static Uint32 wav_length;
	static Uint8 *wav_buffer;
	static SDL_AudioSpec wav_spec;

	if( SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL )
		return 1;

	wav_spec.callback = audio_callback;
	wav_spec.userdata = NULL;

	audio_pos = wav_buffer;
	audio_len = wav_length;

	if( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	SDL_PauseAudio(0);

	while( audio_len > 0 )
		SDL_Delay(100);

	SDL_CloseAudio();
	// SDL_FreeWav(wav_buffer);

	//drawScale(tuning, renderer);


	printf("Hello world.\n");
}

void draw(SDL_Renderer* renderer) {
	int screenWidth;
	int screenHeight;
	
	SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
	//SDL_SetRendererDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);
	
}

void audio_callback(void *userdata, Uint8 *stream, int len){

}

void event_loop(){

}
