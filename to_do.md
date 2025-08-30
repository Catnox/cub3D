CUB3D a faire pour 42
il faut creer une representation graphique 3d realiste de l'interieur d'un labyrinthe depuis une perspective a la premiere personne en utilisant le raycasting.

-version avec bonus
voir dans le subjet
libft ok, toutes les fonction de la minilibx, toutes les fonctions de la librairie math (man math 3), ainsi que open, close, read, write, printf, malloc, free, perror, strerror, exit, gettimeofday.
les murs doivent avoir differentes texture selon l'orientation nord, sud, est, ouest.
sol et ciel doivent deux couleurs differentes.
les fleches gauche/droite doivent permettre de regarder a gauche/droite
les touche W,A,S,D doivent permettre de changer le point de vue.
la touche ESC doit permettre de fermer la fenetre de jeu et quitter proprement le jeu.
cliquer avec la souris sur la croix rouge doit fermer la fenetre de jeu et quitter proprement.
1er argument pris est le .cub
la map doit etre compose de 6 caracteres possible : 0 pour une case vide, 1 pour un mur et N, S, E ou W pour la position de depart du joueur et spawn l'orientation (donc le joueur si N regarde au nord au depart...).
la carte doit etre ferme par des murs sinon le programme retourne une erreur.
les element doivent etre avant la map sur le .cub peut importe l'ordre des elements.
space est valid pour le parsing de la map du moment que on respecte les regles de la map.
les elements doivent commencer par leur identifiants suivi du chemin (ex: NO ./path_to_north_texture, a faire pour NO, SO, WE and EA), pour le sol c'est F suivi du rgb dans la plage [0,255] (ex: F 220,100,0), a faire pour le ciel avec C.
partie bonus doit avoir :
la collision avec les murs
une mini map
des portes que l'on peut ouvrir et fermer
des animations
gerer le point de vue avec la souris
pour la partie bonus on peut ajouter d'autres symbole sur la map et utiliser d'autres fonctions

-ajout de HUD
ce sont des indication de jeux sur l'ecran tel que la sante le nombre de munitions...

-animation de l'arme
quelques images pour passer de l'arme a celui ou on voit le feu du canon de l'arme

-ajout des ennemies
pour que l'ennemi soit touche je pense qu'il faut regarder sur quelle case est l'ennemi et faire traverser le shoot sur cette case pour dire que l'ennemi est touche

-gestion de l'ia pour les ennemies
utilisation de l'algorithme A* pour definier le chemin le plus rapide

-ajout de musique
trouver quelque chose de repetitif que l'on peut jouer en boucle et libre de droit

-ajout de bruitage
bruit du coup de feu, bruit de l'ouverture de porte

-animation pour ouvrir les portes
jouer sur l'opacite ou enregistrer plusieurs images pour ouvrir la porte en la coulissant. a la fin de l'animation la porte est ouverte pour un temps. si le personnage ou les ennemis ne sont pas trop pres de la porte on peut passer la position a off puis animer la fermeture.

-generation aleatoire des niveaux
certainment faire une map avec calloc qui fait des espaces pour avoir une forme rectangulaire

-gestion vue haut/bas
pour regarder en haut ou en bas, generer l'image derriere le hud

-animation des murs
changer les murs pour indiquer la progression (couleur), animation visuelle.

-environnement visuel
possibilite de changer le hero, les ennemis, le theme.
theme nazi original.
ajouter mod.
si passage vu exterieur faire une lune de sang qui est remplie selon le nombre de mort atteint dans le jeu.

-difficulte du jeu
devoir trouver une cle pour ouvrir des portes afin de progresser

-items a ramasser
tresor pour high score, vie pour se remettre d'aplomb apres s'etre fait touche par un ennemi, munition pour ne pas avoir a utiliser l'arme de poing.

-controle
ajout de la course, du strife

-test
faire un script, fichier de test