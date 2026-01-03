/**
 * @file view_ncurses.c
 * @brief Implémentation de la vue ncurses pour Space Invaders
 */

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/view_ncurses.h"
#include "../include/model.h"
#include "../assets/sprite.h"

/* ============================================================================
 * FONCTIONS PRIVÉES - AFFICHAGE DES ÉLÉMENTS
 * ============================================================================ */

/**
 * @file view_ncurses.c
 * @brief Vue ncurses : dessin et affichage pour la version terminal.
 *
 * Contient les fonctions d'initialisation de ncurses, rendu et nettoyage.
 */

/**
 * @brief Affiche le joueur
 */
static void draw_player(GameState *game) {
    if (game->player.y >= 0 && game->player.y < LINES && 
        game->player.x >= 0 && game->player.x < COLS) {
        attron(COLOR_PAIR(COLOR_PLAYER));
        mvaddch(game->player.y, game->player.x, PLAYER_CHAR);
        attroff(COLOR_PAIR(COLOR_PLAYER));
    }
}

/**
 * @brief Affiche un ennemi avec son sprite
 */
static void draw_enemy(Enemy *enemy, int frame) {
    if (!enemy->alive) return;
    
    attron(COLOR_PAIR(COLOR_ENEMY));
    
    // Sélectionner le sprite selon le type
    const char *sprite;
    switch (enemy->type) {
        case SQUID:
            sprite = squid;
            break;
        case CRABS:
            sprite = crab;
            break;
        case OCTOPUS:
            sprite = octo;
            break;
    }
    
    if (sprite && enemy->y >= 0 && enemy->y < LINES && 
        enemy->x >= 0 && enemy->x < COLS) {
        mvaddch(enemy->y, enemy->x, sprite[0]);
    }
    
    attroff(COLOR_PAIR(COLOR_ENEMY));
}

/**
 * @brief Affiche tous les ennemis
 */
static void draw_enemies(GameState *game) {
    static int animation_frame = 0;
    animation_frame++;
    
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        draw_enemy(&game->enemies[i], animation_frame / 15);
    }
}

/**
 * @brief Affiche les projectiles du joueur
 */
static void draw_bullets(GameState *game) {
    attron(COLOR_PAIR(COLOR_BULLET));
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active && 
            game->bullets[i].y >= 0 && game->bullets[i].y < LINES && 
            game->bullets[i].x >= 0 && game->bullets[i].x < COLS) {
            mvaddch(game->bullets[i].y, game->bullets[i].x, BULLET_CHAR);
        }
    }
    attroff(COLOR_PAIR(COLOR_BULLET));
}

/**
 * @brief Affiche les projectiles ennemis
 */
static void draw_enemy_bullets(GameState *game) {
    attron(COLOR_PAIR(COLOR_ENEMY_BULLET));
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (game->enemy_bullets[i].active && 
            game->enemy_bullets[i].y >= 0 && game->enemy_bullets[i].y < LINES && 
            game->enemy_bullets[i].x >= 0 && game->enemy_bullets[i].x < COLS) {
            mvaddch(game->enemy_bullets[i].y, game->enemy_bullets[i].x, ENEMY_BULLET_CHAR);
        }
    }
    attroff(COLOR_PAIR(COLOR_ENEMY_BULLET));
}

/**
 * @brief Affiche les boucliers
 */
static void draw_shields(GameState *game) {
    attron(COLOR_PAIR(COLOR_SHIELD));
    for (int i = 0; i < MAX_SHIELD_BRICKS; i++) {
        if (game->shields[i].active && 
            game->shields[i].y >= 0 && game->shields[i].y < LINES && 
            game->shields[i].x >= 0 && game->shields[i].x < COLS) {
            mvaddch(game->shields[i].y, game->shields[i].x, SHIELD_CHAR);
        }
    }
    attroff(COLOR_PAIR(COLOR_SHIELD));
}

/**
 * @brief Affiche le HUD (score, vies)
 */
