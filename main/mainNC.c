#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/model.h"
#include "../include/controller.h"
#include "../include/view_ncurses.h"
#include "../include/view_sdl.h"

/**
 * @file mainNC.c
 * @brief Point d'entrée pour la version ncurses du jeu.
 *
 * Initialise ncurses, le modèle et lance la boucle principale de rendu
 * et de traitement des entrées terminal.
 */

/**
 * @brief Fonction principale pour la version ncurses.
 *
 * @param argc Nombre d'arguments (non utilisé).
 * @param argv Tableau d'arguments (non utilisé).
 * @return EXIT_SUCCESS en sortie normale.
 */
int main(int argc, char const *argv[])
{
    GameState game;

    game.isSDL = false;
    game.isNC = true;
    init_ncurses();
    
    init_model(&game, COLS, LINES, score_init);

    int running = 1;
    while (running) {
        /* Affichage */
        render_ncurses(&game);
        
        /* Entrées */
        running = handle_input_ncurses(&game);
        
        /* Logique du jeu (si en jeu) */
        if (game.currView == JEU) {
            update_bullets(&game);
            update_enemies(&game);
            enemy_shoot(&game);
            check_collisions(&game);
        }
        
        napms(16); /* ~60 FPS */
    }
    
    cleanup_ncurses();
    return EXIT_SUCCESS;
}

