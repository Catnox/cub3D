# DDA.C - EXPLICATION DÉTAILLÉE

## 📋 TABLE DES MATIÈRES

1. [Vue d'ensemble](#vue-densemble)
2. [Fonction dda_step](#fonction-dda_step)
3. [Fonction check_wall_hit](#fonction-check_wall_hit)
4. [Fonction perform_dda](#fonction-perform_dda)
5. [Questions fréquentes](#questions-fréquentes)
6. [Diagrammes](#diagrammes)

---

## 🎯 VUE D'ENSEMBLE

### Rôle du fichier
`dda.c` implémente l'**algorithme DDA (Digital Differential Analyzer)** :
- Traversée efficace de la grille jusqu'à collision mur
- Calcul de la distance perpendiculaire pour éviter l'effet "fish-eye"
- Détermination du côté de collision (Nord/Sud ou Est/Ouest)
- Protection contre les boucles infinies

### Algorithme DDA
Le **Digital Differential Analyzer** est un algorithme optimisé pour :
- **Traverser une grille** en ligne droite
- **Minimiser les calculs** (pas de trigonométrie)
- **Détecter efficacement** les intersections
- **Éviter les calculs flottants** complexes

### Fonctions principales
```
dda.c
├── dda_step()        (static) - Un pas de l'algorithme DDA
├── check_wall_hit()  (static) - Détection collision/limites
└── perform_dda()     (public) - Algorithme DDA complet
```

### Principe mathématique
```
Au lieu de calculer tous les points du rayon :
→ On avance case par case dans la grille
→ On utilise les distances précalculées (delta_dist)
→ On s'arrête au premier mur rencontré
```

### Dépendances
```c
#include "cub3d.h"
```
- **Initialisation** : `init_ray()` (dans raycasting.c)
- **Rendu** : `draw_walls()` (dans render.c)
- **Map** : `game->map->grid[][]`

---

## 👣 FONCTION DDA_STEP

### Signature
```c
static void dda_step(t_ray *ray)
```

### Vue d'ensemble
Effectue **un pas** de l'algorithme DDA. Détermine dans quelle direction avancer (X ou Y) et met à jour la position du rayon dans la grille.

### Analyse ligne par ligne

```c
static void	dda_step(t_ray *ray)
{
```

```c
	if (ray->side_dist.x < ray->side_dist.y)
	{
```
**Comparaison des distances** :
- `side_dist.x` : Distance jusqu'à la prochaine ligne verticale
- `side_dist.y` : Distance jusqu'à la prochaine ligne horizontale
- **On avance vers la plus proche** (distance minimale)

**Logique** :
```
Si side_dist.x < side_dist.y :
→ La prochaine ligne verticale est plus proche
→ On avance en X (horizontalement)
→ On traverse une ligne verticale

Sinon :
→ La prochaine ligne horizontale est plus proche  
→ On avance en Y (verticalement)
→ On traverse une ligne horizontale
```

```c
		ray->side_dist.x += ray->delta_dist.x;
```
**Mise à jour distance X** :
- Ajoute `delta_dist.x` à `side_dist.x`
- `delta_dist.x` = distance pour traverser une case en X
- **Prépare la prochaine ligne verticale**

**Principe** :
```
Avant : side_dist.x = distance jusqu'à ligne x=N
Après : side_dist.x = distance jusqu'à ligne x=N+1 (ou N-1)
```

```c
		ray->map_x += ray->step_x;
```
**Avancement dans la grille** :
- `step_x` = +1 (droite) ou -1 (gauche)
- `map_x` = nouvelle case X dans la grille
- **Position discrète** mise à jour

```c
		ray->side = 0;
```
**Côté de collision** :
- `side = 0` : Collision avec ligne **verticale** (mur Est/Ouest)
- Utilisé pour le rendu des textures et l'ombrage

```c
	}
	else
	{
		ray->side_dist.y += ray->delta_dist.y;
		ray->map_y += ray->step_y;
		ray->side = 1;
	}
```
**Branche Y** : Logique identique pour l'axe vertical
- `side = 1` : Collision avec ligne **horizontale** (mur Nord/Sud)

```c
}
```

### Exemple d'exécution

```c
État initial :
side_dist.x = 1.2, side_dist.y = 0.8
delta_dist.x = 1.67, delta_dist.y = 1.25
map_x = 2, map_y = 1
step_x = +1, step_y = +1

Étape 1 : side_dist.x (1.2) > side_dist.y (0.8)
→ Avance en Y
→ side_dist.y = 0.8 + 1.25 = 2.05
→ map_y = 1 + 1 = 2
→ side = 1 (ligne horizontale)

État après étape 1 :
side_dist.x = 1.2, side_dist.y = 2.05
map_x = 2, map_y = 2

Étape 2 : side_dist.x (1.2) < side_dist.y (2.05)
→ Avance en X
→ side_dist.x = 1.2 + 1.67 = 2.87
→ map_x = 2 + 1 = 3
→ side = 0 (ligne verticale)
```

---

## 🛡️ FONCTION CHECK_WALL_HIT

### Signature
```c
static int check_wall_hit(t_ray *ray, t_game *game)
```

### Vue d'ensemble
Vérifie si le rayon a **atteint un mur** ou **sortie des limites** de la carte. Fonction de sécurité et de détection de collision.

### Analyse ligne par ligne

```c
static int	check_wall_hit(t_ray *ray, t_game *game)
{
```

### Vérification des limites

```c
	if (ray->map_x < 0 || ray->map_x >= game->map->width
		|| ray->map_y < 0 || ray->map_y >= game->map->height)
	{
```
**Vérification hors-limites** :

**Conditions testées** :
- `map_x < 0` : Sortie par la gauche
- `map_x >= width` : Sortie par la droite  
- `map_y < 0` : Sortie par le haut
- `map_y >= height` : Sortie par le bas

**Pourquoi ces vérifications ?** :
- Évite les accès mémoire invalides
- Protection contre `game->map->grid[y][x]` hors limites
- Sécurité contre les rayons "perdus"

```c
		ray->hit = 1;
		return (1);
	}
```
**Collision détectée** :
- `ray->hit = 1` : Marque collision trouvée
- `return (1)` : Arrête l'algorithme DDA
- **Traitement** : Considéré comme mur invisible aux bords

### Vérification mur

```c
	if (game->map->grid[ray->map_y][ray->map_x] == '1')
	{
```
**Test case grille** :
- `game->map->grid[y][x]` : Accès à la case de grille
- `== '1'` : Caractère mur dans la map
- **Coordonnées** : [y][x] (ligne puis colonne)

**Note importante** : `grid[y][x]` et non `grid[x][y]` !
- Y = ligne (coordonnée verticale)
- X = colonne (coordonnée horizontale)

```c
		ray->hit = 1;
		return (1);
	}
```
**Mur trouvé** :
- Même traitement que hors-limites
- Collision valide détectée

```c
	return (0);
```
**Pas de collision** :
- Case vide ('0', 'N', 'S', 'E', 'W')
- Algorithme DDA continue

```c
}
```

### Valeurs de retour

```c
return (1) : Collision détectée
├── Mur ('1') trouvé
└── Hors limites de la carte

return (0) : Pas de collision
└── Case vide, DDA continue
```

---

## 🚀 FONCTION PERFORM_DDA

### Signature
```c
void perform_dda(t_ray *ray, t_game *game)
```

### Vue d'ensemble
Fonction **publique principale** qui exécute l'algorithme DDA complet. Traverse la grille jusqu'à trouver un mur et calcule la distance perpendiculaire finale.

### Analyse ligne par ligne

```c
void	perform_dda(t_ray *ray, t_game *game)
{
```

### Protection boucle infinie

```c
	int	max_steps;

	max_steps = 0;
```
**Compteur de sécurité** :
- Évite les boucles infinies
- Protection contre les bugs de logique
- Limite raisonnable basée sur la taille de la map

```c
	while (ray->hit == 0 && max_steps < game->map->width + game->map->height)
	{
```
**Boucle principale DDA** :

**Conditions d'arrêt** :
1. `ray->hit == 0` : Pas encore de collision
2. `max_steps < width + height` : Limite sécurité

**Limite `width + height`** :
- Distance maximale possible dans la grille
- Traverse toute la diagonale + marge
- Exemple : map 10×8 → limite 18 pas

```c
		max_steps++;
```
**Incrément sécurité** : Compte les étapes pour éviter l'infini

```c
		dda_step(ray);
```
**Étape DDA** :
- Avance d'une case dans la grille
- Met à jour `map_x/y`, `side_dist`, `side`
- Détermine direction optimale (X ou Y)

```c
		if (check_wall_hit(ray, game))
			break ;
```
**Test collision** :
- Vérifie mur ou hors-limites
- `break` : Sort de la boucle si collision
- `ray->hit` mis à 1 par `check_wall_hit()`

```c
	}
```

### Calcul distance perpendiculaire

```c
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player->pos.x
				+ (1 - ray->step_x) / 2) / ray->dir.x;
```
**Distance perpendiculaire côté X** :

**Formule** : `perp_wall_dist = (map_x - pos.x + (1 - step_x)/2) / dir.x`

**Décomposition** :
- `map_x - pos.x` : Distance entre joueur et case mur
- `(1 - step_x) / 2` : Correction pour le bord de case
- `/ dir.x` : Projection perpendiculaire

**Correction de bord** :
```c
step_x = +1 (droite) → (1-1)/2 = 0 → Bord gauche de la case
step_x = -1 (gauche) → (1-(-1))/2 = 1 → Bord droit de la case
```

**Pourquoi perpendiculaire ?** :
- Évite l'effet "fish-eye" (distorsion visuelle)
- Distance réelle vs distance projetée
- Rendu 3D correct

```c
	else
		ray->perp_wall_dist = (ray->map_y - game->player->pos.y
				+ (1 - ray->step_y) / 2) / ray->dir.y;
```
**Distance perpendiculaire côté Y** : Même logique pour l'axe vertical

### Protection distance nulle

```c
	if (ray->perp_wall_dist <= 0)
		ray->perp_wall_dist = 0.1;
```
**Sécurité division par zéro** :
- Distance négative ou nulle → 0.1
- Évite les erreurs de rendu
- Cas rare mais possible avec erreurs flottantes

```c
}
```

### Exemple complet d'exécution

```c
État initial (après init_ray) :
map_x=2, map_y=1, hit=0
side_dist.x=1.2, side_dist.y=0.8
delta_dist.x=1.67, delta_dist.y=1.25
step_x=+1, step_y=+1

Boucle DDA :

Étape 1 : max_steps=1
→ dda_step() : side_dist.y plus petit
  → map_y=2, side_dist.y=2.05, side=1
→ check_wall_hit() : grid[2][2]='0' → return 0

Étape 2 : max_steps=2  
→ dda_step() : side_dist.x plus petit
  → map_x=3, side_dist.x=2.87, side=0
→ check_wall_hit() : grid[2][3]='1' → hit=1, return 1, break

Calcul distance :
side=0 (côté X), step_x=+1
perp_wall_dist = (3 - 2.3 + (1-1)/2) / 0.6 = 0.7 / 0.6 = 1.17

Résultat : Mur trouvé à distance 1.17, côté vertical (side=0)
```

---

## ❓ QUESTIONS FRÉQUENTES

### Q1 : Qu'est-ce que l'algorithme DDA ?
**R :**
```
Digital Differential Analyzer - algorithme pour traverser une grille :

Avantages :
- Pas de trigonométrie (cos, sin)
- Calculs entiers principalement  
- Très rapide et efficace
- Traversée optimale case par case

Principe : Au lieu de calculer tous les points du rayon,
on avance case par case jusqu'au premier obstacle.
```

### Q2 : Pourquoi comparer side_dist.x et side_dist.y ?
**R :**
```c
side_dist.x = distance jusqu'à prochaine ligne verticale
side_dist.y = distance jusqu'à prochaine ligne horizontale

On avance vers la plus proche :
→ Garantit de ne manquer aucune intersection
→ Traverse la grille efficacement
→ Trouve le chemin le plus court
```

### Q3 : Que représente ray->side ?
**R :**
```c
side = 0 : Collision ligne verticale (mur Est/Ouest)
side = 1 : Collision ligne horizontale (mur Nord/Sud)

Utilisé pour :
- Choix de texture (mur vertical vs horizontal)
- Calcul d'ombrage (Nord/Sud plus sombres)
- Calcul distance perpendiculaire
```

### Q4 : Pourquoi la limite max_steps ?
**R :**
```c
Protection contre boucles infinies :

Causes possibles :
- Bug dans step_x/step_y
- Erreurs directions delta_dist
- Corruption données ray

Limite width + height :
→ Distance maximale possible dans grille
→ Permet traversée complète diagonale
```

### Q5 : Comment fonctionne la distance perpendiculaire ?
**R :**
```c
Distance perpendiculaire ≠ Distance euclidienne

Distance euclidienne = sqrt((x1-x2)² + (y1-y2)²)
Distance perpendiculaire = projection sur direction regard

Évite "fish-eye effect" :
- Bords écran semblent plus loin
- Distorsion visuelle corrigée
- Rendu 3D réaliste
```

### Q6 : Pourquoi (1 - step_x) / 2 ?
**R :**
```c
Correction pour le bord exact de la case :

step_x = +1 (va droite) → (1-1)/2 = 0 → bord gauche case
step_x = -1 (va gauche) → (1-(-1))/2 = 1 → bord droit case

Assure que la distance est calculée jusqu'au bord exact
du mur, pas jusqu'au centre de la case.
```

### Q7 : Que se passe si ray->perp_wall_dist <= 0 ?
**R :**
```c
Cas possible avec erreurs de calcul flottant :
- Ray->dir très proche de 0
- Erreurs d'arrondi
- Position joueur exactement sur mur

Solution : perp_wall_dist = 0.1
→ Évite division par zéro dans le rendu
→ Mur très proche mais visible
```

### Q8 : Comment éviter grid[y][x] hors limites ?
**R :**
```c
Vérifications dans check_wall_hit() :

map_x < 0 || map_x >= width  (hors limites X)
map_y < 0 || map_y >= height (hors limites Y)

Protection contre :
- Segmentation fault
- Accès mémoire invalide
- Rayons "perdus" dans le vide
```

### Q9 : Pourquoi grid[y][x] et pas grid[x][y] ?
**R :**
```c
Convention tableau 2D en C :

array[ligne][colonne] = array[y][x]

Y = coordonnée verticale = ligne
X = coordonnée horizontale = colonne

Erreur fréquente ! Toujours vérifier l'ordre.
```

### Q10 : Comment débugger l'algorithme DDA ?
**R :**
```c
Ajouts temporaires dans perform_dda() :

printf("Step %d: map(%d,%d), side_dist(%.2f,%.2f), side=%d\n",
       max_steps, ray->map_x, ray->map_y, 
       ray->side_dist.x, ray->side_dist.y, ray->side);

Permet de visualiser :
- Progression case par case
- Distances à chaque étape
- Côté de collision final
```

---

## 📊 DIAGRAMMES

### Algorithme DDA complet

```
perform_dda(ray, game)
      │
      ├─→ max_steps = 0
      │
      ├─→ WHILE (ray->hit == 0 && max_steps < limit):
      │    │
      │    ├─→ max_steps++
      │    │
      │    ├─→ dda_step(ray)
      │    │    ├─→ Compare side_dist.x vs side_dist.y
      │    │    ├─→ Avance dans direction minimum
      │    │    ├─→ Met à jour map_x/y et side_dist
      │    │    └─→ Détermine ray->side (0 ou 1)
      │    │
      │    ├─→ check_wall_hit(ray, game)
      │    │    ├─→ Vérifie hors limites
      │    │    ├─→ Vérifie grid[y][x] == '1'
      │    │    └─→ Set ray->hit=1 si collision
      │    │
      │    └─→ if (collision) break
      │
      ├─→ Calcul perp_wall_dist selon ray->side
      │
      └─→ Protection perp_wall_dist > 0
```

### Progression DDA dans la grille

```
Exemple : Joueur (1.3, 1.8), direction (0.8, 0.6)

Initial: map_x=1, map_y=1
     0   1   2   3   4
   ┌───┬───┬───┬───┬───┐
 0 │   │   │   │   │   │
   ├───┼───┼───┼───┼───┤
 1 │   │ P │ 2 │ 4 │   │  P=start, nombres=ordre DDA
   ├───┼───┼───┼───┼───┤
 2 │   │ 1 │ 3 │ ■ │   │  ■=mur trouvé
   └───┴───┴───┴───┴───┘

Étapes DDA :
1. side_dist.y < side_dist.x → avance Y → (1,2)
2. side_dist.x < side_dist.y → avance X → (2,2)  
3. side_dist.y < side_dist.x → avance Y → (2,1)
4. side_dist.x < side_dist.y → avance X → (3,1)
5. side_dist.y < side_dist.x → avance Y → (3,2) → MUR!
```

### Calcul distance perpendiculaire

```
Vue de dessus - Distance perpendiculaire vs euclidienne :

         Mur
         │
    P ──→│ A   Distance euclidienne PA = √((x₁-x₂)² + (y₁-y₂)²)
     \   │     Distance perpendiculaire PB = PA × cos(angle)
      \  │
       \ │
        \│ B
         
P = Joueur, A = Point impact rayon, B = Projection perpendiculaire

Pourquoi perpendiculaire ?
→ Évite l'effet "fish-eye"
→ Murs droits restent droits
→ Distances uniformes sur l'écran
```

### Comparaison side_dist

```
Situation : Rayon dans case (2,1)

     1   2   3   4
   ┌───┬───┬───┬───┐
 0 │   │   │   │   │
   ├───┼───┼───┼───┤
 1 │   │ P→│   │   │  P = position rayon
   ├───┼───┼───┼───┤  → = direction
 2 │   │   │   │   │
   └───┴───┴───┴───┘

side_dist.x = distance jusqu'à ligne x=3 (verticale)
side_dist.y = distance jusqu'à ligne y=0 ou y=2 (horizontale)

Si side_dist.x < side_dist.y :
→ Ligne verticale plus proche
→ dda_step() avance en X
→ Nouvelle position (3,1)
→ ray->side = 0 (côté vertical)
```

### Protection hors limites

```
Grille 4×3 avec vérifications :

     0   1   2   3   (4)
   ┌───┬───┬───┬───┐ ↑ 
 0 │   │   │   │   │ │ map_x >= width
   ├───┼───┼───┼───┤ │
 1 │   │ P │   │   │ │
   ├───┼───┼───┼───┤ │
 2 │   │   │   │   │ │
   └───┴───┴───┴───┘ │
(3) ←───────────────┘
   map_y >= height

Protections check_wall_hit() :
- map_x < 0 ou map_x >= 4 → collision
- map_y < 0 ou map_y >= 3 → collision
- grid[map_y][map_x] == '1' → collision
```

### Effet fish-eye corrigé

```
Sans correction (distance euclidienne) :
┌─────────────────────────────────────┐
│                                     │ ← Murs courbés
│    ╱                           ╲    │   (effet fish-eye)
│   ╱                             ╲   │
│  ╱               P               ╲  │
│ ╱                                 ╲ │
└───────────────────────────────────────┘

Avec correction (distance perpendiculaire) :
┌─────────────────────────────────────┐
│                                     │ ← Murs droits
│ │                               │ │ │   (rendu correct)
│ │                               │ │ │
│ │               P               │ │ │
│ │                               │ │ │
└─┴───────────────────────────────────┴─┘
```

### Performance et sécurité

```
Optimisations DDA :

1. Pré-calculs (delta_dist) :
   ✅ Calculé une fois dans init_ray()
   ✅ Réutilisé à chaque dda_step()
   
2. Comparaisons simples :
   ✅ if (side_dist.x < side_dist.y)
   ✅ Pas de sqrt() ou trigonométrie
   
3. Traversée optimale :
   ✅ Une case par étape minimum
   ✅ Aucune intersection manquée

Sécurités :
- max_steps contre boucles infinies
- Vérification hors limites  
- Protection perp_wall_dist <= 0
```

### Correspondance ray->side et textures

```
Vue de dessus des collisions :

    Nord (side=1)
        ↑
        │
Ouest ──┼── Est  
(side=0)│(side=0)
        │
        ↓
    Sud (side=1)

side = 0 : Collision ligne verticale (murs Est/Ouest)
side = 1 : Collision ligne horizontale (murs Nord/Sud)

Utilisation :
- Choix texture selon orientation mur
- Calcul ombrage (Nord/Sud plus sombres)
- Calcul correct distance perpendiculaire
```

---

## 🎓 CHECKLIST POUR LA CORRECTION

### Comprendre dda_step()
- [ ] Expliquer pourquoi comparer side_dist.x et side_dist.y
- [ ] Comprendre la mise à jour side_dist += delta_dist
- [ ] Identifier le rôle de step_x/step_y (+1/-1)
- [ ] Expliquer ray->side (0=vertical, 1=horizontal)

### Comprendre check_wall_hit()
- [ ] Expliquer les vérifications hors limites
- [ ] Comprendre grid[y][x] vs grid[x][y]
- [ ] Identifier les valeurs de retour (0/1)
- [ ] Expliquer ray->hit comme flag d'arrêt

### Comprendre perform_dda()
- [ ] Expliquer la protection max_steps
- [ ] Comprendre la boucle while avec deux conditions
- [ ] Identifier le calcul distance perpendiculaire
- [ ] Expliquer la correction (1-step)/2

### Algorithme DDA général
- [ ] Expliquer l'avantage vs calcul point par point
- [ ] Comprendre la traversée case par case
- [ ] Identifier l'efficacité (pas de trigonométrie)
- [ ] Expliquer la garantie de trouver toute intersection

### Distance perpendiculaire
- [ ] Différencier distance euclidienne vs perpendiculaire
- [ ] Expliquer l'effet fish-eye et sa correction
- [ ] Comprendre la projection sur direction regard
- [ ] Identifier l'importance pour le rendu 3D

### Questions pièges possibles
- [ ] "Pourquoi pas grid[x][y] ?"
  → Convention C : array[ligne][colonne] = array[y][x]
  
- [ ] "Comment éviter les boucles infinies ?"
  → max_steps < width + height (limite sécurité)
  
- [ ] "Que se passe si perp_wall_dist = 0 ?"
  → Protection perp_wall_dist = 0.1 (évite division par zéro)
  
- [ ] "Comment optimiser l'algorithme ?"
  → Pré-calculs delta_dist, comparaisons simples, pas de sqrt()
  
- [ ] "Pourquoi side=0 vs side=1 ?"
  → 0=vertical (Est/Ouest), 1=horizontal (Nord/Sud)

### Concepts clés à maîtriser
- [ ] **DDA** : Traversée grille sans trigonométrie
- [ ] **side_dist** : Distances aux prochaines lignes grille
- [ ] **step** : Directions d'avancement (-1/+1)
- [ ] **side** : Type de collision (vertical/horizontal)
- [ ] **Distance perpendiculaire** : Correction fish-eye

---

## 🎯 FIN DE L'EXPLICATION

Ce fichier couvre **100% du code** de `dda.c` avec des explications détaillées adaptées pour une correction de projet 42.

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
10. ✅ `RAYCASTING_EXPLICATION.md` - Explication de raycasting.c
11. ✅ `DDA_EXPLICATION.md` - Explication de dda.c (ce fichier)

**Cœur algorithme DDA documenté** :
- Traversée efficace de grille
- Détection collision optimisée  
- Calcul distance perpendiculaire
- Protection contre erreurs

**Pipeline complet maintenant documenté** :
Input → Movement → **Raycasting** → **DDA** → Rendering

**Prochaines étapes suggérées** :
- Visualiser la progression case par case
- Comprendre l'optimisation vs calcul brute-force
- Maîtriser la correction effet fish-eye

Algorithme fondamental du moteur 3D expliqué ! 🎯