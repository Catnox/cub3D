# RENDER.C - EXPLICATION DÉTAILLÉE

## 📋 TABLE DES MATIÈRES

1. [Vue d'ensemble](#vue-densemble)
2. [Fonction render_frame](#fonction-render_frame)
3. [Fonction draw_floor_ceiling](#fonction-draw_floor_ceiling)
4. [Fonction calculate_draw_bounds](#fonction-calculate_draw_bounds)
5. [Fonction get_wall_texture](#fonction-get_wall_texture)
6. [Fonction draw_walls](#fonction-draw_walls)
7. [Questions fréquentes](#questions-fréquentes)
8. [Diagrammes](#diagrammes)

---

## 🎯 VUE D'ENSEMBLE

### Rôle du fichier
`render.c` gère le **rendu graphique principal** de cub3D :
- Coordination du rendu complet d'une frame
- Dessin du sol et du plafond
- Calcul des dimensions des murs
- Sélection des textures selon l'orientation
- Orchestration du dessin des murs

### Fonctions principales
```
render.c
├── render_frame()            - Coordinateur principal du rendu
├── draw_floor_ceiling()      - Sol et plafond colorés
├── calculate_draw_bounds()   - Calcul hauteur/limites des murs
├── get_wall_texture()        - Sélection texture selon orientation
└── draw_walls()              - Coordination du dessin des murs
```

### Dépendances
```c
#include "cub3d.h"
```
- **MLX42** : `mlx_put_pixel()`, `mlx_texture_t`
- **Raycasting** : Structure `t_ray`
- **Wall rendering** : `draw_textured_wall()`, `draw_colored_wall()`
- **Libft** : `ft_memset()`

---

## 🖼️ FONCTION RENDER_FRAME

### Signature
```c
void render_frame(t_game *game)
```

### Vue d'ensemble
Fonction **maître** qui orchestre le rendu complet d'une frame. Elle coordonne toutes les étapes du rendu dans l'ordre correct.

### Analyse ligne par ligne

```c
void	render_frame(t_game *game)
{
```
**Point d'entrée principal** :
- Appelée depuis `game_update()` dans game_loop.c
- Fréquence : ~60 FPS
- Dessine une frame complète

```c
	ft_memset(game->img->pixels, 0,
		game->img->width * game->img->height * sizeof(int));
```
**Nettoyage du buffer d'image** :

**`ft_memset()`** : Fonction libft pour remplir la mémoire
- `game->img->pixels` : Buffer de l'image MLX42
- `0` : Valeur de remplissage (noir, RGBA = 0x00000000)
- Taille : `largeur × hauteur × 4 bytes` (32 bits par pixel)

**Pourquoi nettoyer ?** :
- Efface la frame précédente
- Évite les artefacts visuels
- Prépare un canvas propre

**Format de pixels** :
```c
sizeof(int) = 4 bytes = 32 bits
Format RGBA : [R][G][B][A] (8 bits chacun)
Valeur 0 = 0x00000000 = noir transparent
```

```c
	draw_floor_ceiling(game);
```
**Dessin sol/plafond** :
- Remplit la moitié supérieure (plafond)
- Remplit la moitié inférieure (sol)
- Couleurs définies dans le fichier .cub

**Ordre important** : Avant les murs pour que les murs les recouvrent

```c
	cast_rays(game);
```
**Raycasting principal** :
- Fonction dans `raycasting.c`
- Lance un rayon pour chaque colonne de pixels (x = 0 à WINDOW_WIDTH-1)
- Pour chaque rayon, appelle `draw_walls()` pour dessiner la colonne

```c
}
```

### Ordre de rendu

```
1. Nettoyer buffer     → Canvas noir
2. Sol/plafond         → Arrière-plan coloré
3. Raycasting + murs   → Premier plan avec profondeur
```

**Pourquoi cet ordre ?** :
- Sol/plafond en premier = arrière-plan
- Murs par-dessus = premier plan
- Pas besoin de Z-buffer (ordre de dessin suffit)

---

## 🌈 FONCTION DRAW_FLOOR_CEILING

### Signature
```c
void draw_floor_ceiling(t_game *game)
```

### Vue d'ensemble
Dessine l'arrière-plan en remplissant :
- **Moitié supérieure** = plafond (couleur ceiling)
- **Moitié inférieure** = sol (couleur floor)

### Analyse ligne par ligne

```c
void	draw_floor_ceiling(t_game *game)
{
```

```c
	int	x;
	int	y;
	int	floor_color;
	int	ceiling_color;
```
**Variables locales** :
- `x, y` : Coordonnées de pixel en cours
- `floor_color, ceiling_color` : Couleurs converties au format MLX42

```c
	floor_color = (game->map->floor_color.r << 24)
		| (game->map->floor_color.g << 16)
		| (game->map->floor_color.b << 8) | game->map->floor_color.a;
```
**Conversion couleur sol** :

**Format MLX42** : 32 bits RGBA
```
Bit 31-24: Rouge   (R << 24)
Bit 23-16: Vert    (G << 16)
Bit 15-8:  Bleu    (B << 8)
Bit 7-0:   Alpha   (A << 0)
```

**Opérateurs bit à bit** :
- `<<` : Décalage à gauche (multiplication par 2^n)
- `|` : OU logique (combine les composantes)

**Exemple** :
```c
R=255, G=128, B=64, A=255

floor_color = (255 << 24) | (128 << 16) | (64 << 8) | 255
            = 0xFF000000 | 0x00800000 | 0x00004000 | 0x000000FF
            = 0xFF8040FF
```

```c
	ceiling_color = (game->map->ceiling_color.r << 24)
		| (game->map->ceiling_color.g << 16)
		| (game->map->ceiling_color.b << 8) | game->map->ceiling_color.a;
```
**Conversion couleur plafond** : Identique au sol

```c
	y = 0;
	while (y < (int)game->img->height)
	{
```
**Boucle verticale** :
- Parcourt toutes les lignes de pixels
- De haut (y=0) vers bas (y=hauteur-1)

```c
		x = 0;
		while (x < (int)game->img->width)
		{
```
**Boucle horizontale** :
- Parcourt toutes les colonnes de pixels
- De gauche (x=0) vers droite (x=largeur-1)

```c
			if (y < (int)game->img->height / 2)
				mlx_put_pixel(game->img, x, y, ceiling_color);
			else
				mlx_put_pixel(game->img, x, y, floor_color);
```
**Choix de couleur** :

**Condition** : `y < height / 2`
- **Moitié supérieure** (y < height/2) : Plafond
- **Moitié inférieure** (y ≥ height/2) : Sol

**mlx_put_pixel()** :
- Fonction MLX42 pour dessiner un pixel
- Paramètres : image, x, y, couleur
- Écrit directement dans le buffer

**Exemple avec hauteur 768** :
```
y = 0 à 383   → Plafond
y = 384 à 767 → Sol
```

```c
			x++;
		}
		y++;
	}
```
**Incrémentation** : Passe au pixel suivant

### Complexité

**Pixels dessinés** : `largeur × hauteur = 1024 × 768 = 786,432 pixels`
**Complexité** : O(largeur × hauteur) = O(n²)

À 60 FPS : `786,432 × 60 = 47,185,920 pixels/seconde` !

---

## 📏 FONCTION CALCULATE_DRAW_BOUNDS

### Signature
```c
void calculate_draw_bounds(t_ray *ray, t_draw_bounds_params *params, t_game *game)
```

### Vue d'ensemble
Calcule les **limites de dessin** d'une colonne de mur :
- Hauteur du mur à l'écran
- Pixel de début (draw_start)
- Pixel de fin (draw_end)
- Gestion des débordements

### Analyse ligne par ligne

```c
void	calculate_draw_bounds(t_ray *ray, t_draw_bounds_params *params,
		t_game *game)
{
```
**Paramètres** :
- `ray` : Résultat du raycasting (distance, etc.)
- `params` : Structure avec pointeurs vers les résultats
- `game` : Contexte global

**Structure t_draw_bounds_params** :
```c
typedef struct s_draw_bounds_params
{
    int *line_height;  // Pointeur vers hauteur calculée
    int *draw_start;   // Pointeur vers début
    int *draw_end;     // Pointeur vers fin
} t_draw_bounds_params;
```

```c
	int	window_height;

	window_height = (int)game->img->height;
```
**Hauteur fenêtre** : Cast de uint32_t vers int pour les calculs

```c
	*(params->line_height) = (int)(window_height / ray->perp_wall_dist);
```
**Calcul hauteur du mur** :

**Formule fondamentale** :
```
hauteur_écran = hauteur_fenêtre / distance_perpendiculaire

Plus le mur est proche → plus il paraît grand
Plus le mur est loin   → plus il paraît petit
```

**Exemple** :
```c
window_height = 768
ray->perp_wall_dist = 2.0

line_height = 768 / 2.0 = 384 pixels
```

**Pourquoi perpendiculaire ?** :
- Évite l'effet "fish-eye" (déformation circulaire)
- Distance corrigée par rapport au plan de projection

```c
	*(params->draw_start) = -*(params->line_height) / 2 + window_height / 2;
```
**Calcul début de dessin** :

**Logique** :
```
Centre écran = window_height / 2 = 384
Moitié mur   = line_height / 2

draw_start = centre - moitié_mur
           = 384 - line_height/2
```

**Pourquoi négatif ?** :
```c
-line_height / 2 + window_height / 2
= window_height/2 - line_height/2
```

**Exemple** :
```c
line_height = 200
window_height = 768

draw_start = -200/2 + 768/2 = -100 + 384 = 284
draw_end   = 200/2 + 768/2  = 100 + 384 = 484

Le mur va du pixel 284 au pixel 484 (200 pixels de haut)
```

```c
	if (*(params->draw_start) < 0)
		*(params->draw_start) = 0;
```
**Clamp début** :
- Si le mur dépasse vers le haut → commencer à 0
- Évite les accès mémoire invalides
- Cas : mur très proche (très grand)

```c
	*(params->draw_end) = *(params->line_height) / 2 + window_height / 2;
```
**Calcul fin de dessin** :
```
draw_end = centre + moitié_mur
         = window_height/2 + line_height/2
```

```c
	if (*(params->draw_end) >= window_height)
		*(params->draw_end) = window_height - 1;
```
**Clamp fin** :
- Si le mur dépasse vers le bas → finir à height-1
- Évite les accès mémoire invalides
- height-1 car indexation de 0 à height-1

```c
}
```

### Exemples concrets

**Mur proche (distance = 0.5)** :
```c
line_height = 768 / 0.5 = 1536 pixels (très grand)
draw_start = 384 - 768 = -384 → clampé à 0
draw_end = 384 + 768 = 1152 → clampé à 767
Résultat : Mur remplit tout l'écran
```

**Mur moyen (distance = 2.0)** :
```c
line_height = 768 / 2.0 = 384 pixels
draw_start = 384 - 192 = 192
draw_end = 384 + 192 = 576
Résultat : Mur du pixel 192 au 576
```

**Mur loin (distance = 10.0)** :
```c
line_height = 768 / 10.0 = 76.8 → 76 pixels
draw_start = 384 - 38 = 346
draw_end = 384 + 38 = 422
Résultat : Petit mur au centre
```

---

## 🧱 FONCTION GET_WALL_TEXTURE

### Signature
```c
mlx_texture_t *get_wall_texture(t_game *game, t_ray *ray)
```

### Vue d'ensemble
Détermine quelle texture utiliser selon :
- L'orientation du mur (N/S/E/W)
- La direction du rayon
- Le côté touché

### Analyse ligne par ligne

```c
mlx_texture_t	*get_wall_texture(t_game *game, t_ray *ray)
{
```
**Retour** : Pointeur vers texture MLX42

```c
	if (ray->side == 0)
	{
```
**Mur vertical** :
- `side == 0` : Mur vertical (perpendiculaire à l'axe X)
- Le rayon a touché un mur "debout" (nord-sud)

```c
		if (ray->step_x > 0)
			return (game->map->textures.east);
		else
			return (game->map->textures.west);
```
**Choix texture verticale** :

**`ray->step_x > 0`** :
- Rayon va vers la droite (+X)
- Touche le côté **ouest** du mur
- Mais on voit la face **est** → texture EST

**`ray->step_x ≤ 0`** :
- Rayon va vers la gauche (-X)
- Touche le côté **est** du mur
- Mais on voit la face **ouest** → texture OUEST

```c
	}
	else
	{
```
**Mur horizontal** :
- `side == 1` : Mur horizontal (perpendiculaire à l'axe Y)
- Le rayon a touché un mur "couché" (est-ouest)

```c
		if (ray->step_y > 0)
			return (game->map->textures.south);
		else
			return (game->map->textures.north);
```
**Choix texture horizontale** :

**`ray->step_y > 0`** :
- Rayon va vers le bas (+Y)
- Touche le côté **nord** du mur
- Mais on voit la face **sud** → texture SUD

**`ray->step_y ≤ 0`** :
- Rayon va vers le haut (-Y)
- Touche le côté **sud** du mur
- Mais on voit la face **nord** → texture NORD

```c
	}
}
```

### Logique de sélection

```
Vue de dessus de la map :

    ┌─────┐
    │  N  │ ← Texture NORD
    │W   E│ ← Textures OUEST et EST
    │  S  │ ← Texture SUD
    └─────┘

Rayon depuis P vers mur :

P ──→ │ mur │   step_x > 0, side = 0 → texture EST
      │     │   (on voit la face est du mur)

P     │ mur │
  ←── │     │   step_x < 0, side = 0 → texture OUEST
              (on voit la face ouest du mur)

P             step_y > 0, side = 1 → texture SUD
│             (on voit la face sud du mur)
↓
─────

↑
│             step_y < 0, side = 1 → texture NORD
P             (on voit la face nord du mur)
─────
```

### Variables t_ray importantes

```c
typedef struct s_ray
{
    // ...
    int side;     // 0 = mur vertical, 1 = mur horizontal
    int step_x;   // Direction X du rayon (-1 ou +1)
    int step_y;   // Direction Y du rayon (-1 ou +1)
    // ...
} t_ray;
```

---

## 🎨 FONCTION DRAW_WALLS

### Signature
```c
void draw_walls(t_game *game, int x, t_ray *ray)
```

### Vue d'ensemble
Fonction **coordinatrice** qui orchestre le dessin d'une colonne de mur :
1. Calcule les limites de dessin
2. Sélectionne la texture appropriée
3. Prépare les paramètres
4. Délègue au bon système de dessin

### Analyse ligne par ligne

```c
void	draw_walls(t_game *game, int x, t_ray *ray)
{
```
**Paramètres** :
- `game` : Contexte global
- `x` : Colonne de pixels à dessiner (0 à WINDOW_WIDTH-1)
- `ray` : Résultat du raycasting pour cette colonne

```c
	int					line_height;
	int					draw_start;
	int					draw_end;
	mlx_texture_t		*texture;
	t_wall_draw_params	params;
```
**Variables locales** :
- `line_height, draw_start, draw_end` : Limites calculées
- `texture` : Texture sélectionnée
- `params` : Structure pour passer les paramètres

```c
	calculate_draw_bounds(ray, &(t_draw_bounds_params){&line_height,
		&draw_start, &draw_end}, game);
```
**Calcul des limites** :

**Syntaxe spéciale** : Initialisation composée
```c
&(t_draw_bounds_params){&line_height, &draw_start, &draw_end}
```
- Crée une structure temporaire
- Remplit avec les adresses des variables locales
- Passe l'adresse de cette structure

**Équivalent verbeux** :
```c
t_draw_bounds_params bounds_params;
bounds_params.line_height = &line_height;
bounds_params.draw_start = &draw_start;
bounds_params.draw_end = &draw_end;
calculate_draw_bounds(ray, &bounds_params, game);
```

```c
	texture = get_wall_texture(game, ray);
```
**Sélection texture** : Selon orientation et direction du rayon

```c
	params.x = x;
	params.line_height = line_height;
	params.draw_start = draw_start;
	params.draw_end = draw_end;
	params.texture = texture;
```
**Préparation paramètres** :
- Regroupe toutes les infos nécessaires
- Structure passée aux fonctions de dessin

```c
	if (!texture)
	{
		draw_colored_wall(game, ray, &params);
		return ;
	}
```
**Gestion absence de texture** :
- Si texture NULL → dessin coloré simple
- Fallback de sécurité
- `return` pour éviter le dessin texturé

```c
	draw_textured_wall(game, ray, &params);
```
**Dessin texturé** :
- Fonction dans `wall_utils.c`
- Applique la texture sur la colonne
- Gère le mapping texture → écran

```c
}
```

### Flux de données

```
draw_walls()
     │
     ├─→ calculate_draw_bounds()   → line_height, draw_start, draw_end
     │
     ├─→ get_wall_texture()        → texture
     │
     ├─→ Préparation params        → Regroupement des données
     │
     └─→ draw_textured_wall()      → Dessin final
         OU
         draw_colored_wall()       → Fallback
```

---

## ❓ QUESTIONS FRÉQUENTES

### Q1 : Pourquoi nettoyer l'image à chaque frame ?
**R :**
```c
ft_memset(game->img->pixels, 0, size);
```
- Efface les artefacts de la frame précédente
- Évite les pixels "fantômes"
- Prépare un canvas propre

### Q2 : Pourquoi dessiner sol/plafond avant les murs ?
**R :**
- Sol/plafond = arrière-plan
- Murs = premier plan
- Ordre de dessin = Z-buffer simple
- Murs recouvrent naturellement sol/plafond

### Q3 : Comment fonctionne la conversion de couleur ?
**R :**
```c
color = (R << 24) | (G << 16) | (B << 8) | A;

Format 32 bits : [R][G][B][A]
R à gauche (bits 31-24)
A à droite (bits 7-0)
```

### Q4 : Pourquoi utiliser perp_wall_dist ?
**R :**
```c
line_height = window_height / ray->perp_wall_dist;
```
- Distance perpendiculaire évite l'effet fish-eye
- Distance "corrigée" par rapport au plan de projection
- Plus réaliste qu'une distance euclidienne simple

### Q5 : Comment fonctionne le clamp des limites ?
**R :**
```c
if (draw_start < 0) draw_start = 0;
if (draw_end >= height) draw_end = height - 1;
```
- Évite les accès mémoire invalides
- Gère les murs très proches (débordent)
- Sécurité du buffer d'image

### Q6 : Pourquoi step_x/step_y pour choisir la texture ?
**R :**
- `step_x/y` indique la direction du rayon
- Permet de savoir quel côté du mur on voit
- Exemple : step_x > 0 → on voit la face EST

### Q7 : Que se passe-t-il si texture est NULL ?
**R :**
```c
if (!texture) {
    draw_colored_wall();  // Fallback coloré
    return;
}
draw_textured_wall();     // Normal
```
- Système de fallback robuste
- Évite les crashes
- Debug visuel (mur coloré différent)

### Q8 : Combien de pixels sont dessinés par frame ?
**R :**
```
Sol/plafond : 1024 × 768 = 786,432 pixels
Murs        : Variable (selon géométrie)
Total       : ~800,000 pixels/frame à 60 FPS
            = ~48 millions pixels/seconde
```

### Q9 : Pourquoi séparer draw_textured_wall et draw_colored_wall ?
**R :**
- **Textured** : Complex mapping texture → pixel
- **Colored** : Simple remplissage couleur unie
- Performances : évite les tests dans la boucle interne
- Maintenabilité : logiques séparées

### Q10 : Comment éviter les divisions par zéro ?
**R :**
```c
// Dans le raycasting, perp_wall_dist est toujours > 0
// Car on trouve forcément un mur à distance finie
// Mais sécurité possible :
if (ray->perp_wall_dist < 0.001)
    ray->perp_wall_dist = 0.001;
```

---

## 📊 DIAGRAMMES

### Flux de rendu complet

```
render_frame()
      │
      ├─→ [1] ft_memset()           → Buffer noir
      │
      ├─→ [2] draw_floor_ceiling()  → Arrière-plan coloré
      │        │
      │        ├─→ Conversion couleurs RGBA
      │        ├─→ Boucle Y (lignes)
      │        └─→ Boucle X (colonnes)
      │             └─→ mlx_put_pixel()
      │
      └─→ [3] cast_rays()           → Premier plan 3D
               │
               ├─→ Pour X = 0 à WIDTH-1
               │    ├─→ Calcul rayon
               │    ├─→ DDA (collision)
               │    └─→ draw_walls()
               │         │
               │         ├─→ calculate_draw_bounds()
               │         ├─→ get_wall_texture()
               │         └─→ draw_textured_wall()
               │
               └─→ Image complète
```

### Structure d'une frame

```
    0                                     1024
0   ┌─────────────────────────────────────┐
    │                                     │
    │            PLAFOND                  │ ← draw_floor_ceiling()
    │         (ceiling_color)             │
384 ├─────────────────────────────────────┤ ← height / 2
    │                                     │
    │              SOL                    │ ← draw_floor_ceiling()
    │          (floor_color)              │
768 └─────────────────────────────────────┘

Puis pour chaque colonne X :
    │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│
    │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│ ← Plafond
    │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│
    │██████████████████████████████████│ ← Mur (draw_walls)
    │██████████████████████████████████│
    │██████████████████████████████████│
    │▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓│ ← Sol
    │▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓│
```

### Calcul des limites de mur

```
Hauteur fenêtre = 768

Distance mur = 2.0
line_height = 768 / 2.0 = 384

Centre écran = 768 / 2 = 384

draw_start = 384 - 384/2 = 384 - 192 = 192
draw_end   = 384 + 384/2 = 384 + 192 = 576

    0   ┌─────────────────┐
        │                 │
        │   PLAFOND       │
    192 ├─────────────────┤ ← draw_start
        │█████████████████│
        │█████████████████│ ← MUR (384 pixels)
        │█████████████████│
    576 ├─────────────────┤ ← draw_end
        │                 │
        │     SOL         │
    768 └─────────────────┘
```

### Sélection de texture selon orientation

```
Vue de dessus de la map avec joueur (P) :

          NORD (texture north)
              ↑
              │
    ┌─────────┼─────────┐
    │         │         │
OUEST│    ┌────┼────┐    │EST
(west│    │    │    │    │east)
    │    │    P    │    │
    │    │    │    │    │
    │    └────┼────┘    │
    │         │         │
    └─────────┼─────────┘
              │
              ↓
            SUD (texture south)

Rayon vers EST (step_x > 0, side = 0) → texture EST
Rayon vers OUEST (step_x < 0, side = 0) → texture OUEST
Rayon vers SUD (step_y > 0, side = 1) → texture SUD
Rayon vers NORD (step_y < 0, side = 1) → texture NORD
```

### Conversion couleur RGBA

```
Couleur source : t_color { r=255, g=128, b=64, a=255 }

Conversion MLX42 32 bits :
┌─────────┬─────────┬─────────┬─────────┐
│   R     │   G     │   B     │   A     │
│  255    │  128    │   64    │  255    │
│31    24 │23    16 │15     8 │7      0 │
└─────────┴─────────┴─────────┴─────────┘

Opérations bit à bit :
R << 24 = 255 << 24 = 0xFF000000
G << 16 = 128 << 16 = 0x00800000
B << 8  =  64 << 8  = 0x00004000
A << 0  = 255 << 0  = 0x000000FF

Résultat :
0xFF000000 | 0x00800000 | 0x00004000 | 0x000000FF = 0xFF8040FF
```

### Performance par frame

```
Opérations par frame (1024x768) :

┌─────────────────────────────────────────────────┐
│ ft_memset()          : 786,432 pixels = 3.1 MB  │
├─────────────────────────────────────────────────┤
│ draw_floor_ceiling() : 786,432 pixels            │
│   - Conversion couleur : 2 fois                 │
│   - Boucles imbriquées : 768 × 1024 itérations  │
│   - mlx_put_pixel()    : 786,432 appels         │
├─────────────────────────────────────────────────┤
│ cast_rays()          : 1024 rayons              │
│   - calculate_bounds   : 1024 fois              │
│   - get_texture        : 1024 fois              │
│   - draw_walls         : Variable pixels        │
└─────────────────────────────────────────────────┘

Total estimé : ~1.5 millions opérations/frame
À 60 FPS : ~90 millions opérations/seconde
```

---

## 🎓 CHECKLIST POUR LA CORRECTION

### Comprendre render_frame()
- [ ] Expliquer l'ordre de rendu (clear → background → foreground)
- [ ] Comprendre le nettoyage de buffer
- [ ] Identifier les 3 étapes principales

### Comprendre draw_floor_ceiling()
- [ ] Expliquer la conversion couleur RGBA
- [ ] Comprendre la division écran (moitié sup/inf)
- [ ] Identifier la complexité O(width × height)

### Comprendre calculate_draw_bounds()
- [ ] Expliquer la formule hauteur/distance
- [ ] Comprendre le centrage vertical
- [ ] Expliquer le clamp des limites

### Comprendre get_wall_texture()
- [ ] Différencier side 0 vs side 1
- [ ] Comprendre step_x et step_y
- [ ] Expliquer la logique N/S/E/W

### Comprendre draw_walls()
- [ ] Expliquer la coordination des étapes
- [ ] Comprendre le fallback texture NULL
- [ ] Identifier la délégation aux fonctions spécialisées

### Questions pièges possibles
- [ ] "Pourquoi nettoyer le buffer à chaque frame ?"
  → Éviter les artefacts visuels
  
- [ ] "Comment fonctionne la perspective ?"
  → height / distance = projection perspective
  
- [ ] "Pourquoi utiliser perp_wall_dist ?"
  → Évite l'effet fish-eye
  
- [ ] "Comment choisir la bonne texture ?"
  → Selon side (0/1) et step direction
  
- [ ] "Que se passe si un mur est très proche ?"
  → Clamp pour éviter débordement buffer

### Concepts clés à maîtriser
- [ ] **Projection perspective** : hauteur inversement proportionnelle à distance
- [ ] **Z-buffer implicite** : Ordre de dessin
- [ ] **RGBA 32 bits** : Format couleur MLX42
- [ ] **Clamp** : Limitation des valeurs aux bornes valides
- [ ] **Side/Step** : Orientation et direction pour textures

---

## 🎯 FIN DE L'EXPLICATION

Ce fichier couvre **100% du code** de `render.c` avec des explications détaillées adaptées pour une correction de projet 42.

**Fichiers d'explication disponibles** :
1. ✅ `EXECUTION_GUIDE.md` - Vue d'ensemble du raycasting
2. ✅ `MAIN_EXPLICATION.md` - Explication de main.c
3. ✅ `INIT_EXPLICATION.md` - Explication de init.c
4. ✅ `GAME_LOOP_EXPLICATION.md` - Explication de game_loop.c
5. ✅ `KEYBOARD_EXPLICATION.md` - Explication de keyboard.c
6. ✅ `MOVEMENT_UTILS_EXPLICATION.md` - Explication de movement_utils.c
7. ✅ `MOVEMENTS_EXPLICATION.md` - Explication de movements.c
8. ✅ `RENDER_EXPLICATION.md` - Explication de render.c (ce fichier)

**Pipeline complet documenté** :
- Input → Movement → Rendering
- Toute la chaîne d'exécution est couverte

**Prochaines étapes suggérées** :
- Comprendre le lien entre raycasting et rendu
- Visualiser l'impact de la distance sur la hauteur
- Maîtriser les concepts de projection perspective

Bon courage pour la correction ! 🚀