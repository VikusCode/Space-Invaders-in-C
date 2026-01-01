#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include "../include/view_sdl.h"
#include "../assets/draw.h"

static SDL_Window *window = NULL;
static SDL_Renderer *rend = NULL; 

void convert_mouse_coordinates(float *x, float *y) {
    if (rend) {
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
    SDL_SetRenderDrawColor(rend, 10, 10, 40, 255);
    SDL_RenderClear(rend);

    // --- DIMENSIONS ---
    float btn_w = 26.0f; // Un peu plus large pour le mot "INSTRUCTIONS"
    float btn_h = 6.0f;
    float gap = 2.0f;    // Espace entre les boutons
    
    // Calcul pour 3 BOUTONS maintenant
    float total_h = (btn_h * 3) + (gap * 2);
    
    float start_y = (game->height - total_h) / 2;
    float btn_x = (game->width - btn_w) / 2;

    // Positions Y des 3 boutons
    float play_y = start_y;
    float instr_y = start_y + btn_h + gap;         // Nouveau bouton milieu
    float quit_y = start_y + (btn_h + gap) * 2;    // Quitter descend en bas

    float text_size = 0.35f; // Texte un peu plus petit pour tout faire rentrer

    // 1. BOUTON JOUER (Vert)
    SDL_SetRenderDrawColor(rend, 0, 200, 0, 255);
    SDL_FRect play_rect = {btn_x, play_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &play_rect);
    draw_text("JOUER", btn_x + 9.5f, play_y + 1.5f, text_size); // Centré à vue d'oeil

    // 2. BOUTON INSTRUCTIONS (Bleu) - NOUVEAU
    SDL_SetRenderDrawColor(rend, 0, 100, 255, 255);
    SDL_FRect instr_rect = {btn_x, instr_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &instr_rect);
    draw_text("INSTRUCTIONS", btn_x + 3.0f, instr_y + 1.5f, text_size);

    // 3. BOUTON QUITTER (Rouge)
    SDL_SetRenderDrawColor(rend, 200, 0, 0, 255);
    SDL_FRect quit_rect = {btn_x, quit_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &quit_rect);
    draw_text("QUITTER", btn_x + 8.0f, quit_y + 1.5f, text_size);

    // Titre
    draw_text("SPACE INVADERS", (game->width - (14 * 4 * 0.5)) / 2, 5.0f, 0.5f);
}

void draw_instructions(GameState *game) {
    // 1. Fond bleu nuit
    SDL_SetRenderDrawColor(rend, 10, 10, 40, 255);
    SDL_RenderClear(rend);

    float cx = game->width / 2; // Centre de l'écran
    float text_size = 0.3f;     // Taille du texte standard
    float title_size = 0.5f;    // Taille du titre
    
    // --- NOUVELLES POSITIONS (PLUS HAUTES) ---
    float y_title = 2.0f;           // Titre tout en haut
    float y_start_cols = 8.0f;      // Les colonnes commencent bien plus haut
    
    float col_left_x = 5.0f;        // Marge gauche
    float col_right_x = cx + 2.0f;  // Marge droite (juste après le milieu)


    // --- TITRE ---
    SDL_SetRenderDrawColor(rend, 255, 255, 0, 255); // Jaune
    draw_text("INSTRUCTIONS", cx - (12 * 4 * title_size) / 2, y_title, title_size);


    // --- COLONNE GAUCHE : LES COMMANDES ---
    SDL_SetRenderDrawColor(rend, 0, 255, 255, 255); // Cyan
    float y_cmd = y_start_cols;
    
    draw_text("ESPACE POUR TIRER", col_left_x, y_cmd, text_size);
    y_cmd += 4.0f;
    draw_text("Q POUR GAUCHE", col_left_x, y_cmd, text_size);
    y_cmd += 4.0f;
    draw_text("D POUR DROITE", col_left_x, y_cmd, text_size);


    // --- COLONNE DROITE : BUT ET VIES ---
    float y_right = y_start_cols;

    // 1. LE BUT DU JEU (Blanc)
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); 
    draw_text("VOTRE BUT EST DE TUER", col_right_x, y_right, text_size);
    y_right += 3.0f;
    draw_text("LES ENNEMIS AU COMPLET", col_right_x, y_right, text_size);
    
    y_right += 6.0f; // Espace un peu réduit pour être sûr que ça passe

    // 2. LES VIES (Rouge clair)
    SDL_SetRenderDrawColor(rend, 255, 100, 100, 255); 
    float cursor = draw_text("VOUS AVEZ ", col_right_x, y_right, text_size);
    draw_number(3, cursor, y_right, text_size); 
    draw_text(" VIES", cursor + 3.0f, y_right, text_size);
    y_right += 3.0f;
    draw_text("NE LES GASPILLEZ PAS", col_right_x, y_right, text_size);


    // --- PIED DE PAGE ---
    // On le descend un peu plus bas (height - 8 au lieu de height - 12)
    
    // "Bonne chance terrien !" (Vert)
    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
    draw_text("BONNE CHANCE TERRIEN", cx - (20 * 4 * text_size) / 2, game->height - 8.0f, text_size);
    
    // Bouton retour (Gris)
    SDL_SetRenderDrawColor(rend, 100, 100, 100, 255); 
    draw_text("ENTREE POUR RETOUR", cx - 12.0f, game->height - 4.0f, 0.25f);
}

