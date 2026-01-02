#ifndef VIEW_SDL_H
#define VIEW_SDL_H
#include "model.h"

#define BTN_X 40
#define BTN_Y 18
#define BTN_H 4
#define BTN_W 20


int init_sdl_view(GameState *game);
void draw_sdl_view(GameState *game);
void draw_game_view(GameState *game);
float draw_text(const char* text, float x, float y, float size);
void close_sdl_view();
void draw_number(int number, float x, float y, float size);
int init_audio(GameState *game); //nouveau
void cleanup_audio(GameState *game); //nouveau
void draw_pause_menu(GameState *game);
void convert_mouse_coordinates(float *x, float *y);
void draw_menu_view(GameState *game);
#endif