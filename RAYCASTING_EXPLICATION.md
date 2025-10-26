# RAYCASTING.C - EXPLICATION DÉTAILLÉE

## 📋 TABLE DES MATIÈRES

1. [Vue d'ensemble](#vue-densemble)
2. [Fonction calculate_ray_direction](#fonction-calculate_ray_direction)
3. [Fonction calculate_delta_distances](#fonction-calculate_delta_distances)
4. [Fonction calculate_step_and_side_dist](#fonction-calculate_step_and_side_dist)
5. [Fonction init_ray](#fonction-init_ray)
6. [Fonction cast_rays](#fonction-cast_rays)
7. [Questions fréquentes](#questions-fréquentes)
8. [Diagrammes](#diagrammes)

---

## 🎯 VUE D'ENSEMBLE

### Rôle du fichier
`raycasting.c` contient le **cœur de l'algorithme de raycasting** :
- Initialisation des rayons pour chaque colonne d'écran
- Calcul des directions et distances pour l'algorithme DDA
- Coordination du lancement de tous les rayons
- Interface entre rendu et détection de collision

### Fonctions principales
```
raycasting.c
├── calculate_ray_direction()     (static) - Direction du rayon
├── calculate_delta_distances()   (static) - Distances delta DDA
├── calculate_step_and_side_dist() (static) - Pas et distances latérales
├── init_ray()                   (public)  - Initialisation complète
└── cast_rays()                  (public)  - Lancement de tous les rayons
```

### Algorithme DDA
Le **Digital Differential Analyzer** est utilisé pour :
- Traverser efficacement la grille de la map
- Trouver la première collision avec un mur
- Calculer la distance exacte du mur

### Dépendances
```c
#include "cub3d.h"
```
- **DDA** : `perform_dda()` (dans dda.c)
- **Rendu** : `draw_walls()` (dans render.c)
- **Math** : `fabs()`

---

## 🎯 FONCTION CALCULATE_RAY_DIRECTION

### Signature
```c
static void calculate_ray_direction(t_ray *ray, t_player *player, int x, t_game *game)
```

### Vue d'ensemble
Calcule la **direction du rayon** pour une colonne d'écran donnée. Convertit une position pixel écran en direction 3D dans le monde.

### Analyse ligne par ligne

```c
static void	calculate_ray_direction(t_ray *ray, t_player *player,
		int x, t_game *game)
{
```
**Paramètres** :
- `ray` : Structure rayon à initialiser
- `player` : Position et orientation du joueur
- `x` : Colonne écran (0 à WINDOW_WIDTH-1)
- `game` : Contexte global

```c
	double	camera_x;
```
**Variable locale** : Position normalisée sur le plan de projection

```c
	camera_x = 2 * x / (double)game->img->width - 1;
```
**Normalisation de la position écran** :

**Formule** : `camera_x = 2 × (x / largeur) - 1`

**Conversion** :
```
x = 0           → camera_x = 2×(0/1024) - 1 = -1    (bord gauche)
x = 512         → camera_x = 2×(512/1024) - 1 = 0   (centre)
x = 1023        → camera_x = 2×(1023/1024) - 1 ≈ 1  (bord droit)
```

**Résultat** : `camera_x ∈ [-1, 1]`
- -1 = bord gauche de l'écran
- 0 = centre de l'écran
- +1 = bord droit de l'écran

**Pourquoi cette normalisation ?** :
- Le plan de projection va de -1 à +1
- Indépendant de la résolution d'écran
- Facilite les calculs géométriques

```c
	ray->dir.x = player->dir.x + player->plane.x * camera_x;
	ray->dir.y = player->dir.y + player->plane.y * camera_x;
```
**Calcul direction du rayon** :

**Formule vectorielle** :
```
direction_rayon = direction_joueur + plan_caméra × position_écran
```

**Décomposition** :
- `player->dir` : Direction centrale du regard (centre écran)
- `player->plane` : Vecteur perpendiculaire (largeur du champ de vision)
- `camera_x` : Position sur le plan [-1, +1]

**Logique géométrique** :
```
Centre écran (camera_x = 0) :
ray->dir = player->dir + plane × 0 = player->dir
→ Rayon au centre = direction du joueur

Bord gauche (camera_x = -1) :
ray->dir = player->dir + plane × (-1) = player->dir - plane
→ Rayon décalé vers la gauche

Bord droit (camera_x = +1) :
ray->dir = player->dir + plane × (+1) = player->dir + plane
→ Rayon décalé vers la droite
```

```c
}
```

### Exemple numérique

```c
Joueur regarde vers l'est (1, 0), plan (0, 0.66) :

x = 0 (bord gauche) :
camera_x = -1
ray->dir.x = 1 + 0×(-1) = 1
ray->dir.y = 0 + 0.66×(-1) = -0.66
→ Direction (1, -0.66) = Est-Nord

x = 512 (centre) :
camera_x = 0
ray->dir.x = 1 + 0×0 = 1
ray->dir.y = 0 + 0.66×0 = 0
→ Direction (1, 0) = Est pur

x = 1023 (bord droit) :
camera_x ≈ 1
ray->dir.x = 1 + 0×1 = 1
ray->dir.y = 0 + 0.66×1 = 0.66
→ Direction (1, 0.66) = Est-Sud
```

---

## 📏 FONCTION CALCULATE_DELTA_DISTANCES

### Signature
```c
static void calculate_delta_distances(t_ray *ray)
```

### Vue d'ensemble
Calcule les **distances delta** pour l'algorithme DDA. Ces distances représentent combien le rayon parcourt pour traverser une case de grille.

### Analyse ligne par ligne

```c
static void	calculate_delta_distances(t_ray *ray)
{
```

```c
	if (ray->dir.x == 0)
		ray->delta_dist.x = 1e30;
	else
		ray->delta_dist.x = fabs(1 / ray->dir.x);
```
**Delta distance X** :

**Si `ray->dir.x == 0`** :
- Rayon parfaitement vertical
- Ne croise jamais de lignes verticales
- Distance infinie : `1e30` (très grand nombre)

**Sinon** :
- `delta_dist.x = |1 / dir.x|`
- Distance pour traverser une case horizontalement

**Logique géométrique** :
```
Pour avancer de 1 unité en X, le rayon avance de |1/dir.x| unités au total.

Exemple :
dir.x = 0.5 → delta_dist.x = |1/0.5| = 2
→ Pour 1 case X, le rayon parcourt 2 unités de distance

dir.x = 2.0 → delta_dist.x = |1/2.0| = 0.5
→ Pour 1 case X, le rayon parcourt 0.5 unités de distance
```

```c
	if (ray->dir.y == 0)
		ray->delta_dist.y = 1e30;
	else
		ray->delta_dist.y = fabs(1 / ray->dir.y);
```
**Delta distance Y** : Logique identique pour l'axe Y

```c
}
```

### Principe des distances delta

```
Rayon avec direction (0.6, 0.8) :

delta_dist.x = |1/0.6| = 1.67
delta_dist.y = |1/0.8| = 1.25

Visualisation :
     ┌───┬───┬───┐
     │   │   │   │
     ├───┼───┼───┤
     │   │ P │   │  P = position joueur
     ├───┼───┼───┤    → = direction rayon
     │   │   │   │
     └───┴───┴───┘

Pour traverser 1 case X : rayon avance de 1.67 unités
Pour traverser 1 case Y : rayon avance de 1.25 unités
```

---

## 🎲 FONCTION CALCULATE_STEP_AND_SIDE_DIST

### Signature
```c
static void calculate_step_and_side_dist(t_ray *ray, t_player *player)
```

### Vue d'ensemble
Calcule les **directions de pas** et les **distances latérales initiales** pour l'algorithme DDA. Détermine dans quelle direction avancer dans la grille et à quelle distance se trouve la prochaine ligne de grille.

### Analyse ligne par ligne

```c
static void	calculate_step_and_side_dist(t_ray *ray, t_player *player)
{
```

### Direction X

```c
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x = (player->pos.x - ray->map_x) * ray->delta_dist.x;
	}
```
**Rayon va vers la gauche** :

**`ray->step_x = -1`** :
- Pas négatif : on se déplace vers la gauche dans la grille
- À chaque étape DDA, `map_x` diminue de 1

**`ray->side_dist.x`** :
- Distance jusqu'à la prochaine ligne verticale vers la gauche
- `(pos.x - map_x)` = distance fractionnaire depuis le bord gauche de la case
- Multipliée par `delta_dist.x` pour obtenir la distance réelle du rayon

**Exemple** :
```c
player->pos.x = 2.3, ray->map_x = 2, delta_dist.x = 1.67

side_dist.x = (2.3 - 2) × 1.67 = 0.3 × 1.67 = 0.5
→ Distance 0.5 jusqu'à la ligne verticale x=2
```

```c
	else
	{
		ray->step_x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - player->pos.x)
			* ray->delta_dist.x;
	}
```
**Rayon va vers la droite** :

**`ray->step_x = 1`** :
- Pas positif : on se déplace vers la droite dans la grille
- À chaque étape DDA, `map_x` augmente de 1

**`ray->side_dist.x`** :
- Distance jusqu'à la prochaine ligne verticale vers la droite
- `(map_x + 1.0 - pos.x)` = distance fractionnaire jusqu'au bord droit de la case

**Exemple** :
```c
player->pos.x = 2.3, ray->map_x = 2, delta_dist.x = 1.67

side_dist.x = (2 + 1.0 - 2.3) × 1.67 = 0.7 × 1.67 = 1.17
→ Distance 1.17 jusqu'à la ligne verticale x=3
```

### Direction Y

```c
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y = (player->pos.y - ray->map_y) * ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - player->pos.y)
			* ray->delta_dist.y;
	}
```
**Logique identique pour Y** : Même principe appliqué à l'axe vertical

```c
}
```

### Visualisation des distances latérales

```
Position joueur (2.3, 1.7) dans la grille :

     0   1   2   3   4
   ┌───┬───┬───┬───┬───┐
 0 │   │   │   │   │   │
   ├───┼───┼───┼───┼───┤
 1 │   │   │ P │   │   │  P = (2.3, 1.7)
   ├───┼───┼───┼───┼───┤
 2 │   │   │   │   │   │
   └───┴───┴───┴───┴───┘

map_x = 2, map_y = 1

Direction vers la droite (dir.x > 0) :
side_dist.x = (3 - 2.3) × delta_dist.x = 0.7 × delta_dist.x
→ Distance jusqu'à la ligne x=3

Direction vers le bas (dir.y > 0) :
side_dist.y = (2 - 1.7) × delta_dist.y = 0.3 × delta_dist.y
→ Distance jusqu'à la ligne y=2
```

---

## 🏁 FONCTION INIT_RAY

### Signature
```c
void init_ray(t_ray *ray, t_player *player, int x, t_game *game)
```

### Vue d'ensemble
Fonction **publique** qui initialise complètement un rayon pour l'algorithme DDA. Coordonne tous les calculs préparatoires.

### Analyse ligne par ligne

```c
void	init_ray(t_ray *ray, t_player *player, int x, t_game *game)
{
```

```c
	calculate_ray_direction(ray, player, x, game);
```
**Étape 1** : Calcul de la direction du rayon
- Convertit la colonne écran en direction 3D
- Utilise la position et l'orientation du joueur

```c
	ray->map_x = (int)player->pos.x;
	ray->map_y = (int)player->pos.y;
```
**Étape 2** : Position de départ dans la grille
- Case de grille où se trouve le joueur
- Point de départ pour l'algorithme DDA
- Cast vers int = coordonnées de case

```c
	calculate_delta_distances(ray);
```
**Étape 3** : Calcul des distances delta
- Distance pour traverser une case en X
- Distance pour traverser une case en Y

```c
	calculate_step_and_side_dist(ray, player);
```
**Étape 4** : Direction et distances initiales
- Détermine les pas (+1 ou -1) pour X et Y
- Calcule les distances jusqu'aux prochaines lignes de grille

```c
	ray->hit = 0;
```
**Étape 5** : Initialisation état
- `hit = 0` : Aucun mur trouvé pour l'instant
- Sera mis à 1 par `perform_dda()` quand un mur est trouvé

```c
}
```

### Ordre d'initialisation

```
1. calculate_ray_direction()     → ray->dir.x, ray->dir.y
2. Position initiale             → ray->map_x, ray->map_y
3. calculate_delta_distances()   → ray->delta_dist.x, ray->delta_dist.y
4. calculate_step_and_side_dist() → ray->step_x, ray->step_y,
                                   ray->side_dist.x, ray->side_dist.y
5. État initial                  → ray->hit = 0
```

**Pourquoi cet ordre ?** :
- Direction nécessaire pour delta distances
- Delta distances nécessaires pour side distances
- Tout doit être initialisé avant DDA

---

## 🌟 FONCTION CAST_RAYS

### Signature
```c
void cast_rays(t_game *game)
```

### Vue d'ensemble
Fonction **maître** du raycasting qui lance un rayon pour chaque colonne de l'écran. Coordonne tout le processus de rendu 3D.

### Analyse ligne par ligne

```c
void	cast_rays(t_game *game)
{
```
**Point d'entrée principal** du raycasting

```c
	t_ray	ray;
	int		x;
```
**Variables locales** :
- `ray` : Structure réutilisée pour chaque colonne
- `x` : Colonne d'écran en cours (0 à largeur-1)

```c
	x = 0;
	while (x < (int)game->img->width)
	{
```
**Boucle principale** :
- Une itération par colonne d'écran
- Largeur typique : 1024 colonnes

```c
		init_ray(&ray, game->player, x, game);
```
**Initialisation du rayon** :
- Prépare le rayon pour la colonne `x`
- Calcule direction, positions, distances

```c
		perform_dda(&ray, game);
```
**Algorithme DDA** :
- Fonction dans `dda.c`
- Traverse la grille jusqu'à trouver un mur
- Met à jour `ray->hit`, `ray->perp_wall_dist`, `ray->side`

```c
		draw_walls(game, x, &ray);
```
**Rendu de la colonne** :
- Fonction dans `render.c`
- Utilise les résultats du DDA pour dessiner
- Dessine une colonne de pixels à la position `x`

```c
		x++;
	}
```
**Colonne suivante** : Incrémente pour la prochaine itération

```c
}
```

### Flux complet d'une frame

```
cast_rays() appelé depuis render_frame()
     │
     ├─→ x=0: init_ray() → perform_dda() → draw_walls()
     ├─→ x=1: init_ray() → perform_dda() → draw_walls()
     ├─→ x=2: init_ray() → perform_dda() → draw_walls()
     │   ...
     └─→ x=1023: init_ray() → perform_dda() → draw_walls()
     
Résultat : Image 3D complète (1024 colonnes)
```

### Performance

**Opérations par frame** :
- 1024 rayons lancés
- 1024 algorithmes DDA exécutés
- 1024 colonnes de murs dessinées

**À 60 FPS** :
- 61,440 rayons par seconde
- Performances critiques pour le temps réel

---

## ❓ QUESTIONS FRÉQUENTES

### Q1 : Comment fonctionne la conversion écran → direction ?
**R :**
```c
camera_x = 2*x/width - 1;  // [-1, +1]
ray_dir = player_dir + plane * camera_x;

Centre écran (x=512) → camera_x=0 → direction joueur
Bord gauche (x=0)    → camera_x=-1 → direction - plan
Bord droit (x=1023)  → camera_x=+1 → direction + plan
```

### Q2 : Pourquoi utiliser des distances delta ?
**R :**
```c
delta_dist = |1 / ray_direction|

Optimisation DDA : précalcule combien le rayon avance
pour traverser une case de grille.
Évite les calculs répétitifs dans la boucle DDA.
```

### Q3 : Comment déterminer les directions de pas ?
**R :**
```c
if (dir.x < 0) step_x = -1;  // Vers la gauche
else           step_x = +1;  // Vers la droite

Le pas détermine dans quelle direction avancer
dans la grille lors de l'algorithme DDA.
```

### Q4 : Que représentent les side_dist ?
**R :**
```c
side_dist.x = distance jusqu'à la prochaine ligne verticale
side_dist.y = distance jusqu'à la prochaine ligne horizontale

Ces distances initiales permettent de démarrer l'algorithme DDA.
```

### Q5 : Pourquoi ray->hit = 0 dans init_ray ?
**R :**
```c
ray->hit = 0;  // Aucun mur trouvé initialement

perform_dda() mettra hit=1 quand un mur sera détecté.
C'est le flag d'arrêt de l'algorithme DDA.
```

### Q6 : Comment éviter les divisions par zéro ?
**R :**
```c
if (ray->dir.x == 0)
    ray->delta_dist.x = 1e30;  // "Infini"
else
    ray->delta_dist.x = fabs(1 / ray->dir.x);

1e30 représente une distance infinie pour les rayons
parfaitement verticaux/horizontaux.
```

### Q7 : Quelle est la différence avec map_x/map_y ?
**R :**
```c
player->pos.x = 2.3  // Position réelle (flottant)
ray->map_x = 2       // Case de grille (entier)

map_x/y sont les coordonnées discrètes pour parcourir la grille.
```

### Q8 : Combien de rayons sont lancés par frame ?
**R :**
```c
width = 1024  // Colonnes d'écran
→ 1024 rayons par frame
→ 61,440 rayons/seconde à 60 FPS
```

### Q9 : Pourquoi la boucle cast_rays est-elle si simple ?
**R :**
La complexité est déléguée :
- `init_ray()` : Préparation
- `perform_dda()` : Algorithme de traversée
- `draw_walls()` : Rendu final

Séparation des responsabilités !

### Q10 : Comment débugger les directions de rayon ?
**R :**
```c
// Ajout temporaire dans calculate_ray_direction()
printf("x=%d, camera_x=%.2f, dir=(%.2f,%.2f)\n", 
       x, camera_x, ray->dir.x, ray->dir.y);

Vérifier que :
- Centre écran → direction joueur
- Bords écran → directions décalées
```

---

## 📊 DIAGRAMMES

### Pipeline raycasting complet

```
cast_rays(game)
      │
      ├─→ FOR x = 0 to 1023:
      │    │
      │    ├─→ init_ray(x)
      │    │    ├─→ calculate_ray_direction()  → ray->dir
      │    │    ├─→ Position grille            → ray->map_x/y
      │    │    ├─→ calculate_delta_distances() → ray->delta_dist
      │    │    ├─→ calculate_step_and_side_dist() → ray->step, ray->side_dist
      │    │    └─→ ray->hit = 0
      │    │
      │    ├─→ perform_dda()                   → ray->hit=1, ray->perp_wall_dist
      │    │
      │    └─→ draw_walls()                    → Colonne écran dessinée
      │
      └─→ Image 3D complète
```

### Conversion écran → direction

```
Écran (vue de dessus) :        Directions des rayons :

     0   512  1023                    ↖  ↑  ↗
   ┌─────┬─────┬─────┐               ←  P  → Joueur regarde ↑
   │  x  │  x  │  x  │                 Plan de projection
   └─────┴─────┴─────┘

camera_x:  -1    0   +1

Calcul direction :
ray_dir = player_dir + plane × camera_x

Exemple joueur regarde Nord (0,1), plan (1,0) :
x=0    : dir = (0,1) + (1,0)×(-1) = (-1,1) ↖
x=512  : dir = (0,1) + (1,0)×(0)  = (0,1)  ↑
x=1023 : dir = (0,1) + (1,0)×(+1) = (1,1)  ↗
```

### Calcul distances delta

```
Rayon avec direction (0.6, 0.8) :

      Y
      ↑
    2 ┼─┬─┬─┬─→
      │ │ │ │
    1 ┼─┼─┼─┼─
      │ │P│ │  P = joueur (1.5, 1.2)
    0 ┼─┼─┼─┼─  Rayon direction (0.6, 0.8)
      0 1 2 3 → X

delta_dist.x = |1/0.6| = 1.67
delta_dist.y = |1/0.8| = 1.25

Signification :
- Pour traverser 1 case en X : rayon avance 1.67 unités
- Pour traverser 1 case en Y : rayon avance 1.25 unités
```

### Calcul step et side_dist

```
Joueur à (2.3, 1.7), direction (0.5, -0.3) :

     0   1   2   3   4
   ┌───┬───┬───┬───┬───┐
 0 │   │   │   │   │   │
   ├───┼───┼───┼───┼───┤
 1 │   │   │ P─→ │   │  P = (2.3, 1.7)
   ├───┼───┼───┼───┼───┤  direction (0.5, -0.3)
 2 │   │   │   │   │   │
   └───┴───┴───┴───┴───┘

Direction X : dir.x = 0.5 > 0
→ step_x = +1 (vers la droite)
→ side_dist.x = (3 - 2.3) × delta_dist.x = 0.7 × delta_dist.x

Direction Y : dir.y = -0.3 < 0
→ step_y = -1 (vers le haut)
→ side_dist.y = (1.7 - 1) × delta_dist.y = 0.7 × delta_dist.y
```

### Champ de vision (FOV)

```
Vue de dessus du champ de vision :

               Rayons projetés
                    │ │ │
                    │ │ │
              ┌─────┼─┼─┼─────┐
              │     │ │ │     │
         ┌────┼─────┼─┼─┼─────┼────┐
         │    │     │ │ │     │    │
      ───┼────┼─────┼─P─┼─────┼────┼───
         │    │     │ │ │     │    │  P = joueur
         └────┼─────┼─┼─┼─────┼────┘
              │     │ │ │     │
              └─────┼─┼─┼─────┘
                    │ │ │
                  Plan de projection

Le vecteur "plane" détermine la largeur du FOV.
Plus le plan est grand → FOV plus large.
```

### Progression DDA

```
Exemple progression DDA dans la grille :

Initial: P = (1.3, 1.8), direction (0.8, 0.6)
map_x=1, map_y=1

     0   1   2   3
   ┌───┬───┬───┬───┐
 0 │   │   │   │   │
   ├───┼───┼───┼───┤
 1 │   │ P●│   │   │  ● = position initiale
   ├───┼───┼───┼───┤    → = direction rayon
 2 │   │   │ ■ │   │  ■ = mur trouvé
   └───┴───┴───┴───┘

DDA Steps :
1. Compare side_dist.x vs side_dist.y
2. Avance dans la direction avec plus petite side_dist
3. Met à jour map_x/y et side_dist correspondante
4. Vérifie si case = mur
5. Répète jusqu'à hit=1
```

### Résolution et performance

```
Résolution 1024×768 :

┌─────────────────────────────────────────────┐
│████████████████████████████████████████████│ ← 768 lignes
│████████████████████████████████████████████│
│████████████████████████████████████████████│   Chaque colonne
│████████████████████████████████████████████│   = 1 rayon
│████████████████████████████████████████████│
│████████████████████████████████████████████│
│████████████████████████████████████████████│
└─────────────────────────────────────────────┘
 ↑                                         ↑
 Rayon 0                              Rayon 1023

Nombre de rayons par frame : 1024
Nombre de rayons par seconde (60 FPS) : 61,440
```

---

## 🎓 CHECKLIST POUR LA CORRECTION

### Comprendre calculate_ray_direction()
- [ ] Expliquer la normalisation camera_x [-1, +1]
- [ ] Comprendre la formule vectorielle dir + plane × camera_x
- [ ] Identifier le rôle du vecteur plan (largeur FOV)
- [ ] Expliquer la correspondance pixel → direction 3D

### Comprendre calculate_delta_distances()
- [ ] Expliquer le principe |1/direction|
- [ ] Comprendre le cas dir=0 (distance infinie)
- [ ] Identifier l'optimisation pour l'algorithme DDA
- [ ] Expliquer l'unité (distance rayon par case grille)

### Comprendre calculate_step_and_side_dist()
- [ ] Différencier step (+1/-1) selon direction
- [ ] Comprendre side_dist comme distance initiale
- [ ] Expliquer les formules différentes selon signe direction
- [ ] Identifier la préparation pour l'algorithme DDA

### Comprendre init_ray()
- [ ] Expliquer l'ordre d'initialisation
- [ ] Comprendre l'interdépendance des calculs
- [ ] Identifier ray->hit=0 comme état initial
- [ ] Expliquer la conversion pos réelle → map entier

### Comprendre cast_rays()
- [ ] Expliquer la boucle sur les colonnes écran
- [ ] Comprendre la réutilisation de la structure ray
- [ ] Identifier la délégation perform_dda() et draw_walls()
- [ ] Expliquer la correspondance colonne → rayon

### Questions pièges possibles
- [ ] "Comment éviter l'effet fish-eye ?"
  → Utiliser perp_wall_dist calculée par DDA
  
- [ ] "Pourquoi 1024 rayons exactement ?"
  → Un rayon par colonne d'écran (résolution)
  
- [ ] "Comment gérer les rayons parfaitement verticaux ?"
  → delta_dist = 1e30 (distance infinie)
  
- [ ] "Quelle est la différence entre side_dist et delta_dist ?"
  → side_dist = distance initiale, delta_dist = distance par case
  
- [ ] "Comment optimiser les performances ?"
  → Précalculs (delta_dist), algorithme DDA efficace

### Concepts clés à maîtriser
- [ ] **Projection perspective** : Conversion 2D écran → 3D monde
- [ ] **DDA** : Algorithme de traversée de grille
- [ ] **Vecteur plan** : Largeur du champ de vision
- [ ] **Distance delta** : Optimisation calculs DDA
- [ ] **Normalisation** : camera_x [-1, +1] pour FOV

---

## 🎯 FIN DE L'EXPLICATION

Ce fichier couvre **100% du code** de `raycasting.c` avec des explications détaillées adaptées pour une correction de projet 42.

**Fichiers d'explication disponibles** :
1. ✅ `EXECUTION_GUIDE.md` - Vue d'ensemble du raycasting
2. ✅ `MAIN_EXPLICATION.md` - Explication de main.c
3. ✅ `INIT_EXPLICATION.md` - Explication de init.c
4. ✅ `GAME_LOOP_EXPLICATION.md` - Explication de game_loop.c
5. ✅ `KEYBOARD_EXPLICATION.md` - Explication de keyboard.c
6. ✅ `MOVEMENT_UTILS_EXPLICATION.md` - Explication de movement_utils.c
7. ✅ `MOVEMENTS_EXPLICATION.md` - Explication de movements.c
8. ✅ `RENDER_EXPLICATION.md` - Explication de render.c
9. ✅ `WALL_UTILS_EXPLICATION.md` - Explication de wall_utils.c
10. ✅ `RAYCASTING_EXPLICATION.md` - Explication de raycasting.c (ce fichier)

**Cœur du moteur 3D documenté** :
- Conversion écran → rayons 3D
- Préparation algorithme DDA
- Coordination rendu complet

**Prochaines étapes suggérées** :
- Comprendre le lien avec perform_dda() (dda.c)
- Maîtriser les concepts de projection perspective
- Visualiser la correspondance pixel ↔ direction

Bon courage pour la correction ! 🚀