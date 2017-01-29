#include "draw.h"
#include "Tuning.h"
#include "PianoRollPosition.h"
#include "Note.h"
#include "ScalePitch.h"
#include <vector>
#include <math.h>
#include <SDL2/SDL.h>

void draw(PianoRollPosition const &position,
		Tuning const &tuning,
		vector<Note> const &notes,
		SDL_Renderer *renderer)
{
	int screenWidth;
	int screenHeight;
	
	SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
//	SDL_RenderPresent(renderer);
	

//	printf("[DEBUG] position.y is: %f\n", position.y);
    double offset = (position.y - (int) position.y)*screenHeight/position.h; 
	int sc_size = tuning.scale.size();
	const int octaveLinesNum = 2 + (int) position.h;
	const int scaleLinesNum = octaveLinesNum*sc_size; //Might want to change this
	double octaveSize = screenHeight/position.h;        //Careful!

	double beatStart = floor(position.x*4);
	double beatEnd = ceil(position.x+position.w*4);

	vector<SDL_Rect> beatZones;
	//for(int i = beatStart; i<beatEnd; i+=2){

		//beatZones.push_back({






	SDL_Rect scaleLines[scaleLinesNum];
	SDL_Rect octaveLines[octaveLinesNum];

	for(int i=0; i<2+(int) position.h; i++) {
		double octaveHeight = i*octaveSize + offset;
		octaveLines[i].x = 0;
		octaveLines[i].y = screenHeight - (int)(octaveHeight);
		octaveLines[i].w = screenWidth;
		octaveLines[i].h = 2;

		for(int j=0; j<sc_size; j++) {
			double height = octaveHeight + tuning.scale[j]*octaveSize;
			scaleLines[i*sc_size+j].x = 0;
			scaleLines[i*sc_size+j].y = screenHeight - (int)(height);
			scaleLines[i*sc_size+j].w = screenWidth;
			scaleLines[i*sc_size+j].h = 1;
		}
	}


	vector<SDL_Rect> onScreenNotes;
	for (int i=0; i<notes.size(); i++) {
		Note const &note = notes[i];
		double pitch = getPitch(note.scalePitch, tuning);
		SDL_Rect r;
		double x = (note.start - position.x)*(double)screenWidth/position.w;
		double w = (note.duration)*(double)screenWidth/position.w;
		double y = (position.y-pitch)*(double)screenHeight/position.h;
		double h = 0.01*screenHeight/position.h;
		r.x = x; r.y = y; r.w = w; r.h = h;
		onScreenNotes.push_back(r);
	}


	if (octaveLinesNum*sc_size>0) {
		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
		if(SDL_RenderFillRects(renderer, scaleLines, octaveLinesNum*sc_size) != 0)
			printf("Error in draw when rendering scaleLines\n");
	}

	if (octaveLinesNum>0) {
		SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
		if(SDL_RenderFillRects(renderer, octaveLines, octaveLinesNum) != 0)
			printf("Error in draw when rendering octaveLines\n");
	}

	if (onScreenNotes.size()>0) {
		SDL_SetRenderDrawColor(renderer, 210, 10, 10, 255);
		if(SDL_RenderFillRects(renderer, &onScreenNotes[0], onScreenNotes.size()) != 0)
			printf("Error in draw when rendering onScreenNotes\n");
	}


	SDL_RenderPresent(renderer);
	return;
}


