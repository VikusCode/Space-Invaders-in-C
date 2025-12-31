#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include "../include/view_sdl.h"

static SDL_Window *window = NULL;
static SDL_Renderer *rend = NULL; 

void convert_mouse_coordinates(float *x, float *y) {
    if (rend) {
        // Cette fonction magique de SDL3 fait la conversion Pixels -> Grille Logique
        SDL_RenderCoordinatesFromWindow(rend, *x, *y, x, y);
    }
}

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

    rend = SDL_CreateRenderer(window, NULL); // NULL = Driver par défaut

    if(!rend) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erreur rend : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;    
    }

    SDL_SetRenderLogicalPresentation(rend, game->width, game->height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return 1; 
}

void draw_menu_view(GameState *game) {
    // 1. Fond bleu nuit
    SDL_SetRenderDrawColor(rend, 10, 10, 40, 255);
    SDL_RenderClear(rend);

    // --- DIMENSIONS ---
    float btn_w = 24.0f;
    float btn_h = 6.0f;  // Un peu moins haut pour en mettre deux
    float gap = 2.0f;    // Espace entre les boutons
    
    // Calcul pour centrer le BLOC des deux boutons
    float total_h = (btn_h * 2) + gap;
    float start_y = (game->height - total_h) / 2;
    float btn_x = (game->width - btn_w) / 2;

    // Position Y des boutons
    float play_y = start_y;
    float quit_y = start_y + btn_h + gap;

    // --- BOUTON 1 : JOUER (Vert) ---
    SDL_SetRenderDrawColor(rend, 0, 200, 0, 255);
    SDL_FRect play_rect = {btn_x, play_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &play_rect);
    
    // Texte "JOUER"
    float text_size = 0.4f;
    // Centrage approximatif du texte
    draw_text("JOUER", btn_x + 8.0f, play_y + 1.5f, text_size);


    // --- BOUTON 2 : QUITTER (Rouge) ---
    SDL_SetRenderDrawColor(rend, 200, 0, 0, 255);
    SDL_FRect quit_rect = {btn_x, quit_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &quit_rect);

    // Texte "QUITTER"
    // "QUITTER" fait 7 lettres. 7 * 4 * 0.4 = 11.2 de large. 
    // (24 - 11.2) / 2 = 6.4 de marge à gauche.
    draw_text("QUITTER", btn_x + 6.0f, quit_y + 1.5f, text_size);

    // --- TITRE ---
    draw_text("SPACE INVADERS", (game->width - (14 * 4 * 0.5)) / 2, 5.0f, 0.5f);
}

void draw_pause_menu(GameState *game) {
    // 1. Fond gris foncé (pour montrer qu'on est en pause)
    SDL_SetRenderDrawColor(rend, 40, 40, 40, 255);
    SDL_RenderClear(rend);

    // --- DIMENSIONS ---
    float btn_w = 28.0f; // Un peu plus large pour "RECOMMENCER"
    float btn_h = 6.0f;
    float gap = 2.0f;
    
    // Calcul du centrage
    float total_h = (btn_h * 2) + gap;
    float start_y = (game->height - total_h) / 2;
    float btn_x = (game->width - btn_w) / 2;

    float restart_y = start_y;
    float quit_y = start_y + btn_h + gap;
    float text_size = 0.4f;

    // --- TITRE "PAUSE" ---
    draw_text("PAUSE", (game->width - (5 * 4 * 0.5)) / 2, 5.0f, 0.5f);

    // --- BOUTON 1 : RECOMMENCER (Cyan) ---
    SDL_SetRenderDrawColor(rend, 0, 200, 200, 255);
    SDL_FRect restart_rect = {btn_x, restart_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &restart_rect);
    
    // Texte "RECOMMENCER" (11 lettres)
    // Centrage approx : 11 * 4 * 0.4 = 17.6 largeur
    // Marge X = (28 - 17.6) / 2 = 5.2
    draw_text("RECOMMENCER", btn_x + 5.2f, restart_y + 1.5f, text_size);

    // --- BOUTON 2 : QUITTER (Rouge) ---
    SDL_SetRenderDrawColor(rend, 200, 0, 0, 255);
    SDL_FRect quit_rect = {btn_x, quit_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &quit_rect);

    // Texte "QUITTER"
    draw_text("QUITTER", btn_x + 8.0f, quit_y + 1.5f, text_size);
}

