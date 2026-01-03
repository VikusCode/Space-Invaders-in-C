#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include "../include/model.h"
#include "../include/view_sdl.h"
#include "../assets/sprite.h"

static SDL_Window *window = NULL;
static SDL_Renderer *rend = NULL; 

/**
 * @file view_sdl.c
 * @brief Vue SDL : rendu et gestion des vues pour la version SDL.
 *
 * Contient l'initialisation de la fenêtre/renderer, le dessin des
 * différents écrans (menu, instructions, jeu) et la gestion audio.
 */

/**
 * @brief Convertit des coordonnées fenêtre en coordonnées logiques.
 *
 * Convertit les coordonnées reçues par SDL en coordonnées
 * logiques utilisées par le moteur de jeu.
 *
 * @param x Pointeur sur l'abscisse (modifiée en place).
 * @param y Pointeur sur l'ordonnée (modifiée en place).
 */
void convert_mouse_coordinates(float *x, float *y) {
    if (rend) {
        SDL_RenderCoordinatesFromWindow(rend, *x, *y, x, y);
    }
}

/**
 * @brief Initialise la vue SDL (fenêtre et renderer).
 *
 * Configure la présentation logique pour correspondre aux dimensions
 * du jeu et crée la fenêtre plein écran.
 *
 * @param game Pointeur vers l'état du jeu.
 * @return int 1 si succès, 0 en cas d'erreur.
 */
int init_sdl_view(GameState *game) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 0; 
    }

    game->currView = ACCUEIL;

    SDL_DisplayID dis = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(dis);

    if(!mode) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erreur mode: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    int width = mode->w, height = mode->h;

    // Création fenêtre (On garde le flag ici, c'est bien)
    window = SDL_CreateWindow("Jeu Space Invader !", width, height, SDL_WINDOW_FULLSCREEN);

    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erreur window : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;    
    }

    rend = SDL_CreateRenderer(window, NULL); 

    if(!rend) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erreur rend : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;    
    }

    SDL_SetWindowFullscreen(window, true); 

    SDL_SetRenderLogicalPresentation(rend, game->width, game->height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return 1; 
}

/**
 * @brief Dessine l'écran d'accueil (boutons et titre).
 *
 * @param game Pointeur vers l'état du jeu.
 */
void draw_menu_view(GameState *game) {
    SDL_SetRenderDrawColor(rend, 10, 10, 40, 255);
    SDL_RenderClear(rend);

    float btn_w = 26.0f; 
    float btn_h = 6.0f;
    float gap = 2.0f;    
    
    float total_h = (btn_h * 3) + (gap * 2);
    
    float start_y = (game->height - total_h) / 2;
    float btn_x = (game->width - btn_w) / 2;

    float play_y = start_y;
    float instr_y = start_y + btn_h + gap;        
    float quit_y = start_y + (btn_h + gap) * 2;    

    float text_size = 0.35f; 

    // Bouton JOUER (Vert)
    SDL_SetRenderDrawColor(rend, 0, 200, 0, 255);
    SDL_FRect play_rect = {btn_x, play_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &play_rect);
    draw_text("JOUER", btn_x + 9.5f, play_y + 1.5f, text_size); 

    // Bouton INSTRUCTIONS (Bleu)
    SDL_SetRenderDrawColor(rend, 0, 100, 255, 255);
    SDL_FRect instr_rect = {btn_x, instr_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &instr_rect);
    draw_text("INSTRUCTIONS", btn_x + 5.0f, instr_y + 1.5f, text_size);

    // Bouton QUITTER (Rouge)
    SDL_SetRenderDrawColor(rend, 200, 0, 0, 255);
    SDL_FRect quit_rect = {btn_x, quit_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &quit_rect);
    draw_text("QUITTER", btn_x + 8.0f, quit_y + 1.5f, text_size);

    // Nom du jeu en haut
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // Blanc pour le titre
    draw_text("SPACE INVADERS", (game->width - (14 * 4 * 0.5)) / 2, 5.0f, 0.5f);

    // Message en bas
    SDL_SetRenderDrawColor(rend, 150, 150, 150, 255); // Gris clair
    float footer_size = 0.20f; 
    
    float msg_width = 49 * 4 * footer_size; // calcul largeur du message avec le nombre de lettres
    
    // Centrage horizontal
    float msg_x = (game->width - msg_width) / 2;
    float msg_y = game->height - 4.0f; 

    draw_text("UTILISE TA SOURIS OU CLIQUE SUR ENTREE POUR JOUER", msg_x, msg_y, footer_size);
}

/**
 * @brief Dessine l'écran d'instructions avec commandes et but du jeu.
 *
 * @param game Pointeur vers l'état du jeu.
 */
