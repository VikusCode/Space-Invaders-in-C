#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/model.h"
#include "../include/controller.h"
#include "../include/view_ncurses.h"

int main(int argc, char const *argv[])
{
    GameState game;

    game.isSDL = 0;
    game.isNC = 1;
    init_ncurses();
    
    init_model(&game, COLS, LINES, 0);

    int running = 1;
    while (running) {
        // Affichage
        render_ncurses(&game);
        
        // Entrées (Appel de ta nouvelle fonction dans controller.c)
        running = handle_input_ncurses(&game);
        
        // Logique du jeu (seulement si on joue et pas en pause)
        if (game.currView == JEU) {
            update_bullets(&game);
            update_enemies(&game);
            enemy_shoot(&game);
            check_collisions(&game);
        }
        
        napms(16); // ~60 FPS
    }
    
    cleanup_ncurses();
    return EXIT_SUCCESS;
}

