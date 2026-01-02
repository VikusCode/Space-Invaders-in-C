#include <SDL3/SDL.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "../include/model.h"
#include "../include/controller.h"
#include "../include/view_sdl.h"
#include "../include/view_ncurses.h"


#define GRID_WIDTH 80
#define GRID_HEIGHT 40

int main()
{
    char a;
    printf("Version SDL ou NC ? s pour SDL, n pour NCurse \n");
    scanf("%c", &a);

    GameState game;

    if (a == 's' || a == 'S') {

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Erreur Init SDL: %s", SDL_GetError());
        return 1;
    }
    srand(time(NULL));
    game.isNC = 0;
    game.isSDL = 1; 
    init_model(&game, GRID_WIDTH, GRID_HEIGHT, score_init);
    
    
    if (!init_sdl_view(&game)) return EXIT_FAILURE;

    if (!init_audio(&game)) printf("Attention: Pas de son (fichiers manquants ?)\n"); //nouveau test ajoute

    
    int running = 1;

    while (running) {
        handle_event(&game, &running); 

        if (game.currView == JEU && !game.game_over) {
            update_bullets(&game);      
            update_enemies(&game);      
            enemy_shoot(&game); // je l'ai enleve car je voulais teste si les ennemis detruisaient les boucliers        
            check_collisions(&game);
        }
        
        draw_sdl_view(&game);
        
        SDL_Delay(16); // au plus c'est bas plus ca trace
    }

    cleanup_audio(&game); //nouvelle fonction ajoutee
    close_sdl_view();

    } else if (a == 'n' || a == 'N') {
        game.isSDL = 0;
        game.isNC = 1;
        init_ncurses();
    
        GameState game;
        init_model(&game, COLS, LINES, 0);
        game.isSDL = 0; // Important

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
        return 0;
    }

    return EXIT_SUCCESS;
}
