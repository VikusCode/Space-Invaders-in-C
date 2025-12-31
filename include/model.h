#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

// --- Constantes ---
#define MAX_BULLETS 10
#define MAX_ENEMY_BULLETS 20
#define ENEMY_ROWS 4
#define ENEMY_COLS 10
#define ENEMY_SPACING 2
#define MAX_SHIELDS 4
#define SHIELD_W 5
#define SHIELD_H 4
#define MAX_SHIELD_BRICKS (MAX_SHIELDS * SHIELD_W * SHIELD_H)

// --- Structures ---
typedef struct {
    int x, y;
    int active;
} Bullet;

typedef enum {
    SQUID,
    CRABS,
    OCTOPUS
} EnnemyType;

typedef struct {
    int x, y;
    int active; // 1 = brique présente, 0 = détruite
} ShieldBrick;

typedef struct {
    int x, y;
    int alive;
    EnnemyType type;
} Enemy;

typedef struct {
    int x, y;
} Player;

typedef enum {
    ACCUEIL, 
    JEU, 
    MENU_JEU
} VueState;

typedef struct {
    Player player;
    Bullet bullets[MAX_BULLETS];
    Bullet enemy_bullets[MAX_ENEMY_BULLETS];
    Enemy enemies[ENEMY_ROWS * ENEMY_COLS];
    ShieldBrick shields[MAX_SHIELD_BRICKS];
    
    int score;
    int game_over; // 1 perdu, 0 en cours
    int nb_lives; //nombre de vie restant
    int enemy_direction;
    int enemy_move_counter;

    bool isSDL;
    bool isNC; //savoir si c'est sdl ou NC ca

    bool pause; //si true alors = pause ca sert pour le afficher le menu dans le in game

    VueState currView;
    
    int width;
    int height;
} GameState;

void init_model(GameState *game, int width, int height);
void update_bullets(GameState *game);
void update_enemies(GameState *game);
void enemy_shoot(GameState *game);
void check_collisions(GameState *game);
void player_shoot(GameState *game);
void player_move(GameState *game, int direction);

#endif