#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include "../include/view_sdl.h"

static SDL_Window *window = NULL;
static SDL_Renderer *rend = NULL; 

int init_sdl_view(GameState *game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 0; // Utilise 0 pour échec

    game->currView = ACCUEIL;

    SDL_DisplayID dis = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(dis);

    if(!mode) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erreur mode: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    int width = mode->w, height = mode->h;

    // Création fenêtre
    window = SDL_CreateWindow("Jeu Space Invader !", width, height, SDL_WINDOW_FULLSCREEN);

    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erreur window : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;    
    }

    // Création Rendu
    rend = SDL_CreateRenderer(window, NULL); // NULL = Driver par défaut

    if(!rend) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erreur rend : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;    
    }

    // Mise à l'échelle logique (Pixel Art / Grille)
    SDL_SetRenderLogicalPresentation(rend, game->width, game->height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return 1; // 1 = Succès
}

void draw_menu_view() {
    // 1. Fond bleu foncé
    SDL_SetRenderDrawColor(rend, 0, 0, 50, 255);
    SDL_RenderClear(rend);

    // 2. Bouton Vert
    SDL_SetRenderDrawColor(rend, 0, 200, 0, 255);
    SDL_FRect btn_rect = {(float)BTN_X, (float)BTN_Y, (float)BTN_W, (float)BTN_H};
    SDL_RenderFillRect(rend, &btn_rect);

    // 3. Triangle "Play" Blanc
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderLine(rend, BTN_X + 8, BTN_Y + 1, BTN_X + 8, BTN_Y + 3);
    SDL_RenderLine(rend, BTN_X + 8, BTN_Y + 1, BTN_X + 12, BTN_Y + 2);
    SDL_RenderLine(rend, BTN_X + 8, BTN_Y + 3, BTN_X + 12, BTN_Y + 2);
}

void draw_game_view(GameState *game) {
    // 1. Fond Noir
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    // 2. Joueur (Vert)
    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
    SDL_FRect player_rect = {(float)game->player.x, (float)game->player.y, 1.0f, 1.0f};
    SDL_RenderFillRect(rend, &player_rect);

    // 3. Ennemis (Rouge)
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (game->enemies[i].alive) {
            SDL_FRect enemy_rect = {(float)game->enemies[i].x, (float)game->enemies[i].y, 1.0f, 1.0f};
            SDL_RenderFillRect(rend, &enemy_rect);
        }
    }

    SDL_SetRenderDrawColor(rend, 255, 255, 0, 255);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) {
            SDL_FRect b_rect = {(float)game->bullets[i].x + 0.4f, (float)game->bullets[i].y, 0.2f, 1.0f};
            SDL_RenderFillRect(rend, &b_rect);
        }
    }
    
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (game->enemy_bullets[i].active) {
            SDL_FRect eb_rect = {(float)game->enemy_bullets[i].x + 0.4f, (float)game->enemy_bullets[i].y, 0.2f, 1.0f};
            SDL_RenderFillRect(rend, &eb_rect);
        }
    }
}

void draw_sdl_view(GameState *game) {
    
    // On choisit quoi dessiner dans le buffer
    if (game->currView == ACCUEIL) {
        draw_menu_view();
    } else {
        draw_game_view(game);
    }
    
    SDL_RenderPresent(rend);
}

void close_sdl_view() {
    if (rend) SDL_DestroyRenderer(rend); 
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}