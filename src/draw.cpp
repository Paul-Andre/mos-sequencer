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

	double beatStart = floor(position.x*4);
	double beatEnd = ceil(position.x+position.w*4);

	vector<SDL_Rect> beatZones;
	//for(int i = beatStart; i<beatEnd; i+=2){
		//beatZones.push_back({


	const int lowerOctave = floor((position.y-position.h)/tuning.gen1Size);
	const int higherOctave = ceil((position.y)/tuning.gen1Size);

	vector<SDL_Rect> octaveLines;
	vector<SDL_Rect> scaleLines;

	for(int i=lowerOctave; i<higherOctave; i++) {
		double octavePitch = i*tuning.gen1Size;
		printf("%f\n", octavePitch);
		SDL_Rect r;
		r.x = 0;
		r.y = (position.y - octavePitch)*screenHeight/position.h;
		r.w = screenWidth;
		r.h = 2;
		octaveLines.push_back(r);

		for(int j=0; j<tuning.scale.size()-1; j++) {
			SDL_Rect r;
			r.x = 0;
			r.y = (position.y - (octavePitch + tuning.scale[j]))*screenHeight/position.h;
			r.w = screenWidth;
			r.h = 1;
			scaleLines.push_back(r);
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


	if (scaleLines.size()>0) {
		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
		if(SDL_RenderFillRects(renderer, &scaleLines[0], scaleLines.size()) != 0)
			printf("Error in draw when rendering scaleLines\n");
	}

	if (octaveLines.size()>0) {
		SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
		if(SDL_RenderFillRects(renderer, &octaveLines[0], octaveLines.size()) != 0)
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


