// ici a mettre quand on clique sur le bouton jouer game->currView = JEU c'est la ou je vais mettre les cliques sur les boutons depuis la page d'accueil

#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

#include "../include/controller.h"
#include "../include/view_sdl.h"
#include "../include/model.h"

SDL_Event event;
bool pause = false;
void handle_event(GameState *game, int *running) {
    while (SDL_PollEvent(&event)) {
        if (game->isSDL) {
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                float mouse_x = event.button.x;
                float mouse_y = event.button.y;
                convert_mouse_coordinates(&mouse_x, &mouse_y);
                if (game->currView == ACCUEIL) {
                    // --- MÊMES CALCULS QUE LA VUE ---
                    float btn_w = 24.0f;
                    float btn_h = 6.0f;
                    float gap = 2.0f;
                    float total_h = (btn_h * 2) + gap;
                    float start_y = (game->height - total_h) / 2;
                    float btn_x = (game->width - btn_w) / 2;

                    float play_y = start_y;
                    float quit_y = start_y + btn_h + gap;

                    // 1. Test Clic sur JOUER
                    if (mouse_x >= btn_x && mouse_x <= (btn_x + btn_w) &&
                        mouse_y >= play_y && mouse_y <= (play_y + btn_h)) {
                        
                        game->currView = JEU;
                    }
                    
                    // 2. Test Clic sur QUITTER
                    else if (mouse_x >= btn_x && mouse_x <= (btn_x + btn_w) &&
                             mouse_y >= quit_y && mouse_y <= (quit_y + btn_h)) {
                        
                        *running = 0; 
                    }
                } else if (game->currView == MENU_JEU) {
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
                        
                        init_model(game, game->width, game->height);
                        
                        game->currView = JEU; 
                        game->isSDL = 1;      
                    }
                    
                    // CLIC SUR QUITTER
                    else if (mouse_x >= btn_x && mouse_x <= (btn_x + btn_w) &&
                             mouse_y >= quit_y && mouse_y <= (quit_y + btn_h)) {
                        *running = 0; 
                        // draw_menu_view(game); // il faut j'arrive a fiare ene sorte que le boutton quitter du menu pause amene vers la page d'raccueil 
                    }
                }
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (game->currView == ACCUEIL) {
                    switch (event.key.key) {
                        case SDLK_RETURN:
                            game->currView = JEU;
                            draw_game_view(game);
                            break;
                        default:
                            break;
                    }
                } else if (game->currView == JEU) {
                    switch (event.key.key) {
                        case SDLK_ESCAPE: 
                            game->currView = MENU_JEU;
                            break;
                        case SDLK_X: 
                            close_sdl_view();
                            *running=0;
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
                } else if (game->currView == MENU_JEU) {
                    switch (event.key.key)
                    {
                    case SDLK_ESCAPE:
                        game->currView = JEU;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}