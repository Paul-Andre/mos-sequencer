#include "draw.h"
#include "Tuning.h"
#include "PianoRollPosition.h"
#include "Note.h"
#include <vector>
#include <SDL2/SDL.h>

#define OFFSET 30

void draw(PianoRollPosition const &position,
		Tuning const &tuning,
		vector<Note> notes,
		SDL_Renderer *renderer)
{
	int screenWidth;
	int screenHeight;
	
	SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
	//SDL_SetRendererDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);


	int octaveLinesNum = (int) position.h;
	int scaleLinesNum = octaveLinesNum*tuning.scale.size(); //Might want to change this
	double octaveSize = screenHeight/octaveLinesNum;        //Careful!
	
	SDL_Rect scaleLines[scaleLinesNum];
	SDL_Rect octaveLines[octaveLinesNum];

	for(int i=0; i<octavesLinesNum; i++) {
		double octaveHeight = i*octaveSize + OFFSET - octaveSize;
		octaveLines[i].x = 0;
		octaveLines[i].y = screenHeight - (int)(octaveHeight);
		octaveLines[i].w = screenWidth;
		octaveLines[i].h = 1;

		for(int j=0; j<scaleLinesNum; j++) {
			double height = octaveHeight + tuning.scale[j]*octaveSize;
			scaleLines[i*scaleLinesNum+j].x = 0;
			scaleLines[i*scaleLinesNum+j].y = screenHeight - (int)(height);
			scaleLines[i*scaleLinesNum+j].w = screenWidth;
			scaleLines[i*scaleLinesNum+j].h = 1;
		}
	}

	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
	SDL_RenderFillRects(renderer, scaleLines, scaleLinesNum);
	SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
	SDL_RenderFillRects(renderer, octaveLines, octaveLinesNum);




	SDL_RenderPresent(renderer);
}


