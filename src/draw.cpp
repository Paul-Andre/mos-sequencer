#include "draw.h"
#include "Tuning.h"
#include "PianoRollPosition.h"
#include "Note.h"
#include "ScalePitch.h"
#include <vector>
#include <math.h>
#include <SDL2/SDL.h>
//#include <SDL2/SDL2_gfxPrimitives.h>


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

	double beatStart = floor(position.x*2);
	double beatEnd = ceil(position.x+position.w*2);

	vector<SDL_Rect> beatZones;
	for(int i = beatStart; i<beatEnd; i+=2){
			SDL_Rect r;

			double x = (i*0.5- position.x)*(double)screenWidth/position.w ;
			double w = (0.5)*(double)screenWidth/position.w;
			double y = 0;
			double h = screenHeight;
			r.x =x; r.y=0; r.w = w; r.h =h; 
			beatZones.push_back(r);
	}


	const int lowerOctave = floor((position.y-position.h)/tuning.gen1Size);
	const int higherOctave = ceil((position.y)/tuning.gen1Size);

	vector<SDL_Rect> octaveLines;
	vector<SDL_Rect> scaleLines;
	vector<SDL_Rect> squares;
	vector<SDL_Rect> connection;




	for(int i=lowerOctave; i<higherOctave; i++) {
		double octavePitch = i*tuning.gen1Size;
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
		r.x = x; r.y = y-1.9; r.w = w; r.h = h;
		onScreenNotes.push_back(r);
		ScalePitch noAccidentals = note.scalePitch;
		noAccidentals.accidentals =0;
		pitch = getPitch(noAccidentals, tuning);
		y = (position.y-pitch)*(double)screenHeight/position.h;

		SDL_Rect q;
		q.x = x-0.5; q.y = y + 1.6; q.w = 5; q.h = 5;
		squares.push_back(q);

		SDL_Rect t;
		if (fabs(q.y -r.y) >4){
			if (q,y > r.y) {
				t.x = x+1; t.y = y-(q.y-r.y)+2 ; t.w = 1; t.h = q.y-r.y;
				}
			else{
				t.x = x+1; t.y = y + 2; t.w = 1; t.h = r.y-q.y;
			}
		}
		connection.push_back(t);


	}





	if (beatZones.size()>0) {
		SDL_SetRenderDrawColor(renderer, 0, 23, 32, 0);
		if(SDL_RenderFillRects(renderer, &beatZones[0], beatZones.size()) != 0)
			printf("Error in draw when rendering squares\n");

	}



	if (squares.size()>0) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		if(SDL_RenderFillRects(renderer, &squares[0], squares.size()) != 0)
			printf("Error in draw when rendering squares\n");

	}

	if (connection.size()>0) {
		SDL_SetRenderDrawColor(renderer, 255, 255 , 0, 54);
		if(SDL_RenderFillRects(renderer, &connection[0], connection.size()) != 0)
			printf("Error in draw when rendering connection\n");

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

	//vector<ScalePitch> pitches = pitchesInWindow(tuning, 


	SDL_RenderPresent(renderer);
	return;
}


