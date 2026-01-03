# 🚀 Space Invaders (C / SDL3 / Ncurses)

Un clone du célèbre jeu d'arcade **Space Invaders**, développé en **C**.
Ce projet se distingue par son architecture **MVC (Modèle-Vue-Contrôleur)** qui permet de jouer au même jeu via deux interfaces totalement différentes :
1.  🎮 **Version Graphique** (SDL3 + Audio)
2.  📟 **Version Terminal** (Ncurses)

---

## Fonctionnalités

* **Architecture MVC :** Séparation stricte entre la logique (Model), l'affichage (View) et les entrées (Controller).
* **Dual Mode :**
    * Interface graphique fluide avec gestion des sprites et plein écran.
    * Interface rétro en mode texte (ASCII art) pour le terminal.
* **Système Audio (SDL) :** Musique de fond, bruitages de tir et d'explosion (via SDL_Mixer).
* **Gameplay complet :** Ennemis coordonnés, bunkers destructibles, score et vies.
* **Menus :** Accueil, Instructions, Pause, Game Over, Victoire.
* **Tests Unitaires :** Système automatisé pour valider la logique du jeu.

---

## Dépendances et Versions

Le projet est conçu pour **Linux** (Ubuntu/Debian et WSL).

### Bibliothèques requises :
* **GCC / Make :** Outils de compilation standard.
* **SDL3 & SDL3_mixer :** (Version 3.0+ requise). Gère le fenêtrage, le rendu GPU et l'audio.
* **Ncurses :** (Version 6.0+). Gère l'affichage dans le terminal.
* **CMake & Git :** Utilisés par le Makefile pour télécharger SDL3 si nécessaire.

---

## Installation et Compilation

Un **Makefile** automatisé est fourni pour simplifier le processus.
---

## Installation

Le projet dispose d'un **Makefile** automatisé qui gère l'installation des dépendances.

## Fait par Yaël Muselet Dumont et Théo Hecquet
