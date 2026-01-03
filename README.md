# 🚀 Space Invaders (C / SDL3 / Ncurses)

Un clone du célèbre jeu d'arcade **Space Invaders**, développé en **C**.
Ce projet se distingue par son architecture **MVC (Modèle-Vue-Contrôleur)** qui permet de jouer au même jeu via deux interfaces totalement différentes :
1.  🎮 **Version Graphique** (SDL3 + Audio)
2.  📟 **Version Terminal** (Ncurses)

---

## 📋 Fonctionnalités

* **Architecture MVC :** Séparation stricte entre la logique (Model), l'affichage (View) et les entrées (Controller).
* **Dual Mode :**
    * Interface graphique fluide avec gestion des sprites et plein écran.
    * Interface rétro en mode texte (ASCII art) pour le terminal.
* **Système Audio (SDL) :** Musique de fond, bruitages de tir et d'explosion (via SDL_Mixer).
* **Gameplay complet :**
    * Déplacement du vaisseau.
    * Tirs joueurs et ennemis.
    * Ennemis avec comportements (descente, accélération).
    * Bunkers destructibles.
    * Gestion des vies et du score.
* **Menus :** Accueil, Instructions, Pause, Game Over, Victoire.
* **Tests Unitaires :** Système automatisé pour valider la logique du jeu.

---

## 🛠️ Prérequis

Le projet fonctionne sous Linux (testé sur Ubuntu/Debian et Fedora).

### Dépendances nécessaires :
* `gcc` & `make`
* `SDL3` & `SDL3_mixer`
* `ncurses`
* `cmake` (pour la compilation des libs SDL si nécessaire)

---

README détaillé : compilation, dépendances (versions recommandées), exécution, commandes clavier, description de l’architecture MVC.
## 📥 Installation

Le projet dispose d'un **Makefile** puissant qui gère l'installation des dépendances pour vous.

### 1. Cloner le projet
```bash
git clone [https://votre-repo-git.git](https://votre-repo-git.git)
cd SpaceInvaders
```

---

## Fait par Yaël Muselet Dumont et Théo Hecquet
