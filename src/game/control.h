#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H 1

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "event.h"

using namespace std;

/* Control functions */

// Main
void game_mainLoop();

// Rendering
void game_startRenderThread();
void game_joinRenderThread();

// SDL
int game_initializeSDL(string windowTitle);
void game_destroySDL();

#endif
