#include <SDL3/SDL.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/model.h"
#include "../include/controller.h"
#include "../include/view_sdl.h"

// le main la est un test juste du sdl il faudra le concateneer avec le NC

// Définition de la grille logique (doit être cohérent avec view_sdl.c et model.c)
#define GRID_WIDTH 80
#define GRID_HEIGHT 40

int main(int argc, char *argv[]) {
    srand(time(NULL));
    GameState game;
    
    init_model(&game, GRID_WIDTH, GRID_HEIGHT);
    game.isSDL = 1; 
    
    if (!init_sdl_view(&game)) return 1;
    
    int running = 1;

    while (running) {
        
        handle_event(&game, &running); 
        
        if (game.currView == JEU && !game.game_over) {
            if (!game.pause) { // ca sert a faire en sorte que le jeu s'arrete pas mais soit en pause
                update_bullets(&game);      
                update_enemies(&game);      
                enemy_shoot(&game);         
                check_collisions(&game);
            }   
        }
        
        draw_sdl_view(&game);
        
        SDL_Delay(16);
    }

    close_sdl_view();
    return 0;
}