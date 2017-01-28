#ifdef DRAW_H
#define DRAW_H

#include "Tuning.h"
#include "PianoRollPosition.h"
#include "Note.h"
#include <vector>
#include <SDL2/SDL.h>

void draw(PianoRollPosition const &position, Tuning const &tuning, vector<Note> notes, SDL_Renderer *renderer);

#endif
