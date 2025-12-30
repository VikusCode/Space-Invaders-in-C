#include <stdlib.h>
#include "../include/model.h"

void init_model(GameState *game, int width, int height) {
    game->width = width;
    game->height = height;
    
    // Init Joueur (centré en bas)
    game->player.x = width / 2;
    game->player.y = height - 2;
    
    // Reset balles
    for (int i = 0; i < MAX_BULLETS; i++) game->bullets[i].active = 0;
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) game->enemy_bullets[i].active = 0;
    
    // Init Ennemis
    int start_x = (width - (ENEMY_COLS * ENEMY_SPACING)) / 2;
    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            int idx = row * ENEMY_COLS + col;
            game->enemies[idx].x = start_x + col * ENEMY_SPACING;
            game->enemies[idx].y = 2 + row * 2;
            game->enemies[idx].alive = 1;
        }
    }
    
    game->score = 0;
    game->game_over = 0;
    game->enemy_direction = 1;
    game->enemy_move_counter = 0;
}

void player_shoot(GameState *game) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!game->bullets[i].active) {
            game->bullets[i].x = game->player.x;
            game->bullets[i].y = game->player.y - 1;
            game->bullets[i].active = 1;
            break;
        }
    }
}

void player_move(GameState *game, int direction) {
    int new_x = game->player.x + direction;
    if (new_x >= 0 && new_x < game->width) {
        game->player.x = new_x;
    }
}

void update_bullets(GameState *game) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) {
            game->bullets[i].y--;
            if (game->bullets[i].y < 0) game->bullets[i].active = 0;
        }
    }
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (game->enemy_bullets[i].active) {
            game->enemy_bullets[i].y++;
            if (game->enemy_bullets[i].y >= game->height) game->enemy_bullets[i].active = 0;
        }
    }
}

void enemy_shoot(GameState *game) {
    int alive_indices[ENEMY_ROWS * ENEMY_COLS];
    int count = 0;
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (game->enemies[i].alive) alive_indices[count++] = i;
    }
    
    if (count > 0 && rand() % 30 == 0) {
        int shooter = alive_indices[rand() % count];
        for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
            if (!game->enemy_bullets[i].active) {
                game->enemy_bullets[i].x = game->enemies[shooter].x;
                game->enemy_bullets[i].y = game->enemies[shooter].y + 1;
                game->enemy_bullets[i].active = 1;
                break;
            }
        }
    }
}

void update_enemies(GameState *game) {
    game->enemy_move_counter++;
    if (game->enemy_move_counter < 10) return; // Vitesse ennemis
    
    game->enemy_move_counter = 0;
    int should_descend = 0;
    
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (game->enemies[i].alive) {
            if ((game->enemy_direction > 0 && game->enemies[i].x >= game->width - 1) ||
                (game->enemy_direction < 0 && game->enemies[i].x <= 0)) {
                should_descend = 1;
                break;
            }
        }
    }
    
    if (should_descend) {
        game->enemy_direction *= -1;
        for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
            if (game->enemies[i].alive) {
                game->enemies[i].y++;
                if (game->enemies[i].y >= game->player.y) game->game_over = 1;
            }
        }
    } else {
        for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
            if (game->enemies[i].alive) game->enemies[i].x += game->enemy_direction;
        }
    }
}

void check_collisions(GameState *game) {
    // Joueur tire sur Ennemi
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) {
            for (int j = 0; j < ENEMY_ROWS * ENEMY_COLS; j++) {
                if (game->enemies[j].alive && 
                    game->bullets[i].x == game->enemies[j].x && 
                    game->bullets[i].y == game->enemies[j].y) {
                    game->enemies[j].alive = 0;
                    game->bullets[i].active = 0;
                    game->score += 10;
                }
            }
        }
    }
    // Ennemi tire sur Joueur
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (game->enemy_bullets[i].active) {
            if (game->enemy_bullets[i].x == game->player.x && 
                game->enemy_bullets[i].y == game->player.y) {
                game->game_over = 1;
            }
        }
    }
}