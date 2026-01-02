#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <ncurses.h>

#include "../include/controller.h"
#include "../include/view_sdl.h"
#include "../include/model.h"

SDL_Event event;
void handle_event(GameState *game, int *running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            float mouse_x = event.button.x;
            float mouse_y = event.button.y;
            convert_mouse_coordinates(&mouse_x, &mouse_y);
            if (game->currView == ACCUEIL) {
                // --- MÊMES CALCULS QUE LA VUE ---
                float btn_w = 26.0f;
                float btn_h = 6.0f;
                float gap = 2.0f;
                float total_h = (btn_h * 3) + (gap * 2);
                float start_y = (game->height - total_h) / 2;
                float btn_x = (game->width - btn_w) / 2;

                float play_y = start_y;
                float instr_y = start_y + btn_h + gap;
                float quit_y = start_y + (btn_h + gap) * 2;

                // A. CLIC SUR JOUER
                if (mouse_x >= btn_x && mouse_x <= (btn_x + btn_w) &&
                    mouse_y >= play_y && mouse_y <= (play_y + btn_h)) {
                    init_model(game, game->width, game->height, score_init);
                    game->currView = JEU;
                    game->isSDL = 1;
                }
                
                // B. CLIC SUR INSTRUCTIONS 
                else if (mouse_x >= btn_x && mouse_x <= (btn_x + btn_w) &&
                        mouse_y >= instr_y && mouse_y <= (instr_y + btn_h)) {
                    
                    game->currView = INSTRUCTION; 
                }
                
                // C. CLIC SUR QUITTER
                else if (mouse_x >= btn_x && mouse_x <= (btn_x + btn_w) &&
                        mouse_y >= quit_y && mouse_y <= (quit_y + btn_h)) {
                    
                    *running = 0; 
                }
            } else if (game->currView == MENU_JEU || game->currView == MENU_PERD) {
                float btn_w = 28.0f;
                float btn_h = 6.0f;
                float gap = 2.0f;
                float total_h = (btn_h * 2) + gap;
                float start_y = (game->height - total_h) / 2;
                float btn_x = (game->width - btn_w) / 2;

                float restart_y = start_y;
                float quit_y = start_y + btn_h + gap;

                // CLIC SUR RECOMMENCER
                if (mouse_x >= btn_x && mouse_x <= (btn_x + btn_w) &&
                    mouse_y >= restart_y && mouse_y <= (restart_y + btn_h)) {
                    
                    init_model(game, game->width, game->height, score_init);
                    
                    game->currView = JEU; 
                    game->isSDL = 1;      
                }
                
                // CLIC SUR QUITTER
                else if (mouse_x >= btn_x && mouse_x <= (btn_x + btn_w) &&
                         mouse_y >= quit_y && mouse_y <= (quit_y + btn_h)) {
                    game->currView = ACCUEIL;
                    draw_menu_view(game); // Au lieu de quitter le jeu ca retourne au menu principal
                }
            }
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
            switch (game->currView)
            {
                case ACCUEIL:
                    switch (event.key.key) {
                        case SDLK_RETURN:
                            game->currView = JEU;
                            draw_game_view(game);
                            break;
                        case SDLK_RETURN2:
                            game->currView = JEU;
                            draw_game_view(game);
                            break;
                        case SDLK_I:    
                            game->currView = INSTRUCTION;
                            break;
                        default:
                            break;
                    }
                    break;
                case INSTRUCTION:
                    switch (event.key.key) {
                        case SDLK_RETURN:
                            game->currView = ACCUEIL;
                            break;
                        case SDLK_RETURN2:
                            game->currView = ACCUEIL;
                            break;
                        case SDLK_ESCAPE:
                            game->currView = ACCUEIL;
                            break;
                        default:
                            break;
                    }
                    break;
                case JEU:
                    switch (event.key.key) {
                        case SDLK_A: // a enlever apres
                            game->nb_lives = 0;
                            game->currView = MENU_PERD;
                            break;
                        case SDLK_ESCAPE: 
                            game->currView = MENU_JEU;
                            break;
                        case SDLK_Q:
                            player_move(game, -1);
                            break;
                        case SDLK_D:
                            player_move(game, 1);
                            break;
                        case SDLK_SPACE:
                            player_shoot(game);
                        default:
                            break;
                    }
                    break;
                case MENU_GAGNE:
                    switch (event.key.key) {
                        case SDLK_RETURN:
                            init_model(game, game->width, game->height, game->score);
                            game->currView = JEU;
                            game->isSDL = 1; 
                            break;
                        case SDLK_RETURN2:
                            init_model(game, game->width, game->height, game->score);
                            game->currView = JEU;
                            game->isSDL = 1; 
                            break;
                        case SDLK_ESCAPE:
                            game->currView = ACCUEIL;
                            break;
                        default:
                            break;
                    }
                    break;
                case MENU_PERD:
                    switch (event.key.key) { 
                        case SDLK_ESCAPE:
                            game->currView = ACCUEIL;
                            break;
                        case SDLK_RETURN:
                            init_model(game, game->width, game->height, score_init);
                            game->currView = JEU;
                            game->isSDL = 1; 

                            if (game->track_music) {
                                MIX_ResumeTrack(game->track_music);
                            }
                            break;
                        case SDLK_RETURN2:
                            init_model(game, game->width, game->height, score_init);
                            game->currView = JEU;
                            game->isSDL = 1; 
                            break;
                        default:
                            break;
                    }
                    break;
                case MENU_JEU:
                    switch (event.key.key) {
                        case SDLK_ESCAPE:
                            game->currView = JEU;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    } 
}

int handle_input_ncurses(GameState *game) {
    int ch = getch();
    
    if (ch == ERR) {
        return 1; // Pas d'entrée, on continue la boucle
    }
    
    // Commandes globales
    if (ch == 'q' || ch == 'Q') {
        return 0; // Quitter le jeu
    }
    
    switch (game->currView) {
        case ACCUEIL:
            if (ch == '\n' || ch == KEY_ENTER) {
                init_model(game, game->width, game->height, 0); // Reset pour être sûr
                game->currView = JEU;
            }
            break;
            
        case JEU:
            if (ch == KEY_LEFT) {
                player_move(game, -1);
            } else if (ch == KEY_RIGHT) {
                player_move(game, 1);
            } else if (ch == ' ') {
                player_shoot(game);
            } else if (ch == 'p' || ch == 'P') {
                game->currView = MENU_JEU;
            }
            break;
            
        case MENU_JEU:
            if (ch == 'p' || ch == 'P') {
                game->currView = JEU;
            }
            break;
            
        case MENU_PERD:
        case MENU_GAGNE:
            if (ch == 'r' || ch == 'R') {
                init_model(game, game->width, game->height, 0);
                game->currView = JEU;
            }
            break;
        default:
            break;
    }
    
    return 1; // On continue la boucle
}
