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
int closestNote(vector<Note> const &notes, Tuning const &tuning, double x, double y);

static Uint8 *audio_pos;
static Uint32 audio_len;
double phase;

int closestNote(vector<Note> const &notes, Tuning const &tuning, double x, double y) {
	printf("size: %d\n", notes.size());
	for(int i=0; i<notes.size(); i++){
		if((x - notes[i].start) < notes[i].duration && (x - notes[i].start) > 0) {
			if(fabs(getPitch(notes[i].scalePitch, tuning) - y) <0.01){
				return i;
			}
		}
	}

	return -1;
}


int main(int argc, char **argv) {
	int k=0;
	PianoRollPosition position;
	Tuning tuning = generateMosScale( 1., 7./12., 7);
	vector<Note> notes;
	notes.push_back({0,0.5, (ScalePitch) {1,-1}});
	notes.push_back({0.5,0.5, (ScalePitch) {2,-1}});
	notes.push_back({1,0.5, (ScalePitch) {1,-1}});
	notes.push_back({2.5,0.5, (ScalePitch) {2,-1}});
	notes.push_back({2,0.5, (ScalePitch) {3,0}});
	notes.push_back({4.5,0.5, (ScalePitch) {3,1}});

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
	userdata.events = events;
	userdata.state = On;

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
	}
	else {
		// Unpausing audio
		SDL_PauseAudioDevice(dev, 0);
	}


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




				if (e.key.keysym.scancode == SDL_SCANCODE_RETURN) {

					int beat = 44100;
					vector<NoteEvent> events = makeEventStream(notes, beat, tuning);
					userdata.state = On;

					SDL_LockAudioDevice(dev);
					resetPlaybackStructure(userdata);
					userdata.events = events;

					SDL_UnlockAudioDevice(dev);
				}



				else if( currentKeyStates[ SDL_SCANCODE_UP ] )
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


			if(e.type == SDL_MOUSEWHEEL) {
				SDL_GetMouseState(&mouse_position_x,&mouse_position_y);	


				int screenWidth;
				int screenHeight;

				SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);


				double zoom_factor = 1.050;
				double dX;
				double dY;
				double zoom_power;
				if (e.wheel.y==1) {
					zoom_power = zoom_factor;
				}
				else if (e.wheel.y== -1) {
					zoom_power = 1/zoom_factor;
				}
				else {
					zoom_power = 1;
				}

				if  ( currentKeyStates[SDL_SCANCODE_LCTRL] ) {
					dX = zoom_power;
					dY = 1;
				}
				else {
					dX = 1;
					dY = zoom_power;
				}

				double pX = ((double)mouse_position_x / screenWidth) * position.w + position.x;
				double pY = position.y - ((double)mouse_position_y / screenHeight) * position.h;
				double d_left = (pX - position.x) * dX;
				double d_up = (position.y - pY) * dY;
				printf("pY: %f d_up: %f \n", pY, d_up );
				position.w *= dX;
				position.x = pX - d_left;
				position.h *= dY;
				position.y = pY + d_up;
			}

			if(e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_position_x,&mouse_position_y);	
				ScalePitch closest_pitch;
				int screenWidth;
				int screenHeight;

				SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);

				if(e.button.button == SDL_BUTTON_LEFT) {
					double mouse_pitch = position.y - (mouse_position_y / (double)screenHeight) * position.h;

					vector<ScalePitch> pitches = pitchesInWindow(tuning, position.y, position.h);
					double rawPitch, diff = (double)screenHeight;
					ScalePitch best;

					for(int i=0; i<pitches.size(); i++){
						//					printf("Pitch: %d, %d\n", pitches[i].scaleDegree, pitches[i].accidentals);
						rawPitch = getPitch(pitches[i], tuning);
						//					printf("Raw pitch: %f\n", rawPitch);

						if(fabs(mouse_pitch - rawPitch) < diff) {
							best = pitches[i];
							diff = fabs(mouse_pitch - rawPitch);
						}
					}

					printf("mouse is at %f\n", mouse_pitch);
					//	printf("Closest is %f\n", best);
					double start = position.x + (mouse_position_x / (double)screenWidth) * position.w;

					notes.push_back({start,0.5, best });
				}
				if(e.button.button == SDL_BUTTON_RIGHT) {
					
					double x = position.x + (mouse_position_x / (double)screenWidth) * position.w;
					double y = position.y - (mouse_position_y / (double)screenHeight) * position.h;
					printf("Coordinates: %f, %f\n", x, y);
					
					int c = closestNote(notes, tuning, x, y);

					printf("c: %d\n", c);
					printf("closest is {%f, %f, {%d, %d}}\n", notes[c].start, notes[c].duration, notes[c].scalePitch.scaleDegree, notes[c].scalePitch.accidentals);
				}

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

