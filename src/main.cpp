#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

int main(int argc, char **argv) {
	bool quit = false;
	SDL_Event e;
	

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Sequencer",
			             SDL_WINDOWPOS_UNDEFINED,
						 SDL_WINDOWPOS_UNDEFINED,
						 640, 480,
						 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

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