void draw_win_view(GameState *game) {
    // Fond Vert foncé
    SDL_SetRenderDrawColor(rend, 0, 50, 0, 255); 
    SDL_RenderClear(rend);

    float cx = game->width / 2;
    float cy = game->height / 2;

    // Grand Titre
    draw_text("VICTOIRE !", cx - 10.0f, cy - 5.0f, 0.6f);
    
    // Affichage du Score final
    draw_text("SCORE FINAL", cx - 11.0f, cy, 0.4f);
    draw_number(game->score, cx + 12.0f, cy, 0.4f);

    // --- INSTRUCTIONS DE NAVIGATION ---
    
    // 1. Rejouer (Touche Entrée)
    draw_text("ENTREE POUR CONTINUER", cx - 15.0f, cy + 8.0f, 0.3f);
    
    // 2. Retour Accueil (Touche Echap) - AJOUT ICI
    draw_text("ECHAP POUR ACCUEIL", cx - 14.5f, cy + 12.0f, 0.3f);
}

void draw_lose_view(GameState *game) {
    // Fond Rouge foncé
    SDL_SetRenderDrawColor(rend, 50, 0, 0, 255); 
    SDL_RenderClear(rend);

    float cx = game->width / 2;
    float cy = game->height / 2;

    draw_text("GAME OVER", cx - 9.0f, cy - 5.0f, 0.6f);
    
    draw_text("SCORE", cx - 6.0f, cy, 0.4f);
    draw_number(game->score, cx + 6.0f, cy, 0.4f);

    draw_text("ENTREE POUR REJOUER", cx - 15.0f, cy + 8.0f, 0.3f);
    draw_text("ECHAP POUR ACCUEIL", cx - 14.5f, cy + 12.0f, 0.3f);
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

    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
    
    for (int i = 0; i < MAX_SHIELD_BRICKS; i++) {
        // On ne dessine que les briques actives (celles qui n'ont pas été touchées)
        if (game->shields[i].active) {
            SDL_FRect brick_rect = {
                game->shields[i].x, 
                game->shields[i].y, 
                1.0f, 1.0f  // Taille d'une brique = 1 case de la grille
            };
            SDL_RenderFillRect(rend, &brick_rect);
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
    if (game->currView == ACCUEIL)       draw_menu_view(game);
    else if (game->currView == INSTRUCTION) draw_instructions(game); // <--- Ici
    else if (game->currView == JEU)      draw_game_view(game);
    else if (game->currView == MENU_JEU) draw_pause_menu(game);
    else if (game->currView == MENU_GAGNE) draw_win_view(game);       // <--- Ici
    else if (game->currView == MENU_PERD) draw_lose_view(game);      // <--- Ici
    
    SDL_RenderPresent(rend);
}

void close_sdl_view() {
    if (rend) SDL_DestroyRenderer(rend); 
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}