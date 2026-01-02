#pragma once
#include "../include/model.h"
/*
    La vue doit traduire ce qu'on va créer dans le modèle, le controleur doit mettre à jour le modèle 
    Donc en gros, le modèle dépend des controles qui dépendent de la vue
*/

void handle_event(GameState *game, int *running);
int handle_input_ncurses(GameState *game);