static void draw_hud(GameState *game) {
    attron(COLOR_PAIR(COLOR_HUD));
    mvprintw(0, 2, "SCORE: %05d", game->score);
    mvprintw(0, 20, "VIES: %d", game->nb_lives);
    mvprintw(0, COLS - 20, "[P]ause [Q]uit");
    attroff(COLOR_PAIR(COLOR_HUD));
}

/**
 * @brief Affiche une bordure autour de l'écran de jeu
 */
static void draw_border(int height, int width) {
    attron(COLOR_PAIR(COLOR_BORDER));
    for (int i = 0; i < width; i++) {
        mvaddch(1, i, '-');
        mvaddch(height - 1, i, '-');
    }
    for (int i = 1; i < height; i++) {
        mvaddch(i, 0, '|');
        mvaddch(i, width - 1, '|');
    }
    mvaddch(1, 0, '+');
    mvaddch(1, width - 1, '+');
    mvaddch(height - 1, 0, '+');
    mvaddch(height - 1, width - 1, '+');
    attroff(COLOR_PAIR(COLOR_BORDER));
}

/* ============================================================================
 * FONCTIONS PRIVÉES - MENUS ET ÉCRANS
 * ============================================================================ */

/**
 * @brief Affiche l'écran d'accueil
 */
static void draw_menu_accueil(void) {
    int center_y = LINES / 2;
    int center_x = COLS / 2;
    
    clear();
    
    attron(COLOR_PAIR(COLOR_TITLE) | A_BOLD);
    mvprintw(center_y - 6, center_x - 15, "                                 ");
    mvprintw(center_y - 5, center_x - 15, "                                 ");
    mvprintw(center_y - 4, center_x - 15, "      SPACE INVADERS v1.0        ");
    mvprintw(center_y - 3, center_x - 15, "                                 ");
    mvprintw(center_y - 2, center_x - 15, "                                 ");
    attroff(COLOR_PAIR(COLOR_TITLE) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_MENU));
    mvprintw(center_y, center_x - 12, "Appuyez sur ENTREE");
    mvprintw(center_y + 1, center_x - 8, "pour commencer");
    attroff(COLOR_PAIR(COLOR_MENU));
    
    mvprintw(center_y + 4, center_x - 10, "Commandes:");
    mvprintw(center_y + 5, center_x - 10, "  ← →  : Deplacer");
    mvprintw(center_y + 6, center_x - 10, "  ESPACE : Tirer");
    mvprintw(center_y + 7, center_x - 10, "  P : Pause");
    mvprintw(center_y + 8, center_x - 10, "  Q : Quitter");
    
    // Afficher les sprites des ennemis
    attron(COLOR_PAIR(COLOR_ENEMY));
    mvprintw(center_y + 11, center_x - 15, "%s = 40 pts", squid);
    mvprintw(center_y + 12, center_x - 15, "%s = 20 pts", crab);
    mvprintw(center_y + 13, center_x - 15, "%s = 10 pts", octo);
    attroff(COLOR_PAIR(COLOR_ENEMY));
}

/**
 * @brief Affiche l'écran de défaite
 */
static void draw_menu_perd(GameState *game) {
    int center_y = LINES / 2;
    int center_x = COLS / 2;
    
    clear();
    
    attron(COLOR_PAIR(COLOR_GAME_OVER) | A_BOLD);
    mvprintw(center_y - 3, center_x - 12, "                     ");
    mvprintw(center_y - 2, center_x - 12, "                     ");
    mvprintw(center_y - 1, center_x - 12, "    GAME  OVER !     ");
    mvprintw(center_y,     center_x - 12, "                     ");
    mvprintw(center_y + 1, center_x - 12, "                     ");
    attroff(COLOR_PAIR(COLOR_GAME_OVER) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_MENU));
    mvprintw(center_y + 3, center_x - 12, "Score final: %d", game->score);
    mvprintw(center_y + 5, center_x - 18, "Appuyez sur R pour recommencer");
    mvprintw(center_y + 6, center_x - 15, "ou Q pour quitter");
    attroff(COLOR_PAIR(COLOR_MENU));
}

/**
 * @brief Affiche l'écran de victoire
 */
