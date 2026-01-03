#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

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
#define score_init 0
static float vitesse = 25.0;
static int nbVagues = 1;
static int prec_vague;

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
    INSTRUCTION,
    JEU, 
    MENU_JEU,
    MENU_GAGNE,
    MENU_PERD
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

    VueState currView;
    
    // int nbVagues;
    int canShoot; //pour tirer une seule balle par une seule
    
    MIX_Mixer *mixerDevice;
    
    // Musique de fond
    MIX_Audio *audio_music;
    MIX_Track *track_music;

    // Bruitage Tir
    MIX_Audio *audio_shoot;
    MIX_Track *track_shoot;

    // Bruitage Explosion / Mort
    MIX_Audio *audio_explosion;
    MIX_Track *track_explosion;

    int width;
    int height;
} GameState;

void init_model(GameState *game, int width, int height, int score);
void update_bullets(GameState *game);
void update_enemies(GameState *game);
void enemy_shoot(GameState *game);
void check_collisions(GameState *game);
void player_shoot(GameState *game);
void player_move(GameState *game, int direction);


#endif