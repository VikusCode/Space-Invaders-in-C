# Space-Invaders-in-C

Pour l'instant c'est un bon debut, ce que je dois faire, regler la vitesse dans mon sdl car la c'est trop rapide, ne tirer qu'un seul missile a la fois car la on peut spammer mdrr, aussi mettre une fin quand on meurt dans le jeu.
Le main_sdl est un fichier de test principalement, j'ai decale tous les contreoles dans le handle event, ilm rest a finaliser des chosesd meme si en soit la le sdl "fonctionne"

L'accueil en sdl est pour l'instant moche ca ira mieux apres je vais le faire oklm

Faut aussi je nettoie le fichier view_sdl.c et .h car c'est tellement deguelasse c'est infernal.
Faut aussi qu'on arrive a lier nos modele ensemble les concatener oklm

dans handle_event dna scontroller.c j'ai un deuxieme parametre *running a la base je voulais pas lke mettre, j'ai du le mettre je verrai comment faire autrement 

si tu veux tester tout mon code pour l'instant tu peux faire cette commande:
gcc src/model.c src/view_sdl.c src/controller.c src/main_sdl.c -o game_sdl -lSDL3 puis ./game_sdl

update ce qui a change:
la vitesse des ennemis, le compteur de vie dans sdl est bon, le game over aussi, j'ai ajoute un enum pour le type de l'ennemi en face pour l'affichage qui est beaucoup plus beau, le vaisseau aussi, la page d'accueil est a jour et bien belle
j'ai enleve le escape de la page d'accueil car maintenant y a un bouton quitter j'ai laisse la touche entree dans la page d'accueil malgre le bouton, j'ai ajoute un ecran de pause dans le jeu seul probleme, le bouton quitter doit me f  aire aller vers ma page d'accueil malheuresement j'y arrive pas c'est pour ca ya des // en dessous de running = 0