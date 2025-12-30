#include <SDL3/SDL.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/model.h"
#include "../include/view_sdl.h"

// On définit ici la taille de la grille virtuelle
#define GRID_WIDTH 80
#define GRID_HEIGHT 40

int main() {
    srand(time(NULL));
    GameState game;
    
    init_model(&game, GRID_WIDTH, GRID_HEIGHT);
    
    if (!init_sdl_view(&game)) return 1;
    
    int running = 1;
    SDL_Event event;

    while (running) {
        // --- INPUT ---
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = 0;
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) running = 0;
                if (event.key.key == SDLK_LEFT) player_move(&game, -1);
                if (event.key.key == SDLK_RIGHT) player_move(&game, 1);
                if (event.key.key == SDLK_SPACE) player_shoot(&game);
            }
        }

        // --- UPDATE ---
        if (!game.game_over) {
            update_bullets(&game);
            update_enemies(&game);
            enemy_shoot(&game);
            check_collisions(&game);
        }

        // --- DRAW ---
        draw_sdl_view(&game);
        SDL_Delay(50); // ~20 FPS
    }

    close_sdl_view();
    return 0;
}