void draw_alpha(char c, float x, float y, float size) {
    // On convertit tout en majuscule pour simplifier
    if (c >= 'a' && c <= 'z') c -= 32; 
    
    // Si ce n'est pas une lettre A-Z, on ne dessine rien (ou un espace)
    if (c < 'A' || c > 'Z') return;

    int index = c - 'A'; // 0 pour A, 1 pour B, etc.

    // Définition des pixels pour chaque lettre (0=vide, 1=plein)
    // C'est un peu long, mais c'est fait une fois pour toutes !
    const int letters[26][15] = {
        {0,1,0, 1,0,1, 1,1,1, 1,0,1, 1,0,1}, // A
        {1,1,0, 1,0,1, 1,1,0, 1,0,1, 1,1,0}, // B
        {0,1,1, 1,0,0, 1,0,0, 1,0,0, 0,1,1}, // C
        {1,1,0, 1,0,1, 1,0,1, 1,0,1, 1,1,0}, // D
        {1,1,1, 1,0,0, 1,1,1, 1,0,0, 1,1,1}, // E
        {1,1,1, 1,0,0, 1,1,0, 1,0,0, 1,0,0}, // F
        {0,1,1, 1,0,0, 1,0,1, 1,0,1, 0,1,1}, // G
        {1,0,1, 1,0,1, 1,1,1, 1,0,1, 1,0,1}, // H
        {1,1,1, 0,1,0, 0,1,0, 0,1,0, 1,1,1}, // I
        {0,0,1, 0,0,1, 0,0,1, 1,0,1, 0,1,0}, // J
        {1,0,1, 1,0,1, 1,1,0, 1,0,1, 1,0,1}, // K
        {1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,1,1}, // L
        {1,0,1, 1,1,1, 1,0,1, 1,0,1, 1,0,1}, // M
        {1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,0,1}, // N
        {0,1,0, 1,0,1, 1,0,1, 1,0,1, 0,1,0}, // O
        {1,1,0, 1,0,1, 1,1,0, 1,0,0, 1,0,0}, // P
        {0,1,0, 1,0,1, 1,0,1, 0,1,1, 0,1,1}, // Q
        {1,1,0, 1,0,1, 1,1,0, 1,0,1, 1,0,1}, // R
        {0,1,1, 1,0,0, 0,1,0, 0,0,1, 1,1,0}, // S
        {1,1,1, 0,1,0, 0,1,0, 0,1,0, 0,1,0}, // T
        {1,0,1, 1,0,1, 1,0,1, 1,0,1, 0,1,0}, // U
        {1,0,1, 1,0,1, 1,0,1, 1,0,1, 0,1,0}, // V (Simplifié)
        {1,0,1, 1,0,1, 1,0,1, 1,1,1, 1,0,1}, // W
        {1,0,1, 1,0,1, 0,1,0, 1,0,1, 1,0,1}, // X
        {1,0,1, 1,0,1, 0,1,0, 0,1,0, 0,1,0}, // Y
        {1,1,1, 0,0,1, 0,1,0, 1,0,0, 1,1,1}  // Z
    };

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 3; col++) {
            if (letters[index][row * 3 + col]) {
                SDL_FRect pixel = {x + (col * size), y + (row * size), size, size};
                SDL_RenderFillRect(rend, &pixel);
            }
        }
    }
}

// --- DEFINITION DES SPRITES (11 pixels de large x 8 de haut) ---

// 1 = Pixel allumé, 0 = Transparent

// Le "Calamar" (Ligne du haut)
const int sprite_squid[8][11] = {
    {0,0,0,0,1,1,1,0,0,0,0},
    {0,0,0,1,1,1,1,1,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,1,1,0,1,1,1,0,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,1,0,0,0,1,0,0,0},
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,1,0,0,0}
};

// Le "Crabe" (Milieu)
const int sprite_crab[8][11] = {
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,1,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,1,1,0,1,1,1,0,1,1,0},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,1,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,1,0,1},
    {0,0,0,1,1,0,1,1,0,0,0}
};

// Le "Poulpe" (Bas)
const int sprite_octopus[8][11] = {
    {0,0,0,0,1,1,1,0,0,0,0},
    {0,0,0,1,1,1,1,1,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,1,1,0,1,1,1,0,1,1,0},
    {1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,1,1,0,1,1,0,0,0},
    {0,0,1,1,0,0,0,1,1,0,0},
    {1,1,0,0,0,0,0,0,0,1,1}
};

// Le Vaisseau du Joueur (Un peu plus large : 13x8)
const int sprite_player[8][13] = {
    {0,0,0,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Fonction générique pour dessiner un sprite depuis une matrice
// rows/cols : dimensions de la matrice
// pixel_scale : taille d'un "petit pixel" (ex: 0.1f)
void draw_sprite_shape(const int *sprite_data, int rows, int cols, float x, float y, float pixel_scale) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // On fait un peu de math pointeur pour lire le tableau 2D comme un 1D
            if (sprite_data[r * cols + c]) {
                SDL_FRect pixel = {
                    x + (c * pixel_scale),
                    y + (r * pixel_scale),
                    pixel_scale, pixel_scale
                };
                SDL_RenderFillRect(rend, &pixel);
            }
        }
    }
}

