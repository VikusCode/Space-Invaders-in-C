// tests/test_model.c
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/model.h"

// --- PETITE LIBRAIRIE DE TEST MAISON ---
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"

int tests_run = 0;
int tests_passed = 0;

void assert_true(int condition, const char* test_name) {
    tests_run++;
    if (condition) {
        printf("%s[PASS]%s %s\n", COLOR_GREEN, COLOR_RESET, test_name);
        tests_passed++;
    } else {
        printf("%s[FAIL]%s %s\n", COLOR_RED, COLOR_RESET, test_name);
        // On ne quitte pas, on veut voir les autres erreurs potentielles
    }
}

// --- LES TESTS ---

void test_initialisation() {
    GameState game;
    init_model(&game, 800, 600, 0);

    assert_true(game.width == 800, "Largeur init");
    assert_true(game.height == 600, "Hauteur init");
    assert_true(game.player.x == 400, "Position X Joueur init (800/2)");
    assert_true(game.nb_lives == 3, "Nombre de vies init");
    assert_true(game.score == 0, "Score init");
    
    // Vérifier qu'au moins un ennemi est vivant
    assert_true(game.enemies[0].alive == 1, "Ennemi 0 vivant");
}

void test_deplacement_joueur() {
    GameState game;
    init_model(&game, 100, 100, 0); // Petit plateau

    int x_depart = game.player.x;
    
    // 1. Déplacement Droite
    player_move(&game, 1);
    assert_true(game.player.x == x_depart + 1, "Joueur bouge a droite");

    // 2. Déplacement Gauche (retour)
    player_move(&game, -1);
    assert_true(game.player.x == x_depart, "Joueur bouge a gauche");

    // 3. Test Mur Gauche (On force la position à 0)
    game.player.x = 0;
    player_move(&game, -1);
    assert_true(game.player.x == 0, "Collision mur gauche (ne doit pas sortir)");

    // 4. Test Mur Droit
    game.player.x = 99; // Largeur 100 (0 à 99)
    player_move(&game, 1);
    assert_true(game.player.x == 99, "Collision mur droit (ne doit pas sortir)");
}

void test_tir_joueur() {
    GameState game;
    init_model(&game, 800, 600, 0);
    game.isSDL = 0; // Important : On désactive le son pour les tests !

    // Au début, aucune balle active
    assert_true(game.bullets[0].active == 0, "Balle 0 inactive au depart");

    // Le joueur tire
    player_shoot(&game);

    // On cherche la balle activée
    int bullet_found = 0;
    for(int i=0; i<MAX_BULLETS; i++) {
        if(game.bullets[i].active) {
            bullet_found = 1;
            // La balle doit être à la position du joueur (y-1)
            assert_true(game.bullets[i].x == game.player.x, "X Balle ok");
            assert_true(game.bullets[i].y == game.player.y - 1, "Y Balle ok");
            break;
        }
    }
    assert_true(bullet_found, "Une balle a ete tiree");
}

void test_collision_ennemi() {
    GameState game;
    init_model(&game, 800, 600, 0);
    game.isSDL = 0; // Pas de son

    // SCENARIO : On place artificiellement un ennemi et une balle au même endroit
    game.enemies[0].x = 50;
    game.enemies[0].y = 50;
    game.enemies[0].alive = 1;
    game.enemies[0].type = OCTOPUS; // Rapporte 10 points

    game.bullets[0].x = 50;
    game.bullets[0].y = 50;
    game.bullets[0].active = 1;

    // On lance la vérification
    check_collisions(&game);

    assert_true(game.enemies[0].alive == 0, "Ennemi mort apres collision");
    assert_true(game.bullets[0].active == 0, "Balle detruite apres collision");
    assert_true(game.score == 10, "Score augmente (+10)");
}

void test_collision_joueur_gameover() {
    GameState game;
    init_model(&game, 800, 600, 0);
    game.isSDL = 0;

    // SCENARIO : Un ennemi descend sur le joueur
    game.enemies[0].x = game.player.x;
    game.enemies[0].y = game.player.y; // Même hauteur que le joueur !
    game.enemies[0].alive = 1;

    update_enemies(&game);

    assert_true(game.game_over == 1, "Game Over detecte (Ennemi sur joueur)");
    assert_true(game.currView == MENU_PERD, "Vue changee vers MENU_PERD");
}

int main() {
    printf("========================================\n");
    printf("    LANCEMENT DES TESTS UNITAIRES       \n");
    printf("========================================\n");

    test_initialisation();
    test_deplacement_joueur();
    test_tir_joueur();
    test_collision_ennemi();
    test_collision_joueur_gameover();

    printf("========================================\n");
    printf("RESULTAT : %d / %d tests passes.\n", tests_passed, tests_run);
    
    if (tests_passed == tests_run) {
        printf("%sTOUS LES SYSTEMES SONT OPERATIONNELS !%s\n", COLOR_GREEN, COLOR_RESET);
        return 0;
    } else {
        printf("%sERREUR : CERTAINS TESTS ONT ECHOUE.%s\n", COLOR_RED, COLOR_RESET);
        return 1;
    }
}