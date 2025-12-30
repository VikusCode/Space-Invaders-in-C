#ifndef MODEL_H
#define MODEL_H

// --- Constantes ---
#define MAX_BULLETS 10
#define MAX_ENEMY_BULLETS 20
#define ENEMY_ROWS 4
#define ENEMY_COLS 10
#define ENEMY_SPACING 2

// --- Structures ---
typedef struct {
    int x, y;
    int active;
} Bullet;

typedef struct {
    int x, y;
    int alive;
} Enemy;

typedef struct {
    int x, y;
} Player;

typedef enum {
    ACCUEIL, 
    JEU
} VueState;

typedef struct {
    Player player;
    Bullet bullets[MAX_BULLETS];
    Bullet enemy_bullets[MAX_ENEMY_BULLETS];
    Enemy enemies[ENEMY_ROWS * ENEMY_COLS];
    
    int score;
    int game_over;
    int enemy_direction;
    int enemy_move_counter;

    VueState currView;
    
    int width;
    int height;
} GameState;

// --- Fonctions ---
void init_model(GameState *game, int width, int height);
void update_bullets(GameState *game);
void update_enemies(GameState *game);
void enemy_shoot(GameState *game);
void check_collisions(GameState *game);
void player_shoot(GameState *game);
void player_move(GameState *game, int direction);

#endif