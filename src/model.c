#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/ncurses/model.h"

#define HEIGHT getmaxy(stdscr)
#define WIDTH getmaxx(stdscr)

int** plateau = NULL;

void init_plateau(){
    initscr();
    plateau = (int **)malloc(HEIGHT * sizeof(int *));
    for(int i = 0; i < HEIGHT; i++){
        plateau[i] = (int *)malloc(WIDTH * sizeof(int));
    }
    plateau[HEIGHT/2][WIDTH/2] = ENNEMI;
    /*for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            printf("%d", plateau[i][j]);
        }
        printf("\n");
    }*/
}

int main(){
    init_plateau();
    endwin();
    return 0;
}