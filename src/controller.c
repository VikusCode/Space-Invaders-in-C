// ici a mettre quand on clique sur le bouton jouer game->currView = JEU c'est la ou je vais mettre les cliques sur les boutons depuis la page d'accueil

#include <stdio.h>
#include <SDL3/SDL.h>

#include "../include/controller.h"
#include "../include/view_sdl.h"
#include "../include/model.h"

SDL_Event event;

void handle_event(GameState *game, int *running) {
    while (SDL_PollEvent(&event)) {
        if (game->isSDL) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (game->currView == ACCUEIL) {
                    switch (event.key.key) {
                        case SDLK_ESCAPE: // ca faut pas le laisser il y aura un bouton "quitter" sur la page d'accueil apres c'est juste pour le test
                            close_sdl_view();
                            *running = 0;
                            break;
                        case SDLK_RETURN:
                            game->currView = JEU;
                            draw_game_view(game);
                            break;
                        default:
                            break;
                    }
                } else if (game->currView == JEU) {
                    switch (event.key.key) {
                        case SDLK_ESCAPE: //a enlever car je creerai un menu special dans le jeu pour retourner a l'accueil ou reroll le jeu
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
                }
            }
        }
    }
}