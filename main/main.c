#include <SDL3/SDL.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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
    srand(time(NULL));
    game.isNC = 0;
    game.isSDL = 1; 
    init_model(&game, GRID_WIDTH, GRID_HEIGHT, score_init);
    
    
    if (!init_sdl_view(&game)) return EXIT_FAILURE;
    
    int running = 1;

    while (running) {
        
        handle_event(&game, &running); 
        if (game.currView == JEU && !game.game_over) {
            if (!game.pause) { // ca sert a faire en sorte que le jeu s'arrete pas mais soit en pause
                update_bullets(&game);      
                update_enemies(&game);      
                enemy_shoot(&game); // je l'ai enleve car je voulais teste si les ennemis detruisaient les boucliers        
                check_collisions(&game);
            }   
        }
        
        draw_sdl_view(&game);
        
        SDL_Delay(16); // au plus c'esdt bas plus ca trace
    }
    close_sdl_view();
    } else if (a == 'n' || a == 'N') {
        game.isSDL = 0;
        game.isNC = 1;
    }

    return EXIT_SUCCESS;
}
