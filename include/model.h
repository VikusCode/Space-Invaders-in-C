/**
 * @file model.h
 * @brief Définition du modèle de données et prototypes des fonctions.
 *
 * Contient les constantes, types et la structure principale `GameState`.
 */

#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

/** @name Constantes de configuration
 *  Constantes de taille et limites utilisées par le jeu.
 *  @{ */
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
/** @} */

/** Vitesse de déplacement initiale des ennemis (modifiable par vague) */
static float vitesse = 25.0;
/** Numéro de la vague actuelle */
static int nbVagues = 1;
static int prec_vague;

/**
 * @brief Représente un projectile (joueur ou ennemi).
 */
typedef struct {
    int x; /**< Position x */
    int y; /**< Position y */
    int active; /**< 1 si actif, 0 sinon */
} Bullet;

/**
 * @brief Types d'ennemis (valeurs déterminent l'apparence/score).
 */
typedef enum {
    SQUID,
    CRABS,
    OCTOPUS
} EnnemyType;

/**
 * @brief Brique de bouclier (partie d'un bouclier destructible).
 */
typedef struct {
    int x; /**< Position x */
    int y; /**< Position y */
    int active; /**< 1 = présente, 0 = détruite */
} ShieldBrick;

/**
 * @brief Représente un ennemi individuel.
 */
typedef struct {
    int x; /**< Position x */
    int y; /**< Position y */
    int alive; /**< 1 si vivant, 0 si mort */
    EnnemyType type; /**< Type de l'ennemi */
} Enemy;

/**
 * @brief Structure décrivant le joueur.
 */
typedef struct {
    int x; /**< Position x */
    int y; /**< Position y */
} Player;

/**
 * @brief États possibles de l'interface du jeu.
 */
typedef enum {
    ACCUEIL,
    INSTRUCTION,
    JEU,
    MENU_JEU,
    MENU_GAGNE,
    MENU_PERD
} VueState;

/**
 * @brief Structure de l'état global du jeu.
 *
 * Contient entités, projectiles, ressources audio, et variables de
 * contrôle (vues, vies, score, etc.).
 */
typedef struct {
    Player player;
    Bullet bullets[MAX_BULLETS];
    Bullet enemy_bullets[MAX_ENEMY_BULLETS];
    Enemy enemies[ENEMY_ROWS * ENEMY_COLS];
    ShieldBrick shields[MAX_SHIELD_BRICKS];

    int score;
    int game_over; /**< 1 = perdu, 0 = en cours */
    int nb_lives; /**< Vies restantes */
    int enemy_direction;
    int enemy_move_counter;

    bool isSDL;
    bool isNC; /**< Indique si on utilise ncurses */

    VueState currView;

    int canShoot; /**< Contrôle du tir du joueur */

    MIX_Mixer *mixerDevice;

    /* Musiques et bruitages */
    MIX_Audio *audio_music;
    MIX_Track *track_music;
    MIX_Audio *audio_shoot;
    MIX_Track *track_shoot;
    MIX_Audio *audio_explosion;
    MIX_Track *track_explosion;

    int width; /**< Largeur logique */
    int height; /**< Hauteur logique */
} GameState;

/* --- Prototypes publiques du modèle --- */
/**
 * @brief Initialise l'état du jeu.
 */
void init_model(GameState *game, int width, int height, int score);

/** @brief Met à jour les projectiles en jeu. */
void update_bullets(GameState *game);

/** @brief Met à jour les ennemis (mouvement, état). */
void update_enemies(GameState *game);

/** @brief Gère le tir automatique des ennemis. */
void enemy_shoot(GameState *game);

/** @brief Vérifie et applique les collisions. */
void check_collisions(GameState *game);

/** @brief Tente de tirer depuis le joueur. */
void player_shoot(GameState *game);

/** @brief Déplace le joueur horizontalement. */
void player_move(GameState *game, int direction);

#endif /* MODEL_H */