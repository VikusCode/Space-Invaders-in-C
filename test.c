#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PLAYER_CHAR 'A'
#define ENEMY_CHAR 'W'
#define BULLET_CHAR '|'
#define ENEMY_BULLET_CHAR '!'
#define MAX_BULLETS 10
#define MAX_ENEMY_BULLETS 20
#define ENEMY_ROWS 4
#define ENEMY_COLS 10
#define ENEMY_SPACING 2

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

int score = 0;
int game_over = 0;
int max_y, max_x;
int enemy_direction = 1;
int enemy_move_counter = 0;

Player player;
Bullet bullets[MAX_BULLETS];
Bullet enemy_bullets[MAX_ENEMY_BULLETS];
Enemy enemies[ENEMY_ROWS * ENEMY_COLS];

void init_game() {
    getmaxyx(stdscr, max_y, max_x);
    
    player.x = max_x / 2;
    player.y = max_y - 3;
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemy_bullets[i].active = 0;
    }
    
    int start_x = (max_x - (ENEMY_COLS * ENEMY_SPACING)) / 2;
    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            int idx = row * ENEMY_COLS + col;
            enemies[idx].x = start_x + col * ENEMY_SPACING;
            enemies[idx].y = 3 + row * 2;
            enemies[idx].alive = 1;
        }
    }
    
    score = 0;
    game_over = 0;
    enemy_direction = 1;
    enemy_move_counter = 0;
}

void shoot_bullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = player.x;
            bullets[i].y = player.y - 1;
            bullets[i].active = 1;
            break;
        }
    }
}

void enemy_shoot() {
    int alive_enemies[ENEMY_ROWS * ENEMY_COLS];
    int count = 0;
    
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (enemies[i].alive) {
            alive_enemies[count++] = i;
        }
    }
    
    if (count > 0 && rand() % 30 == 0) {
        int shooter = alive_enemies[rand() % count];
        for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
            if (!enemy_bullets[i].active) {
                enemy_bullets[i].x = enemies[shooter].x;
                enemy_bullets[i].y = enemies[shooter].y + 1;
                enemy_bullets[i].active = 1;
                break;
            }
        }
    }
}

void update_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].y--;
            if (bullets[i].y < 0) {
                bullets[i].active = 0;
            }
        }
    }
    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemy_bullets[i].active) {
            enemy_bullets[i].y++;
            if (enemy_bullets[i].y >= max_y) {
                enemy_bullets[i].active = 0;
            }
        }
    }
}

void update_enemies() {
    enemy_move_counter++;
    if (enemy_move_counter < 10) return;
    
    enemy_move_counter = 0;
    
    int should_move_down = 0;
    
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (enemies[i].alive) {
            if ((enemy_direction > 0 && enemies[i].x >= max_x - 2) ||
                (enemy_direction < 0 && enemies[i].x <= 1)) {
                should_move_down = 1;
                break;
            }
        }
    }
    
    if (should_move_down) {
        enemy_direction *= -1;
        for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
            if (enemies[i].alive) {
                enemies[i].y++;
                if (enemies[i].y >= player.y) {
                    game_over = 1;
                }
            }
        }
    } else {
        for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
            if (enemies[i].alive) {
                enemies[i].x += enemy_direction;
            }
        }
    }
}

void check_collisions() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            for (int j = 0; j < ENEMY_ROWS * ENEMY_COLS; j++) {
                if (enemies[j].alive && 
                    bullets[i].x == enemies[j].x && 
                    bullets[i].y == enemies[j].y) {
                    enemies[j].alive = 0;
                    bullets[i].active = 0;
                    score += 10;
                }
            }
        }
    }
    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemy_bullets[i].active) {
            if (enemy_bullets[i].x == player.x && 
                enemy_bullets[i].y == player.y) {
                game_over = 1;
            }
        }
    }
    
    int all_dead = 1;
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (enemies[i].alive) {
            all_dead = 0;
            break;
        }
    }
    if (all_dead) {
        init_game();
        score += 100;
    }
}

void draw() {
    clear();
    
    mvprintw(0, 2, "Score: %d", score);
    mvprintw(0, max_x - 20, "Q: Quitter");
    
    mvaddch(player.y, player.x, PLAYER_CHAR);
    
    for (int i = 0; i < ENEMY_ROWS * ENEMY_COLS; i++) {
        if (enemies[i].alive) {
            mvaddch(enemies[i].y, enemies[i].x, ENEMY_CHAR);
        }
    }
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            mvaddch(bullets[i].y, bullets[i].x, BULLET_CHAR);
        }
    }
    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemy_bullets[i].active) {
            mvaddch(enemy_bullets[i].y, enemy_bullets[i].x, ENEMY_BULLET_CHAR);
        }
    }
    
    if (game_over) {
        mvprintw(max_y / 2, max_x / 2 - 10, "GAME OVER!");
        mvprintw(max_y / 2 + 1, max_x / 2 - 15, "Score final: %d", score);
        mvprintw(max_y / 2 + 2, max_x / 2 - 15, "Appuyez sur R pour rejouer");
    }
    
    refresh();
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    
    srand(time(NULL));
    
    init_game();
    
    int ch;
    while (1) {
        ch = getch();
        
        if (ch == 'q' || ch == 'Q') {
            break;
        }
        
        if (game_over) {
            if (ch == 'r' || ch == 'R') {
                init_game();
            }
        } else {
            if (ch == KEY_LEFT && player.x > 0) {
                player.x--;
            }
            if (ch == KEY_RIGHT && player.x < max_x - 1) {
                player.x++;
            }
            if (ch == ' ') {
                shoot_bullet();
            }
            
            update_bullets();
            update_enemies();
            enemy_shoot();
            check_collisions();
        }
        
        draw();
        usleep(50000);
    }
    
    endwin();
    return 0;
}