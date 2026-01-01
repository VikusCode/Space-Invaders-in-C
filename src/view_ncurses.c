#include <ncurses.h>
#include "../include/view_ncurses.h"
#include "../include/model.h" // Nécessaire pour les types d'ennemis et currView

// Définition des Paires de couleurs
#define COL_PLAYER  1
#define COL_ENEMY   2
#define COL_BULLET  3
#define COL_SHIELD  4
#define COL_TEXT    5

void init_ncurses_view() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Ne bloque pas le programme
    curs_set(0);           // Cache le curseur clignotant

    // Initialisation des couleurs si le terminal le supporte
    if (has_colors()) {
        start_color();
        init_pair(COL_PLAYER, COLOR_GREEN, COLOR_BLACK);
        init_pair(COL_ENEMY, COLOR_RED, COLOR_BLACK);
        init_pair(COL_BULLET, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COL_SHIELD, COLOR_GREEN, COLOR_BLACK);
        init_pair(COL_TEXT, COLOR_CYAN, COLOR_BLACK);
    }
}

void close_ncurses_view() {
    endwin();
}

int get_ncurses_input() {
    return getch();
}

// --- ECRAN D'ACCUEIL ---
void draw_menu_ncurses(GameState *game) {
    clear();
    
    int cx = game->width / 2;
    int cy = game->height / 2;

    attron(COLOR_PAIR(COL_TEXT));
    mvprintw(cy - 4, cx - 7, "SPACE INVADERS");
    
    // Simuler le bouton "JOUER"
    mvprintw(cy, cx - 10, "[ ENTREE ]  JOUER");
    
    // Simuler le bouton "QUITTER"
    mvprintw(cy + 2, cx - 10, "[ ECHAP  ]  QUITTER");
    attroff(COLOR_PAIR(COL_TEXT));

    refresh();
}

// --- ECRAN MENU PAUSE ---
void draw_pause_ncurses(GameState *game) {
    clear();
    
    int cx = game->width / 2;
    int cy = game->height / 2;

    attron(COLOR_PAIR(COL_TEXT));
    mvprintw(cy - 4, cx - 3, "PAUSE");
    
    mvprintw(cy, cx - 12, "[ ENTREE ]  RECOMMENCER");
    mvprintw(cy + 2, cx - 12, "[ ECHAP  ]  REPRENDRE"); // Ou quitter selon ta logique
    mvprintw(cy + 4, cx - 12, "[ A      ]  QUITTER");    // Si tu veux une autre touche pour quitter
    attroff(COLOR_PAIR(COL_TEXT));

    refresh();
}

// --- ECRAN DE JEU ---
void draw_game_ncurses(GameState *game) {
    clear();
    
    // 1. HUD (Score et Vies)
    attron(COLOR_PAIR(COL_TEXT));
    mvprintw(0, 1, "SCORE: %d", game->score);
    // Vérifie si ta variable s'appelle 'lives' ou 'nb_lives' dans model.h
    mvprintw(0, game->width - 12, "LIVES: %d", game->nb_lives); 
    
    // Ligne de séparation
    for(int x=0; x<game->width; x++) mvaddch(1, x, '_');
    attroff(COLOR_PAIR(COL_TEXT));

    if(game->game_over) {
        mvprintw(game->height/2, game->width/2 - 5, "GAME OVER");
        refresh();
        return;
    }

    // 2. Joueur (Vert)
    attron(COLOR_PAIR(COL_PLAYER));
    // On dessine un petit vaisseau '^'
    mvaddch(game->player.y, game->player.x, '^');
    attroff(COLOR_PAIR(COL_PLAYER));

    // 3. Ennemis (Rouge)
    attron(COLOR_PAIR(COL_ENEMY));
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (game->enemies[i].alive) {
            char sprite = 'W'; // Par défaut
            
            // On change le caractère selon le type
            switch(game->enemies[i].type) {
                case SQUID:   sprite = 'o'; break; // Petit
                case CRABS:    sprite = 'x'; break; // Moyen
                case OCTOPUS: sprite = 'M'; break; // Gros
            }
            mvaddch(game->enemies[i].y, game->enemies[i].x, sprite);
        }
    }
    attroff(COLOR_PAIR(COL_ENEMY));

    // 4. Boucliers (Vert)
    attron(COLOR_PAIR(COL_SHIELD));
    for (int i = 0; i < MAX_SHIELD_BRICKS; i++) {
        if (game->shields[i].active) {
            // '#' représente une brique de bouclier
            mvaddch((int)game->shields[i].y, (int)game->shields[i].x, '#');
        }
    }
    attroff(COLOR_PAIR(COL_SHIELD));

    // 5. Balles Joueur (Jaune)
    attron(COLOR_PAIR(COL_BULLET));
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) 
            mvaddch(game->bullets[i].y, game->bullets[i].x, '|');
    }
    attroff(COLOR_PAIR(COL_BULLET));
    
    // 6. Balles Ennemis (Blanc ou Rouge)
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (game->enemy_bullets[i].active) 
            mvaddch(game->enemy_bullets[i].y, game->enemy_bullets[i].x, '!');
    }

    refresh();
}

// Fonction Principale (Aiguilleur)
void draw_ncurses_view(GameState *game) {
    if (game->currView == ACCUEIL) {
        draw_menu_ncurses(game);
    } 
    else if (game->currView == MENU_JEU) {
        draw_pause_ncurses(game);
    }
    else {
        draw_game_ncurses(game);
    }
}