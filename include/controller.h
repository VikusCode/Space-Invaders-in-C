/**
 * @file controller.h
 * @brief Prototypes du contrôleur (gestion des événements et saisie).
 */

#pragma once

#include "../include/model.h"

/**
 * @brief Traite les événements SDL et met à jour l'état du jeu.
 *
 * @param game Pointeur vers l'état du jeu.
 * @param running Pointeur vers le drapeau d'exécution (0 = arrêter).
 */
void handle_event(GameState *game, int *running);

/**
 * @brief Lit et traite l'entrée clavier en mode ncurses.
 *
 * @param game Pointeur vers l'état du jeu.
 * @return int 0 pour quitter, 1 pour continuer.
 */
int handle_input_ncurses(GameState *game);