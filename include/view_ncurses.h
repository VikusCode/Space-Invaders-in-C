/**
 * @file view_ncurses.h
 * @brief Vue ncurses pour Space Invaders
 */

#ifndef VIEW_NCURSES_H
#define VIEW_NCURSES_H

#include "model.h"

/* ============================================================================
 * CONSTANTES DE COULEURS
 * ============================================================================ */

#define COLOR_PLAYER        1
#define COLOR_ENEMY         2
#define COLOR_BULLET        3
#define COLOR_ENEMY_BULLET  4
#define COLOR_SHIELD        5
#define COLOR_HUD           6
#define COLOR_BORDER        7
#define COLOR_TITLE         8
#define COLOR_MENU          9
#define COLOR_GAME_OVER     10
#define COLOR_WIN           11
#define COLOR_PAUSE         12

/* ============================================================================
 * CONSTANTES DE CARACTÈRES
 * ============================================================================ */

#define PLAYER_CHAR         'A'
#define BULLET_CHAR         '|'
#define ENEMY_BULLET_CHAR   '!'
#define SHIELD_CHAR         '#'

/* ============================================================================
 * FONCTIONS PUBLIQUES
 * ============================================================================ */

/**
 * @brief Initialise ncurses
 */
void init_ncurses(void);

/**
 * @brief Nettoie et ferme ncurses
 */
void cleanup_ncurses(void);

/**
 * @brief Affiche l'état du jeu selon la vue actuelle
 * @param game Pointeur vers l'état du jeu
 */
void render_ncurses(GameState *game);



// /**
//  * @brief Gère les entrées clavier
//  * @param game Pointeur vers l'état du jeu
//  * @return 1 pour continuer, 0 pour quitter
//  */
// int handle_input_ncurses(GameState *game);

#endif /* VIEW_NCURSES_H */