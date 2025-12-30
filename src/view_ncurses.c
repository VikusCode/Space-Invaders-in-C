#include <ncurses.h>
#include "../include/view_ncurses.h"

void init_ncurses_view() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Ne bloque pas le programme
    curs_set(0);
}

void close_ncurses_view() {
    endwin();
}

int get_ncurses_input() {
    return getch();
}

void draw_ncurses_view(GameState *game) {
    clear();
    
    // UI
    mvprintw(0, 1, "Score: %d", game->score);
    if(game->game_over) mvprintw(game->height/2, game->width/2 - 5, "GAME OVER");

    // Joueur
    mvaddch(game->player.y, game->player.x, 'A');

    // Ennemis
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (game->enemies[i].alive) {
            mvaddch(game->enemies[i].y, game->enemies[i].x, 'W');
        }
    }

    // Balles
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) mvaddch(game->bullets[i].y, game->bullets[i].x, '|');
    }
    
    // Balles ennemis
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (game->enemy_bullets[i].active) mvaddch(game->enemy_bullets[i].y, game->enemy_bullets[i].x, '!');
    }

    refresh();
}