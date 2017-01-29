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


	//relative mouse positions
	int window_size_x ;
	int window_size_y ;
	SDL_GetRendererOutputSize(renderer, &window_size_x, &window_size_y);
	int mouse_position_x;
	int mouse_position_y;
	SDL_GetMouseState(&mouse_position_x,&mouse_position_y);	





	// Event loop
	while(!quit) {
		if(SDL_WaitEventTimeout(&e, 50)){
			if(e.type == SDL_QUIT)
				quit = true;
            const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

			if(e.type == SDL_KEYDOWN  && !currentKeyStates[SDL_SCANCODE_LCTRL]) {




				if( currentKeyStates[ SDL_SCANCODE_UP ] )
                {
                	position.y -= 0.1;
                }
                else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
                {
                	position.y += 0.1;
                }
                else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
                {
                	position.x += 0.1;
                }
                else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
                {
                	position.x -= 0.1;
                }

                


			}
			SDL_GetMouseState(&mouse_position_x,&mouse_position_y);	



			double zoom_factor = 1.0000001;
			double dX = window_size_x * (1 - 1 / zoom_factor);
			double dY = window_size_y *(1 - 1/zoom_factor);
			double pX = (mouse_position_x - position.x) / position.w;
			double pY = (mouse_position_y - position.y) / position.h;
			double d_left = pX * dX;
			double d_up = pY * dY;


			if(e.type == SDL_MOUSEWHEEL && currentKeyStates[SDL_SCANCODE_LCTRL]) {
				printf("%d\n x1x1x1x1x1", dX );
				printf("%d\n x2x2x2x2x2", d_left );
				position.w += dX*e.wheel.y;
				position.x += d_left*e.wheel.y;
			}
			else if (e.type == SDL_MOUSEWHEEL && !currentKeyStates[SDL_SCANCODE_LCTRL]){
				printf("%d\n y1y1y1y1y1", dY );
				printf("%d\n y2y2y2y2y2", d_up );
				position.h += d_up*e.wheel.y;
				position.y += dY*e.wheel.y;

			
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

