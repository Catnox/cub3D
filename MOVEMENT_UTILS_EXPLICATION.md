# MOVEMENT_UTILS.C - EXPLICATION DÉTAILLÉE

## 📋 TABLE DES MATIÈRES

1. [Vue d'ensemble](#vue-densemble)
2. [Fonction check_collision](#fonction-check_collision)
3. [Fonction check_collision_with_buffer](#fonction-check_collision_with_buffer)
4. [Fonction calculate_new_position](#fonction-calculate_new_position)
5. [Fonction move_player](#fonction-move_player)
6. [Questions fréquentes](#questions-fréquentes)
7. [Diagrammes](#diagrammes)

---

## 🎯 VUE D'ENSEMBLE

### Rôle du fichier
`movement_utils.c` gère la **logique de déplacement du joueur** :
- Calcul des nouvelles positions
- Détection des collisions
- Application des mouvements
- Gestion de la hitbox du joueur

### Fonctions principales
```
movement_utils.c
├── check_collision()              - Collision basique point-mur
├── check_collision_with_buffer()  - Collision avancée avec hitbox
├── calculate_new_position()       - Calcul position après mouvement
└── move_player()                  - Application du mouvement final
```

### Constantes importantes
```c
const double buffer = 0.15;  // Taille de la hitbox du joueur
const double speed = 0.05;   // Vitesse de déplacement
```

---

## 🎲 FONCTION CHECK_COLLISION

### Signature
```c
int check_collision(t_game *game, double x, double y)
```

### Vue d'ensemble
Fonction basique qui vérifie si un point donné entre en collision avec :
- Les limites de la carte
- Les murs (caractère '1')

### Analyse ligne par ligne

```c
int	check_collision(t_game *game, double x, double y)
{
```
**Paramètres** :
- `game` : Structure principale contenant la map
- `x, y` : Coordonnées à vérifier (en double pour précision)

```c
	if (x < 0 || y < 0)
		return (1);
```
**Vérification des limites négatives** :
- Empêche de sortir de la carte par le haut ou la gauche
- `return (1)` = collision détectée

```c
	map_x = (int)x;
	map_y = (int)y;
```
**Conversion en coordonnées de grille** :
- Cast des doubles en int pour obtenir les indices de la map
- `floor()` implicite avec le cast vers int
- Exemple : `3.7` devient `3` pour l'indice de la map

```c
	if (map_x >= game->map->width || map_y >= game->map->height)
		return (1);
```
**Vérification des limites positives** :
- Empêche de sortir de la carte par le bas ou la droite
- Compare avec les dimensions de la map

```c
	if (game->map->grid[map_y][map_x] == '1')
		return (1);
```
**Vérification des murs** :
- Accède à la grille 2D de la map
- `'1'` représente un mur
- `return (1)` si on touche un mur

```c
	return (0);
```
**Pas de collision** :
- `return (0)` = position valide
- Le joueur peut se déplacer ici

### Exemple d'utilisation

```c
// Position valide (couloir)
check_collision(game, 1.5, 2.5) → 0  // OK

// Collision avec mur
check_collision(game, 1.0, 1.0) → 1  // Mur détecté

// Hors limites
check_collision(game, -1.0, 2.5) → 1  // Hors carte
```

---

## 🛡️ FONCTION CHECK_COLLISION_WITH_BUFFER

### Signature
```c
int check_collision_with_buffer(t_game *game, double x, double y)
```

### Vue d'ensemble
Version améliorée de la détection de collision qui :
- Crée une "hitbox" carrée autour du joueur
- Vérifie 5 points de collision (centre + 4 coins)
- Empêche le joueur de "glisser" contre les murs

### Analyse ligne par ligne

```c
int	check_collision_with_buffer(t_game *game, double x, double y)
{
```
**Paramètres identiques** à check_collision()

```c
	const double	buffer = 0.15;
```
**Taille de la hitbox** :
- 0.15 unités autour du point central
- Crée un carré de 0.3 × 0.3 unités
- La taille est empirique pour un bon gameplay

```c
	if (check_collision(game, x, y))
		return (1);
```
**Vérification du centre** :
- Premier test au point exact
- Si collision au centre → pas besoin de vérifier les coins

```c
	if (check_collision(game, x + buffer, y + buffer))
		return (1);
```
**Coin haut-droit** :
- Décalage de +0.15 en x et y
- Représente le coin supérieur droit de la hitbox

```c
	if (check_collision(game, x - buffer, y + buffer))
		return (1);
```
**Coin haut-gauche** :
- Décalage de -0.15 en x, +0.15 en y
- Représente le coin supérieur gauche de la hitbox

```c
	if (check_collision(game, x + buffer, y - buffer))
		return (1);
```
**Coin bas-droit** :
- Décalage de +0.15 en x, -0.15 en y
- Représente le coin inférieur droit de la hitbox

```c
	if (check_collision(game, x - buffer, y - buffer))
		return (1);
```
**Coin bas-gauche** :
- Décalage de -0.15 en x et y
- Représente le coin inférieur gauche de la hitbox

```c
	return (0);
```
**Aucune collision** :
- Tous les points sont valides
- Le joueur peut se déplacer en toute sécurité

### Visualisation de la hitbox

```
    Hitbox du joueur (vue de dessus)
    Taille : 0.3 × 0.3 unités

    P2(-)──────P3(+)
     │          │
     │    P1    │    (+) = +buffer
     │    (.)   │    (-) = -buffer
     │          │    (.) = point central
    P4(-)──────P5(+)

Points vérifiés :
P1 = (x, y)           // Centre
P2 = (x-b, y+b)       // Haut gauche
P3 = (x+b, y+b)       // Haut droit
P4 = (x-b, y-b)       // Bas gauche
P5 = (x+b, y-b)       // Bas droit
```

### Pourquoi une hitbox ?

Sans buffer (mauvais) :
```
┌─────┐
│     │
│  P  │ → Le joueur (P) peut s'approcher
│     │   trop près du mur
└─────┘
```

Avec buffer (bon) :
```
┌─────┐
│     │   [P] = hitbox du joueur
│ [P] │ → Maintient une distance minimale
│     │   avec le mur
└─────┘
```

---

## 🎯 FONCTION CALCULATE_NEW_POSITION

### Signature
```c
t_vec2 calculate_new_position(t_game *game, int direction)
```

### Vue d'ensemble
Calcule la nouvelle position du joueur en fonction :
- De sa direction actuelle
- Du type de mouvement demandé (avant, arrière, gauche, droite)
- De la vitesse de déplacement

### Analyse ligne par ligne

```c
t_vec2	calculate_new_position(t_game *game, int direction)
{
```
**Retour** : Structure t_vec2 contenant (x, y)
**Paramètres** :
- `game` : État du jeu
- `direction` : Code du mouvement (1, -1, 2, 3)

```c
	t_vec2			new_pos;
	const double	speed = 0.05;
```
**Variables locales** :
- `new_pos` : Position calculée à retourner
- `speed` : Vitesse de déplacement (0.05 unités/frame)

```c
	new_pos = game->player->pos;
```
**Copie position initiale** :
- Point de départ = position actuelle
- Sera modifiée selon le mouvement

```c
	if (direction == 1)
	{
		new_pos.x += game->player->dir.x * speed;
		new_pos.y += game->player->dir.y * speed;
	}
```
**Mouvement AVANT (W)** :
- Direction 1 = avancer
- Ajoute le vecteur direction * vitesse
- Utilise le vecteur de direction normalisé

**Calcul vectoriel** :
```
Nouvelle position = Position + Direction × Vitesse

pos.x += dir.x * 0.05
pos.y += dir.y * 0.05
```

```c
	else if (direction == -1)
	{
		new_pos.x -= game->player->dir.x * speed;
		new_pos.y -= game->player->dir.y * speed;
	}
```
**Mouvement ARRIÈRE (S)** :
- Direction -1 = reculer
- Soustrait le vecteur direction * vitesse
- Exactement l'inverse d'avancer

```c
	else if (direction == 2)
	{
		new_pos.x -= game->player->plane.x * speed;
		new_pos.y -= game->player->plane.y * speed;
	}
```
**Mouvement GAUCHE (A)** :
- Direction 2 = strafe gauche
- Utilise le vecteur plan (perpendiculaire à direction)
- Soustrait pour aller à gauche

**Vecteur plan** :
- Perpendiculaire au vecteur direction
- Utilisé pour les déplacements latéraux
- Rotation de 90° du vecteur direction

```c
	else if (direction == 3)
	{
		new_pos.x += game->player->plane.x * speed;
		new_pos.y += game->player->plane.y * speed;
	}
```
**Mouvement DROITE (D)** :
- Direction 3 = strafe droit
- Utilise aussi le vecteur plan
- Ajoute pour aller à droite

```c
	return (new_pos);
```
**Retourne nouvelle position** :
- Sans vérification de collision
- Les collisions sont gérées par move_player()

### Codes de direction

```c
direction == 1   → Avancer    (W)  : pos += dir * speed
direction == -1  → Reculer    (S)  : pos -= dir * speed
direction == 2   → Gauche     (A)  : pos -= plane * speed
direction == 3   → Droite     (D)  : pos += plane * speed
```

### Vecteurs utilisés

```
Vue de dessus du joueur :

       dir   Direction de regard
        ↑    (dir.x, dir.y)
        │
        │
    ←───┼───→  Vecteur plan
   plane│plane (plane.x, plane.y)
        │
        
Rotation de 90° :
plane.x = -dir.y
plane.y = +dir.x
```

---

## 🚶 FONCTION MOVE_PLAYER

### Signature
```c
void move_player(t_game *game, int direction)
```

### Vue d'ensemble
Fonction finale qui :
1. Calcule la nouvelle position souhaitée
2. Vérifie les collisions
3. Applique le mouvement valide
4. Gère les mouvements "glissants"

### Analyse ligne par ligne

```c
void	move_player(t_game *game, int direction)
{
```
**Point d'entrée** pour tout mouvement du joueur

```c
	t_vec2	new_pos;
```
**Variable locale** :
- Stocke la position candidate
- Permet de tester avant d'appliquer

```c
	new_pos = calculate_new_position(game, direction);
```
**Calcul position désirée** :
- Appelle la fonction précédente
- Dépend de la direction (1, -1, 2, 3)
- Ne prend pas en compte les collisions

```c
	if (!check_collision_with_buffer(game, new_pos.x, game->player->pos.y))
		game->player->pos.x = new_pos.x;
```
**Mouvement en X** :
1. Vérifie collision en X (garde Y actuel)
2. Si pas de collision → applique nouveau X
3. Sinon → garde l'ancien X

**Pourquoi tester séparément ?** :
- Permet le "glissement" le long des murs
- Plus fluide qu'un blocage total
- Meilleure expérience de jeu

```c
	if (!check_collision_with_buffer(game, game->player->pos.x, new_pos.y))
		game->player->pos.y = new_pos.y;
```
**Mouvement en Y** :
1. Vérifie collision en Y (avec nouveau/ancien X)
2. Si pas de collision → applique nouveau Y
3. Sinon → garde l'ancien Y

```c
}
```

### Exemple de glissement

```
Vue de dessus, joueur se déplace en diagonale vers un mur :

   Avant collision :     Après collision :
   
    ┌────┐               ┌────┐
    │    │               │    │
    │    │ ↗            │    │ →
    │    │ P            │    P
    └────┘               └────┘

1. Direction diagonale   1. Collision en Y
2. X et Y vont changer  2. X continue seul
3. P va vers ↗          3. P glisse vers →
```

### Ordre des opérations

```
move_player()
     ↓
calculate_new_position()  → Nouvelle pos (x, y)
     ↓
check X avec buffer      → OK ? → Update X
     ↓
check Y avec buffer      → OK ? → Update Y
```

---

## ❓ QUESTIONS FRÉQUENTES

### Q1 : Pourquoi utiliser un buffer de collision ?
**R :**
```c
const double buffer = 0.15;
```
- Évite que le joueur "colle" aux murs
- Crée une hitbox réaliste
- Améliore le gameplay et la navigation

### Q2 : Pourquoi séparer X et Y dans move_player ?
**R :**
```c
// Séparé
if (!collision(new_x, old_y)) move_x;
if (!collision(new_x, new_y)) move_y;

// VS Groupé (❌)
if (!collision(new_x, new_y)) {
    move_x;
    move_y;
}
```
→ Le séparé permet de glisser le long des murs !

### Q3 : Comment fonctionne le vecteur plan ?
**R :**
```
Direction : (dir_x, dir_y)
Plan     : (-dir_y, dir_x)  // Rotation 90°

Exemple :
Si direction = (1,0) → regarde à droite
Alors plan  = (0,1) → perpendiculaire haut
```

### Q4 : Pourquoi une vitesse de 0.05 ?
**R :**
```c
const double speed = 0.05;
```
- Équilibre entre réactivité et contrôle
- À 60 FPS = 3 unités par seconde
- Une case = 1 unité

### Q5 : Comment sont gérées les collisions en diagonale ?
**R :**
```
1. Joueur appuie W+D
2. calculate_new_position ajoute dir + plane
3. check_collision_with_buffer vérifie les 5 points
4. move_player applique X et/ou Y selon collisions
```

---

## 📊 DIAGRAMMES

### Système de collision avec buffer

```
Points de collision vérifiés :

    P2    P3
     ┌────┐
     │    │
     │ P1 │   P1 = Centre
     │    │   P2-P5 = Coins (±buffer)
     └────┘
    P4    P5

Si UN SEUL point touche = Collision !
```

### Vecteurs de mouvement

```
          N
          ↑
     NW ↖│↗ NE
        ┌─┼─┐
    W ←─┼─┼─→ E
        └─┼─┘
     SW ↙│↘ SE
          ↓
          S

Mouvement = Direction × Vitesse
W = (+1, 0)   × 0.05
S = (-1, 0)   × 0.05
A = (0, -1)   × 0.05
D = (0, +1)   × 0.05
```

### Flux de mouvement

```
[INPUT] → Touche pressée (ex: W)
   ↓
keyboard.c:handle_continuous_input()
   ↓
movement_utils.c:move_player(dir=1)
   ↓
calculate_new_position()
   │
   ├─→ Si W: pos += dir * speed
   ├─→ Si S: pos -= dir * speed
   ├─→ Si A: pos -= plane * speed
   └─→ Si D: pos += plane * speed
   ↓
check_collision_with_buffer()
   │
   ├─→ Check centre
   ├─→ Check coin haut-gauche
   ├─→ Check coin haut-droit
   ├─→ Check coin bas-gauche
   └─→ Check coin bas-droit
   ↓
move_player()
   │
   ├─→ Si X valide: update X
   └─→ Si Y valide: update Y
```

### Glissement le long des murs

```
Cas 1 : Approche perpendiculaire
┌────┐
│    │ ↑   Mouvement bloqué
│    │ P   complètement
└────┘

Cas 2 : Approche diagonale
┌────┐
│    │ ↗   Glisse horizontalement
│    │ P   le long du mur
└────┘

Cas 3 : Approche parallèle
┌────┐
│    │ →   Mouvement complet
│    P     sans blocage
└────┘
```

---

## 🎓 CHECKLIST POUR LA CORRECTION

### Comprendre check_collision()
- [ ] Expliquer la différence entre position réelle (double) et grille (int)
- [ ] Identifier les 3 types de collision (hors limites, murs)
- [ ] Comprendre le retour (1 = collision, 0 = ok)

### Comprendre check_collision_with_buffer()
- [ ] Expliquer le concept de hitbox
- [ ] Identifier les 5 points vérifiés
- [ ] Justifier la taille du buffer (0.15)

### Comprendre calculate_new_position()
- [ ] Expliquer les 4 directions de mouvement
- [ ] Comprendre les vecteurs direction et plan
- [ ] Expliquer la vitesse de déplacement

### Comprendre move_player()
- [ ] Expliquer la séparation X/Y
- [ ] Comprendre le mécanisme de glissement
- [ ] Identifier l'ordre des vérifications

### Questions pièges possibles
- [ ] "Pourquoi ne pas grouper X et Y ?"
  → Permet le glissement le long des murs
  
- [ ] "Comment fonctionne le mouvement diagonal ?"
  → Combinaison de direction et plan, tests séparés
  
- [ ] "Pourquoi un buffer de collision ?"
  → Meilleur gameplay, évite de coller aux murs
  
- [ ] "Comment est calculée la vitesse ?"
  → 0.05 unités/frame = 3 unités/seconde à 60 FPS
  
- [ ] "Comment fonctionne le vecteur plan ?"
  → Rotation 90° du vecteur direction

### Concepts clés à maîtriser
- [ ] **Hitbox** : Zone de collision autour du joueur
- [ ] **Vecteurs** : Direction et plan pour le mouvement
- [ ] **Glissement** : Tests séparés X/Y
- [ ] **Buffer** : Distance de sécurité avec les murs
- [ ] **Grille** : Conversion position réelle vers map

---

## 🎯 FIN DE L'EXPLICATION

Ce fichier couvre **100% du code** de `movement_utils.c` avec des explications détaillées adaptées pour une correction de projet 42.

**Fichiers d'explication disponibles** :
1. ✅ `EXECUTION_GUIDE.md` - Vue d'ensemble du raycasting
2. ✅ `MAIN_EXPLICATION.md` - Explication de main.c
3. ✅ `INIT_EXPLICATION.md` - Explication de init.c
4. ✅ `GAME_LOOP_EXPLICATION.md` - Explication de game_loop.c
5. ✅ `KEYBOARD_EXPLICATION.md` - Explication de keyboard.c
6. ✅ `MOVEMENT_UTILS_EXPLICATION.md` - Explication de movement_utils.c (ce fichier)

**Prochaines étapes suggérées** :
- Relire les 6 fichiers markdown
- Tracer le flux complet : input → keyboard → movement
- Tester les explications de collision
- Préparer des exemples de glissement le long des murs

Bon courage pour la correction ! 🚀