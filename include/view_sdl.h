/**
 * @file view_sdl.h
 * @brief Interface de la vue SDL (prototypes et constantes).
 */

#ifndef VIEW_SDL_H
#define VIEW_SDL_H
#include "model.h"

/** Position / taille par défaut des boutons (unités logiques) */
#define BTN_X 40
#define BTN_Y 18
#define BTN_H 4
#define BTN_W 20
#define BOUCLE_FOND 10000000

/**
 * @brief Initialise la vue SDL (fenêtre et renderer).
 * @return 1 si succès, 0 sinon.
 */
int init_sdl_view(GameState *game);

/** @brief Dessine la vue SDL principale (menu, jeu, etc.). */
void draw_sdl_view(GameState *game);

/** @brief Dessine l'écran de jeu (HUD, entités). */
void draw_game_view(GameState *game);

/** @brief Dessine une chaîne de texte et renvoie la nouvelle position x. */
float draw_text(const char* text, float x, float y, float size);

/** @brief Ferme/cleanup la vue SDL. */
void close_sdl_view(void);

/** @brief Dessine un nombre à l'écran. */
void draw_number(int number, float x, float y, float size);

/** @brief Initialisation audio (SDL_mixer). */
int init_audio(GameState *game);

/** @brief Nettoyage audio. */
void cleanup_audio(GameState *game);

/** @brief Dessine le menu de pause. */
void draw_pause_menu(GameState *game);

/** @brief Convertit des coordonnées fenêtre en coordonnées logiques. */
void convert_mouse_coordinates(float *x, float *y);

/** @brief Dessine le menu d'accueil. */
void draw_menu_view(GameState *game);

#endif /* VIEW_SDL_H */