void draw_instructions(GameState *game) {
    // Fond Bleu foncé
    SDL_SetRenderDrawColor(rend, 10, 10, 40, 255);
    SDL_RenderClear(rend);

    float cx = game->width / 2; // Centre de l'écran
    float text_size = 0.3f;     // Taille du texte standard
    float title_size = 0.5f;    // Taille du titre
    
    float y_title = 2.0f;           // Titre tout en haut
    float y_start_cols = 8.0f;      // Les colonnes commencent bien plus haut
    
    float col_left_x = 5.0f;        // Marge gauche
    float col_right_x = cx + 2.0f;  // Marge droite (juste après le milieu)

    // Titre INSTRUCTIONS 
    SDL_SetRenderDrawColor(rend, 255, 255, 0, 255); // Jaune
    draw_text("INSTRUCTIONS", cx - (12 * 4 * title_size) / 2, y_title, title_size);

    // Colonne gauche : COMMANDES
    SDL_SetRenderDrawColor(rend, 0, 255, 255, 255); // Cyan
    float y_cmd = y_start_cols;
    
    draw_text("ESPACE POUR TIRER", col_left_x, y_cmd, text_size);
    y_cmd += 4.0f;
    draw_text("Q POUR GAUCHE", col_left_x, y_cmd, text_size);
    y_cmd += 4.0f;
    draw_text("D POUR DROITE", col_left_x, y_cmd, text_size);


    // Colonne droite : BUT DU JEU
    float y_right = y_start_cols;

    // Le but (Blanc)
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); 
    draw_text("VOTRE BUT EST DE TUER", col_right_x, y_right, text_size);
    y_right += 3.0f;
    draw_text("LES ENNEMIS AU COMPLET", col_right_x, y_right, text_size);
    
    y_right += 6.0f; 

    // Les vies (Rouge)
    SDL_SetRenderDrawColor(rend, 255, 100, 100, 255); 
    float cursor = draw_text("VOUS AVEZ ", col_right_x, y_right, text_size);
    draw_number(3, cursor, y_right, text_size); 
    draw_text(" VIES", cursor + 3.0f, y_right, text_size);
    y_right += 3.0f;
    draw_text("NE LES GASPILLEZ PAS", col_right_x, y_right, text_size);

    
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
    
    // Rejouer (Touche Entrée)
    draw_text("ENTREE POUR CONTINUER", cx - 15.0f, cy + 8.0f, 0.3f);
    
    // Accueil (Touche Echap)
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

    draw_text("PAUSE", (game->width - (5 * 4 * 0.5)) / 2, 5.0f, 0.5f);

    // bouton RECOMMENCER (Cyan)
    SDL_SetRenderDrawColor(rend, 0, 200, 200, 255);
    SDL_FRect restart_rect = {btn_x, restart_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &restart_rect);
    
    draw_text("RECOMMENCER", btn_x + 5.2f, restart_y + 1.5f, text_size);

    // quitter (Rouge)
    SDL_SetRenderDrawColor(rend, 200, 0, 0, 255);
    SDL_FRect quit_rect = {btn_x, quit_y, btn_w, btn_h};
    SDL_RenderFillRect(rend, &quit_rect);

    draw_text("QUITTER", btn_x + 8.0f, quit_y + 1.5f, text_size);
}

void draw_alpha(char c, float x, float y, float size) {
    if (c >= 'a' && c <= 'z') c -= 32; 
    
    if (c < 'A' || c > 'Z') return;

    int index = c - 'A'; 

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

//nouvelle fonction
int init_audio(GameState *game) {
    // 1. Init de la librairie Mixer
    if (!MIX_Init()) {
        SDL_Log("MIX_Init failed: %s", SDL_GetError());
        return 0;
    }

    // 2. Création du Device
    game->mixerDevice = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!game->mixerDevice) {
        SDL_Log("MIX_CreateMixerDevice Error: %s", SDL_GetError());
        return 0;
    }

    // Chargement des audios
    game->audio_music = MIX_LoadAudio(game->mixerDevice, "assets/music.mp3", true);
    game->audio_shoot = MIX_LoadAudio(game->mixerDevice, "assets/shoot.wav", true);
    game->audio_explosion = MIX_LoadAudio(game->mixerDevice, "assets/explosion.wav", true);

    
    // Chargement des tracks
    // Musique de fond
    if (game->audio_music) {
        game->track_music = MIX_CreateTrack(game->mixerDevice);
        MIX_SetTrackAudio(game->track_music, game->audio_music);
        // J'ai mis -1 pour que ca tourne en boucle
        MIX_PlayTrack(game->track_music, -1);
    }
    
    // Shoot player
    if (game->audio_shoot) {
        game->track_shoot = MIX_CreateTrack(game->mixerDevice);
        MIX_SetTrackAudio(game->track_shoot, game->audio_shoot);
    }

    // Shoot ennemi
    if (game->audio_explosion) {
        game->track_explosion = MIX_CreateTrack(game->mixerDevice);
        MIX_SetTrackAudio(game->track_explosion, game->audio_explosion);
    }
    
    return 1;
}

// nouvelle fonction
void cleanup_audio(GameState *game) {
    // Derstruction des tracks
    if (game->track_shoot) MIX_DestroyTrack(game->track_shoot);
    if (game->track_music) MIX_DestroyTrack(game->track_music);
    if (game->track_explosion) MIX_DestroyTrack(game->track_explosion);

    // Destruction des audios
    if (game->audio_music) MIX_DestroyAudio(game->audio_music);
    if (game->audio_shoot) MIX_DestroyAudio(game->audio_shoot);
    if (game->audio_explosion) MIX_DestroyAudio(game->audio_explosion);

    // Destruction du mixerDevice
    if (game->mixerDevice) MIX_DestroyMixer(game->mixerDevice);
}

void draw_sdl_view(GameState *game) {
    if (game->currView == ACCUEIL) {
        MIX_PauseTrack(game->track_music); 
        draw_menu_view(game);
    } else if (game->currView == INSTRUCTION) { 
        draw_instructions(game); 
    }else if (game->currView == JEU) {   
        draw_game_view(game); 
    } else if (game->currView == MENU_JEU) { 
        draw_pause_menu(game); 
        MIX_PauseTrack(game->track_music); 
    } else if (game->currView == MENU_GAGNE) {
        MIX_PauseTrack(game->track_music); 
        draw_win_view(game);       
    } else if (game->currView == MENU_PERD) {
        MIX_PauseTrack(game->track_music); 
        draw_lose_view(game);      
    }
    SDL_RenderPresent(rend);
}

void close_sdl_view() {
    if (rend) SDL_DestroyRenderer(rend); 
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}