#ifndef VIEW_NCURSES_H
#define VIEW_NCURSES_H
#include "model.h"

void init_ncurses_view();
void draw_ncurses_view(GameState *game);
void close_ncurses_view();
int get_ncurses_input();

#endif