// Dessine un chiffre spécifique à une position (x, y)
// size = taille d'un "pixel" du chiffre
void draw_digit(int digit, float x, float y, float size) {
    // Représentation binaire des chiffres 0-9 (Matrice 5 lignes x 3 colonnes)
    // 1 = Dessiner, 0 = Vide
    const int font[10][15] = {
        {1,1,1, 1,0,1, 1,0,1, 1,0,1, 1,1,1}, // 0
        {0,1,0, 0,1,0, 0,1,0, 0,1,0, 0,1,0}, // 1
        {1,1,1, 0,0,1, 1,1,1, 1,0,0, 1,1,1}, // 2
        {1,1,1, 0,0,1, 1,1,1, 0,0,1, 1,1,1}, // 3
        {1,0,1, 1,0,1, 1,1,1, 0,0,1, 0,0,1}, // 4
        {1,1,1, 1,0,0, 1,1,1, 0,0,1, 1,1,1}, // 5
        {1,1,1, 1,0,0, 1,1,1, 1,0,1, 1,1,1}, // 6
        {1,1,1, 0,0,1, 0,0,1, 0,0,1, 0,0,1}, // 7
        {1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,1,1}, // 8
        {1,1,1, 1,0,1, 1,1,1, 0,0,1, 1,1,1}  // 9
    };

    if (digit < 0 || digit > 9) return;

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // Blanc

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 3; col++) {
            if (font[digit][row * 3 + col]) {
                SDL_FRect pixel = {
                    x + (col * size), 
                    y + (row * size), 
                    size, size
                };
                SDL_RenderFillRect(rend, &pixel);
            }
        }
    }
}

float draw_text(const char* text, float x, float y, float size) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= '0' && text[i] <= '9') {
            draw_digit(text[i] - '0', x, y, size);
        } else if (text[i] == ' ') {
            // Juste un espace vide
        } else {
            draw_alpha(text[i], x, y, size);
        }
        x += 4 * size; // Espacement (3 pour la lettre + 1 vide)
    }
    return x;
}

// Dessine un nombre complet (ex: 150)
void draw_number(int number, float x, float y, float size) {
    char buffer[10];
    sprintf(buffer, "%d", number); // Convertit le int en string "150"
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        // On convertit le char '0'-'9' en int 0-9
        int digit = buffer[i] - '0'; 
        
        // On dessine le chiffre
        draw_digit(digit, x + (i * 4 * size), y, size); 
        // (i * 4 * size) permet d'espacer les chiffres
    }
}

void draw_game_view(GameState *game) {
    // 1. Fond Noir et Nettoyage
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    // --- INTERFACE (HUD) ---
    float text_size = 0.25f;
    float hud_y = 0.8f; // Hauteur du texte

    // A. SCORE (Décalé à gauche, x=5)
    float cursor_x = 5.0f; 
    cursor_x = draw_text("SCORE ", cursor_x, hud_y, text_size);
    draw_number(game->score, cursor_x, hud_y, text_size);
    
    // B. VIES (Collé à droite)
    float lives_x = game->width - 15.0f; // Marge suffisante à droite
    lives_x = draw_text("LIVES ", lives_x, hud_y, text_size);
    
    // Vérifie si ta variable s'appelle 'lives' ou 'nb_lives' dans model.h !
    draw_number(game->nb_lives, lives_x, hud_y, text_size); 

    // --- DESSIN DES ENNEMIS ---
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (game->enemies[i].alive) {
            float scale = 0.07f;
            float draw_x = game->enemies[i].x + 0.1f;
            float draw_y = game->enemies[i].y;

            switch (game->enemies[i].type) {
                case SQUID:
                    draw_sprite_shape((const int*)sprite_squid, 8, 11, draw_x, draw_y, scale);
                    break;
                case CRABS: // Attention: Vérifie que c'est bien CRAB dans model.h (pas CRABS)
                    draw_sprite_shape((const int*)sprite_crab, 8, 11, draw_x, draw_y, scale);
                    break;
                case OCTOPUS:
                    draw_sprite_shape((const int*)sprite_octopus, 8, 11, draw_x, draw_y, scale);
                    break;
            }
        }
    }

    // --- DESSIN DU JOUEUR (Il manquait dans ton code !) ---
    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
    // On utilise le sprite du joueur défini plus haut
    draw_sprite_shape((const int*)sprite_player, 8, 13, game->player.x, game->player.y, 0.08f);

    // --- BALLES JOUEUR ---
    SDL_SetRenderDrawColor(rend, 255, 255, 0, 255);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) {
            SDL_FRect b_rect = {(float)game->bullets[i].x + 0.4f, (float)game->bullets[i].y, 0.2f, 1.0f};
            SDL_RenderFillRect(rend, &b_rect);
        }
    }
    
    // --- BALLES ENNEMIS ---
    SDL_SetRenderDrawColor(rend, 255, 50, 50, 255);
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (game->enemy_bullets[i].active) {
            SDL_FRect eb_rect = {(float)game->enemy_bullets[i].x + 0.4f, (float)game->enemy_bullets[i].y, 0.2f, 1.0f};
            SDL_RenderFillRect(rend, &eb_rect);
        }
    }
    
    SDL_RenderPresent(rend);
}

void draw_sdl_view(GameState *game) {
    if (game->currView == ACCUEIL) {
        draw_menu_view(game);
    } else if (game->currView == JEU) {
        draw_game_view(game);
    } else if (game->currView == MENU_JEU) { // le nouveau menu si "echap" en jeu
        draw_pause_menu(game);
    }
    SDL_RenderPresent(rend);
}

void close_sdl_view() {
    if (rend) SDL_DestroyRenderer(rend); 
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}