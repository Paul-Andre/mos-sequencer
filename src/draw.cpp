#include "draw.h"
#include "Tuning.h"
#include "PianoRollPosition.h"
#include "Note.h"
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

//	printf("[DEBUG] offset is: %f\n", offset);

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

	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
	if(SDL_RenderFillRects(renderer, scaleLines, octaveLinesNum*sc_size) != 0)
		printf("Error in draw when rendering scaleLines\n");
	SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
	if(SDL_RenderFillRects(renderer, octaveLines, octaveLinesNum) != 0)
		printf("Error in draw when rendering octaveLines\n");

	SDL_RenderPresent(renderer);
	return;
}


