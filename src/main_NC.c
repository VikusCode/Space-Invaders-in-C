#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Pour usleep
#include <time.h>
#include <ncurses.h> // Pour les constantes de touches (KEY_LEFT, etc.)

#include "../include/model.h"
#include "../include/view_ncurses.h"

// Note: On n'inclut PAS controller.h car il est spécifique à SDL (SDL_Event)
// On gère les inputs directement ici pour Ncurses.

int main() {
    srand(time(NULL));
    
    // 1. D'abord Ncurses (pour récupérer la taille du terminal COLS/LINES)
    init_ncurses_view();
    
    GameState game;
    // On utilise COLS et LINES fournis par ncurses
    init_model(&game, COLS, LINES);
    game.isSDL = 0; // Important : indique au modèle qu'on n'est pas en SDL

    int running = 1;
    int ch; // Variable pour stocker la touche pressée

    while (running) {
        // --- A. GESTION DES ENTRÉES (CONTROLLER NCURSES) ---
        ch = get_ncurses_input();

        if (ch != ERR) { // Si une touche est pressée
            
            // --- ECRAN ACCUEIL ---
            if (game.currView == ACCUEIL) {
                if (ch == '\n' || ch == KEY_ENTER) { // Touche Entrée
                    game.currView = JEU;
                }
                else if (ch == 27) { // Touche Echap (27 en ASCII)
                    running = 0;
                }
            }
            
            // --- ECRAN DE JEU ---
            else if (game.currView == JEU) {
                switch(ch) {
                    case 27: // Echap -> Menu Pause
                        game.currView = MENU_JEU;
                        break;
                    
                    case KEY_LEFT:
                    case 'q':
                    case 'Q':
                        player_move(&game, -1);
                        break;
                    
                    case KEY_RIGHT:
                    case 'd':
                    case 'D':
                        player_move(&game, 1);
                        break;
                    
                    case ' ': // Espace
                        player_shoot(&game);
                        break;
                }
            }
            
            // --- MENU PAUSE (MENU_JEU) ---
            else if (game.currView == MENU_JEU) {
                if (ch == '\n' || ch == KEY_ENTER) { // Entrée -> Recommencer
                    init_model(&game, COLS, LINES);
                    game.currView = JEU;
                    game.isSDL = 0;
                }
                else if (ch == 27) { // Echap -> Reprendre
                    game.currView = JEU;
                }
                else if (ch == 'a' || ch == 'q') { // A ou Q -> Quitter
                    running = 0;
                }
            }
        }

        // --- B. MISE A JOUR DU MODELE ---
        // On ne met à jour que si le jeu est actif et pas fini
        if (game.currView == JEU && !game.game_over) {
            update_bullets(&game);
            update_enemies(&game);
            enemy_shoot(&game);
            check_collisions(&game);
            
            // Si tu as implémenté le cooldown de tir :
            // update_cooldowns(&game); 
        }

        // --- C. AFFICHAGE ---
        draw_ncurses_view(&game);

        // --- D. TIMING ---
        // 16000 microsecondes = 16ms ~= 60 FPS
        usleep(16000);
    }

    close_ncurses_view();
    return 0;
}