static void draw_menu_gagne(GameState *game) {
    int center_y = LINES / 2;
    int center_x = COLS / 2;
    
    clear();
    
    attron(COLOR_PAIR(COLOR_WIN) | A_BOLD);
    mvprintw(center_y - 3, center_x - 12, "                    ");
    mvprintw(center_y - 2, center_x - 12, "                     ");
    mvprintw(center_y - 1, center_x - 12, "    VICTOIRE !!!     ");
    mvprintw(center_y,     center_x - 12, "                     ");
    mvprintw(center_y + 1, center_x - 12, "                     ");
    attroff(COLOR_PAIR(COLOR_WIN) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_MENU));
    mvprintw(center_y + 3, center_x - 12, "Score final: %d", game->score);
    mvprintw(center_y + 5, center_x - 18, "Appuyez sur R pour continuer");
    mvprintw(center_y + 6, center_x - 15, "ou Q pour quitter");
    attroff(COLOR_PAIR(COLOR_MENU));
}

/**
 * @brief Affiche l'écran de pause
 */
static void draw_pause(void) {
    int center_y = LINES / 2;
    int center_x = COLS / 2;
    
    attron(COLOR_PAIR(COLOR_PAUSE) | A_BOLD);
    mvprintw(center_y - 1, center_x - 8, "             ");
    mvprintw(center_y,     center_x - 8, "    PAUSE    ");
    mvprintw(center_y + 1, center_x - 8, "             ");
    attroff(COLOR_PAIR(COLOR_PAUSE) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_MENU));
    mvprintw(center_y + 3, center_x - 12, "Appuyez sur P pour continuer");
    attroff(COLOR_PAIR(COLOR_MENU));
}

/* ============================================================================
 * FONCTIONS PUBLIQUES
 * ============================================================================ */

/**
 * @brief Initialise la bibliothèque ncurses et les couleurs.
 *
 * Configure le terminal (mode cbreak, pas d'echo, touches spéciales,
 * taille minimale) et initialise les paires de couleurs utilisées.
 */
void init_ncurses(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    
    // Vérifier taille minimale
    if (LINES < 24 || COLS < 80) {
        endwin();
        fprintf(stderr, "Terminal trop petit! Minimum: 80x24, actuel: %dx%d\n", 
                COLS, LINES);
        exit(1);
    }
    
    // Initialiser les couleurs
    if (has_colors()) {
        start_color();
        init_pair(COLOR_PLAYER, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_ENEMY, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_BULLET, COLOR_CYAN, COLOR_BLACK);
        init_pair(COLOR_ENEMY_BULLET, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_SHIELD, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(COLOR_HUD, COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_BORDER, COLOR_BLUE, COLOR_BLACK);
        init_pair(COLOR_TITLE, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_MENU, COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_GAME_OVER, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_WIN, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_PAUSE, COLOR_YELLOW, COLOR_BLACK);
    }
}

/**
 * @brief Restaure l'état du terminal et quitte ncurses.
 */
void cleanup_ncurses(void) {
    endwin();
}

/**
 * @brief Effectue le rendu d'une frame en mode ncurses selon la vue.
 *
 * Affiche les menus, le HUD et les entités selon `game->currView`.
 *
 * @param game Pointeur vers l'état du jeu.
 */
void render_ncurses(GameState *game) {
    erase();
    
    switch (game->currView) {
        case ACCUEIL:
            draw_menu_accueil();
            break;
            
        case JEU:
            draw_hud(game);
            draw_border(game->height, game->width);
            draw_shields(game);
            draw_enemies(game);
            draw_enemy_bullets(game);
            draw_bullets(game);
            draw_player(game);
            break;
            
        case MENU_JEU:
            draw_hud(game);
            draw_border(game->height, game->width);
            draw_shields(game);
            draw_enemies(game);
            draw_enemy_bullets(game);
            draw_bullets(game);
            draw_player(game);
            draw_pause();
            break;
            
        case MENU_PERD:
            draw_menu_perd(game);
            break;
            
        case MENU_GAGNE:
            draw_menu_gagne(game);
            break;
            
        default:
            break;
    }
    usleep(16666);
    
}
