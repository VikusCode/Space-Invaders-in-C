#include <stdlib.h>
#include <stdio.h>
#include "../include/model.h"

void init_model(GameState *game, int width, int height) {
    game->width = width;
    game->height = height;
    
    // --- INIT JOUEUR ---
    game->player.x = width / 2;
    game->player.y = height - 2;
    game->nb_lives = 3;
    game->score = 0;
    game->currView = ACCUEIL; // Ou JEU pour tester direct

    // --- INIT BALLES ---
    for (int i = 0; i < MAX_BULLETS; i++) game->bullets[i].active = 0;
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) game->enemy_bullets[i].active = 0;

    // --- INIT ENNEMIS (DYNAMIQUE) ---
    
    // 1. Calcul des marges (10% de chaque côté)
    int margin = width / 10; 
    
    // 2. Largeur disponible pour les ennemis (80% du centre)
    int usable_width = width - (2 * margin);
    
    int dynamic_spacing = usable_width / (ENEMY_COLS);
    
    if (dynamic_spacing < 2) dynamic_spacing = 2;

    int total_enemy_block_width = (ENEMY_COLS - 1) * dynamic_spacing;
    int start_x = (width - total_enemy_block_width) / 2;
    
    // 5. Hauteur de départ (comme vu précédemment, ex: 15% du haut)
    int start_y = height / 6; 

    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            int idx = row * ENEMY_COLS + col;
            
            
            // On utilise le start_x calculé + l'espacement dynamique
            game->enemies[idx].x = start_x + (col * dynamic_spacing);
            
            // On utilise le start_y (height/6) + l'espacement vertical (2)
            game->enemies[idx].y = start_y + (row * 2); 
            
            
            game->enemies[idx].alive = 1;

            // 3. Assignation des types
            if (row == 0) {
                game->enemies[idx].type = SQUID;
            } else if (row == 1 || row == 2) {
                game->enemies[idx].type = CRABS;
            } else {
                game->enemies[idx].type = OCTOPUS;
            }
        }
    }

    int shape[4][5] = {    // J'ai mis 4 et 5 en dur pour être sûr que ça marche
        {0, 1, 1, 1, 0}, 
        {1, 1, 1, 1, 1}, 
        {1, 1, 1, 1, 1}, 
        {1, 1, 0, 1, 1}
    };

    // 2. Init à 0
    for(int i=0; i<MAX_SHIELD_BRICKS; i++) game->shields[i].active = 0;

    // 3. Calculs
    float spacing = width / 5.0f; 
    
    // CORRECTION ICI : On change le nom de la variable pour éviter le conflit
    float shield_start_y = height - 7.0f; 

    int brick_idx = 0;

    for (int s = 0; s < MAX_SHIELDS; s++) {
        float shield_start_x = (s + 1) * spacing - (SHIELD_W / 2.0f);

        for (int r = 0; r < SHIELD_H; r++) {
            for (int c = 0; c < SHIELD_W; c++) {
                
                // Attention : On vérifie qu'on ne dépasse pas le tableau shape
                // Si SHIELD_H vaut 4 dans le .h, c'est bon.
                if (shape[r][c] == 1) {
                    if (brick_idx < MAX_SHIELD_BRICKS) {
                        game->shields[brick_idx].x = shield_start_x + c;
                        
                        // CORRECTION ICI : On utilise la nouvelle variable
                        game->shields[brick_idx].y = shield_start_y + r; 
                        
                        game->shields[brick_idx].active = 1;
                        brick_idx++;
                    }
                }
            }
        }
    }
    
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
    
    if (count > 0 && rand() % 45 == 0) { // au plus tu augmentes le chiffre apres le % au plus il tire lentement 
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
    if (game->enemy_move_counter < 25) return; // au plus tu augmentes au plus ils sont lents les ennemis
    
    game->enemy_move_counter = 0;
    int should_descend = 0;
    
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (game->enemies[i].alive) {
            
            // A. COLLISION AVEC LE BOUCLIER (Le bouclier meurt, pas le joueur)
            for (int s = 0; s < MAX_SHIELD_BRICKS; s++) {
                if (game->shields[s].active) {
                    // Si l'ennemi marche sur la brique de bouclier
                    if (game->enemies[i].x == game->shields[s].x && 
                        game->enemies[i].y == game->shields[s].y) {
                        
                        game->shields[s].active = 0; // Le bouclier est détruit !
                    }
                }
            }

            // B. COLLISION AVEC LE JOUEUR (GAME OVER)
            // C'est seulement si l'ennemi atteint la hauteur du vaisseau
            if (game->enemies[i].y >= game->player.y) {
                game->game_over = 1;
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
    // --- 1. BALLES JOUEUR vs BOUCLIERS ---
    // (Si on tire sur nos propres boucliers, on les casse !)
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) {
            for (int s = 0; s < MAX_SHIELD_BRICKS; s++) {
                if (game->shields[s].active) {
                    // Collision simple point vs point (ou rectangle vs point)
                    // On ajoute une petite marge (>= et <=) pour être sûr de toucher
                    if (game->bullets[i].x >= game->shields[s].x && 
                        game->bullets[i].x <  game->shields[s].x + 1 &&
                        game->bullets[i].y >= game->shields[s].y && 
                        game->bullets[i].y <  game->shields[s].y + 1) {
                        
                        game->shields[s].active = 0; // Détruit la brique
                        game->bullets[i].active = 0; // Détruit la balle
                        break; // Une balle ne casse qu'une brique à la fois
                    }
                }
            }
        }
    }

    // --- 2. BALLES ENNEMIES vs BOUCLIERS ---
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (game->enemy_bullets[i].active) {
            for (int s = 0; s < MAX_SHIELD_BRICKS; s++) {
                if (game->shields[s].active) {
                    // Même logique
                    if (game->enemy_bullets[i].x >= game->shields[s].x && 
                        game->enemy_bullets[i].x <  game->shields[s].x + 1 &&
                        game->enemy_bullets[i].y >= game->shields[s].y && 
                        game->enemy_bullets[i].y <  game->shields[s].y + 1) {
                        
                        game->shields[s].active = 0; // Détruit la brique
                        game->enemy_bullets[i].active = 0; // Détruit la balle
                        break;
                    }
                }
            }
        }
    }
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
                    if (game->nb_lives > 0) game->nb_lives--; // baisser la vie de notre vaisseau
                    if (game->nb_lives == 0) game->game_over=1; // perdre si on a plus de vie
            }
        }
    }
}