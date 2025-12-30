#ifndef VIEW_SDL_H
#define VIEW_SDL_H
#include "model.h"

#define BTN_X 40
#define BTN_Y 18
#define BTN_H 4
#define BTN_W 20


int init_sdl_view(GameState *game);
void draw_sdl_view(GameState *game);
void close_sdl_view();

#endif