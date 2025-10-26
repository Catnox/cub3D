# WALL_UTILS.C - EXPLICATION DÉTAILLÉE

## 📋 TABLE DES MATIÈRES

1. [Vue d'ensemble](#vue-densemble)
2. [Fonction calculate_texture_x](#fonction-calculate_texture_x)
3. [Fonction init_color_components](#fonction-init_color_components)
4. [Fonction get_texture_color](#fonction-get_texture_color)
5. [Fonction draw_textured_wall](#fonction-draw_textured_wall)
6. [Fonction draw_colored_wall](#fonction-draw_colored_wall)
7. [Questions fréquentes](#questions-fréquentes)
8. [Diagrammes](#diagrammes)

---

## 🎯 VUE D'ENSEMBLE

### Rôle du fichier
`wall_utils.c` contient les **utilitaires de rendu des murs** :
- Calcul de mapping texture → écran
- Extraction de couleurs depuis les textures
- Dessin de colonnes de murs texturées
- Fallback pour murs colorés (debug)

### Fonctions principales
```
wall_utils.c
├── calculate_texture_x()     - Mapping horizontal texture
├── init_color_components()   - Extraction composantes RGBA
├── get_texture_color()       - Récupération couleur texture
├── draw_textured_wall()      - Dessin mur avec texture
└── draw_colored_wall()       - Dessin mur coloré (debug)
```

### Concepts clés
- **Texture mapping** : Correspondance texture ↔ écran
- **Scaling** : Adaptation taille texture → hauteur mur
- **Bounds checking** : Sécurité accès mémoire textures
- **Format support** : RGB (3 bytes) et RGBA (4 bytes)

---

## 📐 FONCTION CALCULATE_TEXTURE_X

### Signature
```c
int calculate_texture_x(t_game *game, t_ray *ray, mlx_texture_t *texture)
```

### Vue d'ensemble
Calcule la **coordonnée X dans la texture** correspondant au point où le rayon touche le mur. Cette fonction détermine quelle "colonne" de la texture utiliser.

### Analyse ligne par ligne

```c
int	calculate_texture_x(t_game *game, t_ray *ray, mlx_texture_t *texture)
{
```

```c
	double	wall_x;
	int		tex_x;
```
**Variables locales** :
- `wall_x` : Position exacte sur le mur (0.0 à 1.0)
- `tex_x` : Colonne de texture correspondante (0 à texture_width-1)

```c
	if (ray->side == 0)
		wall_x = game->player->pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		wall_x = game->player->pos.x + ray->perp_wall_dist * ray->dir.x;
```
**Calcul position sur le mur** :

**Si `ray->side == 0` (mur vertical)** :
- Le mur est perpendiculaire à l'axe X
- Position Y où le rayon touche : `player_y + distance × direction_y`
- On utilise la coordonnée Y pour déterminer la position horizontale sur le mur

**Si `ray->side == 1` (mur horizontal)** :
- Le mur est perpendiculaire à l'axe Y
- Position X où le rayon touche : `player_x + distance × direction_x`
- On utilise la coordonnée X pour déterminer la position horizontale sur le mur

**Logique géométrique** :
```
Rayon = Position_départ + Distance × Direction

Pour mur vertical :   wall_x = pos.y + dist × dir.y
Pour mur horizontal : wall_x = pos.x + dist × dir.x
```

```c
	wall_x -= floor(wall_x);
```
**Normalisation [0.0, 1.0)** :

**`floor(wall_x)`** : Partie entière (ex: floor(2.7) = 2)
**`wall_x - floor(wall_x)`** : Partie fractionnaire (ex: 2.7 - 2 = 0.7)

**Pourquoi ?** :
- Les murs ont des coordonnées entières (0, 1, 2, ...)
- On veut la position **relative** sur le mur (0.0 à 0.999...)
- Exemple : wall_x = 3.25 → position 0.25 sur le mur de la case (3,y)

**Exemples** :
```c
wall_x = 1.0  → 1.0 - 1 = 0.0  (début du mur)
wall_x = 1.3  → 1.3 - 1 = 0.3  (30% du mur)
wall_x = 1.7  → 1.7 - 1 = 0.7  (70% du mur)
wall_x = 2.0  → 2.0 - 2 = 0.0  (début du mur suivant)
```

```c
	tex_x = (int)(wall_x * (double)texture->width);
```
**Mapping vers texture** :

**Formule** : `tex_x = wall_x × largeur_texture`

**Exemples avec texture 64×64** :
```c
wall_x = 0.0  → tex_x = 0.0 × 64 = 0   (colonne 0)
wall_x = 0.25 → tex_x = 0.25 × 64 = 16 (colonne 16)
wall_x = 0.5  → tex_x = 0.5 × 64 = 32  (colonne 32)
wall_x = 0.75 → tex_x = 0.75 × 64 = 48 (colonne 48)
wall_x = 1.0  → tex_x = 1.0 × 64 = 64  (hors limites!)
```

**Cast vers int** : Troncature (floor implicite)

```c
	if (ray->side == 0 && ray->dir.x > 0)
		tex_x = texture->width - tex_x - 1;
```
**Flip horizontal (murs verticaux)** :

**Condition** : Mur vertical ET rayon va vers droite
- `side == 0` : Mur vertical
- `dir.x > 0` : Rayon va vers +X (droite)

**Flip** : `tex_x = width - tex_x - 1`
- Inverse l'ordre des colonnes de texture
- Exemple : tex_x=10 devient 64-10-1=53

**Pourquoi flipper ?** :
- Cohérence visuelle selon la direction d'approche
- Évite les textures "miroir" incohérentes

```c
	if (ray->side == 1 && ray->dir.y < 0)
		tex_x = texture->width - tex_x - 1;
```
**Flip horizontal (murs horizontaux)** :

**Condition** : Mur horizontal ET rayon va vers haut
- `side == 1` : Mur horizontal
- `dir.y < 0` : Rayon va vers -Y (haut)

**Même logique de flip** pour cohérence visuelle

```c
	return (tex_x);
}
```

### Logique des flips

```
Vue de dessus, exemples de flip :

Mur vertical (side=0), rayon vers droite (dir.x > 0) :
P ──→ │texture│  FLIP car on voit l'envers
      │   ┌─┐ │
      │   └─┘ │

Mur vertical (side=0), rayon vers gauche (dir.x < 0) :
      │texture│ ←── P  PAS DE FLIP car on voit l'endroit
      │ ┌─┐   │
      │ └─┘   │

Mur horizontal (side=1), rayon vers bas (dir.y > 0) :
P
│              PAS DE FLIP
↓
─────texture─────

Mur horizontal (side=1), rayon vers haut (dir.y < 0) :
─────texture─────
↑              FLIP car on voit l'envers
│
P
```

---

## 🎨 FONCTION INIT_COLOR_COMPONENTS

### Signature
```c
void init_color_components(mlx_texture_t *texture, int pixel_index, t_color_components *color)
```

### Vue d'ensemble
Fonction utilitaire qui extrait les composantes RGBA d'un pixel de texture. Gère les formats RGB (3 bytes) et RGBA (4 bytes).

### Analyse ligne par ligne

```c
void	init_color_components(mlx_texture_t *texture, int pixel_index,
							t_color_components *color)
{
```
**Paramètres** :
- `texture` : Texture source
- `pixel_index` : Index dans le tableau de bytes
- `color` : Structure à remplir (sortie)

```c
	color->r = texture->pixels[pixel_index];
	color->g = texture->pixels[pixel_index + 1];
	color->b = texture->pixels[pixel_index + 2];
```
**Extraction RGB** :

**Format des pixels en mémoire** :
```
RGB:  [R][G][B][R][G][B][R][G][B]...
RGBA: [R][G][B][A][R][G][B][A]...
Index: 0  1  2  3  4  5  6  7...
```

**Accès séquentiel** :
- `pixels[index]` = Rouge
- `pixels[index+1]` = Vert
- `pixels[index+2]` = Bleu

```c
	if (texture->bytes_per_pixel == 4)
		color->a = texture->pixels[pixel_index + 3];
	else
		color->a = 0xFF;
```
**Extraction Alpha** :

**Si 4 bytes par pixel (RGBA)** :
- `pixels[index+3]` = Alpha
- Valeur réelle de transparence

**Si 3 bytes par pixel (RGB)** :
- Pas de canal alpha dans la texture
- Alpha = 0xFF = 255 = opaque par défaut

```c
}
```

### Gestion des formats

```c
// Format RGB (3 bytes/pixel)
texture->bytes_per_pixel = 3
Pixel: [R][G][B][R][G][B]...
Alpha: Défaut 0xFF (opaque)

// Format RGBA (4 bytes/pixel)
texture->bytes_per_pixel = 4
Pixel: [R][G][B][A][R][G][B][A]...
Alpha: Valeur de la texture
```

---

## 🖼️ FONCTION GET_TEXTURE_COLOR

### Signature
```c
uint32_t get_texture_color(mlx_texture_t *texture, int tex_x, int tex_y)
```

### Vue d'ensemble
Récupère la couleur d'un pixel spécifique dans une texture. Inclut la vérification des limites et la gestion d'erreur.

### Analyse ligne par ligne

```c
uint32_t	get_texture_color(mlx_texture_t *texture, int tex_x, int tex_y)
{
```
**Retour** : Couleur 32 bits au format RGBA MLX42

```c
	int					pixel_index;
	uint32_t			color;
	t_color_components	rgba;
```
**Variables locales** :
- `pixel_index` : Position dans le tableau de bytes
- `color` : Couleur finale 32 bits
- `rgba` : Composantes temporaires

```c
	pixel_index = (tex_y * texture->width + tex_x) * texture->bytes_per_pixel;
```
**Calcul index 2D → 1D** :

**Formule** : `index = (y × largeur + x) × bytes_par_pixel`

**Logique** :
- Texture stockée ligne par ligne
- Ligne y = y × largeur pixels
- Colonne x = + x pixels
- Chaque pixel = × bytes_par_pixel bytes

**Exemple texture 64×64 RGBA** :
```c
tex_x=10, tex_y=5, bytes_per_pixel=4

pixel_index = (5 × 64 + 10) × 4
            = (320 + 10) × 4
            = 330 × 4
            = 1320

Bytes texture: [0][1][2]...[1320][1321][1322][1323]...
Pixel (10,5):                 ↑R    ↑G    ↑B    ↑A
```

```c
	rgba.r = 0;
	rgba.g = 0;
	rgba.b = 0;
	rgba.a = 0xFF;
```
**Initialisation par défaut** :
- RGB = 0 (noir)
- Alpha = 0xFF (opaque)
- Valeurs de fallback en cas d'erreur

```c
	if (pixel_index >= 0 && pixel_index < (int)(texture->width
		* texture->height * texture->bytes_per_pixel))
	{
```
**Vérification limites** :

**Condition** : `0 ≤ index < taille_totale`

**Taille totale** : `largeur × hauteur × bytes_par_pixel`

**Pourquoi cette vérification ?** :
- Évite les accès mémoire invalides
- Détecte les coordonnées hors limites
- Prévient les crashes

```c
		if (texture->bytes_per_pixel == 4 || texture->bytes_per_pixel == 3)
		{
			init_color_components(texture, pixel_index, &rgba);
			color = (rgba.r << 24) | (rgba.g << 16) | (rgba.b << 8) | rgba.a;
		}
```
**Traitement formats supportés** :

**Formats RGB/RGBA** :
- 3 ou 4 bytes par pixel = formats standards
- Appel à `init_color_components()` pour extraction
- Conversion au format 32 bits MLX42

**Conversion RGBA → 32 bits** :
```c
Format MLX42: [R][G][B][A] (8 bits chacun)
Bit shifting:
R << 24 = Rouge en bits 31-24
G << 16 = Vert en bits 23-16
B << 8  = Bleu en bits 15-8
A << 0  = Alpha en bits 7-0
```

```c
		else
			color = 0xFFFFFFFF;
```
**Format non supporté** :
- Couleur blanche (debug)
- Indique un problème de format

```c
	}
	else
		color = 0xFF00FFFF;
```
**Hors limites** :
- Couleur magenta (debug)
- `0xFF00FFFF` = rouge + bleu = magenta
- Facilement reconnaissable visuellement

```c
	return (color);
}
```

### Codes couleur de debug

```c
0xFFFFFFFF = Blanc     → Format texture non supporté
0xFF00FFFF = Magenta   → Coordonnées hors limites
0x000000FF = Noir      → Valeur par défaut normale
```

---

## 🎨 FONCTION DRAW_TEXTURED_WALL

### Signature
```c
void draw_textured_wall(t_game *game, t_ray *ray, t_wall_draw_params *params)
```

### Vue d'ensemble
Fonction principale qui dessine une **colonne de mur texturé**. Elle mappe chaque pixel écran à un pixel texture avec scaling approprié.

### Analyse ligne par ligne

```c
void	draw_textured_wall(t_game *game, t_ray *ray, t_wall_draw_params *params)
{
```

```c
	int		tex_x;
	int		tex_y;
	double	step;
	double	tex_pos;
	int		y;
```
**Variables locales** :
- `tex_x` : Colonne texture (constante pour la colonne)
- `tex_y` : Ligne texture (variable selon y écran)
- `step` : Incrément texture par pixel écran
- `tex_pos` : Position flottante dans texture
- `y` : Coordonnée écran en cours

```c
	tex_x = calculate_texture_x(game, ray, params->texture);
```
**Colonne texture** :
- Calculée une seule fois pour toute la colonne
- Détermine quelle "bande verticale" de texture utiliser
- Constante de `draw_start` à `draw_end`

```c
	step = 1.0 * params->texture->height / params->line_height;
```
**Calcul du pas de texture** :

**Formule** : `step = hauteur_texture / hauteur_mur_écran`

**Logique** :
- `line_height` = hauteur du mur en pixels écran
- `texture->height` = hauteur de la texture en pixels
- `step` = combien de pixels texture par pixel écran

**Exemples** :
```c
Texture 64×64, mur 128 pixels à l'écran :
step = 64 / 128 = 0.5
→ 1 pixel texture pour 2 pixels écran (agrandissement)

Texture 64×64, mur 32 pixels à l'écran :
step = 64 / 32 = 2.0
→ 2 pixels texture pour 1 pixel écran (rétrécissement)
```

```c
	tex_pos = (params->draw_start - WINDOW_HEIGHT
			/ 2 + params->line_height / 2) * step;
```
**Position initiale dans texture** :

**Décomposition** :
```c
centre_écran = WINDOW_HEIGHT / 2
centre_mur = params->line_height / 2
offset_début = params->draw_start - centre_écran + centre_mur

tex_pos = offset_début * step
```

**Logique** :
- Si le mur commence en haut de l'écran (draw_start=0), on commence au début de la texture
- Si le mur est centré, on commence au centre de la texture
- L'offset est multiplié par `step` pour la correspondance texture

**Exemple** :
```c
WINDOW_HEIGHT = 768, line_height = 200, draw_start = 284
centre_écran = 384, centre_mur = 100
offset = 284 - 384 + 100 = 0
tex_pos = 0 * step = 0
→ Commence au début de la texture
```

```c
	y = params->draw_start;
	while (y <= params->draw_end)
	{
```
**Boucle verticale** :
- De `draw_start` à `draw_end`
- Un pixel écran par itération

```c
		tex_y = (int)tex_pos & (params->texture->height - 1);
```
**Calcul ligne texture** :

**`(int)tex_pos`** : Conversion flottant → entier
**`& (height - 1)`** : Modulo optimisé (si height est puissance de 2)

**Pourquoi le modulo ?** :
- Texture peut être plus petite que le mur
- Répétition (tiling) automatique
- Évite les débordements

**Exemple avec texture 64×64** :
```c
tex_pos = 67.5
tex_y = 67 & 63 = 67 & 0x3F = 3
→ Wraparound, ligne 3 de la texture
```

```c
		tex_pos += step;
```
**Avancement dans texture** :
- Ajoute le pas calculé
- Progression uniforme dans la texture

```c
		mlx_put_pixel(game->img, params->x, y,
			get_texture_color(params->texture, tex_x, tex_y));
```
**Dessin du pixel** :
- Position écran : `(params->x, y)`
- Couleur : extraite de la texture `(tex_x, tex_y)`
- Un appel `mlx_put_pixel()` par pixel

```c
		y++;
	}
```
**Pixel suivant** : Ligne écran suivante

### Principe du texture mapping

```
Texture 64×64        Écran (colonne)
┌──────────┐         ┌─┐
│ 0 ┌────┐ │ tex_x=20│ │ params->x
│ 1 │tex │ │   ─────→│█│ y=draw_start
│ 2 │ture│ │         │█│ y+1
│ 3 │    │ │         │█│ y+2
│...│    │ │         │.│ ...
│63 └────┘ │         │█│ y=draw_end
└──────────┘         └─┘

tex_y varie de 0 à 63 (avec répétition si nécessaire)
tex_x reste constant (colonne déterminée par ray)
```

---

## 🎭 FONCTION DRAW_COLORED_WALL

### Signature
```c
void draw_colored_wall(t_game *game, t_ray *ray, t_wall_draw_params *params)
```

### Vue d'ensemble
Fonction de **fallback** qui dessine un mur avec une couleur unie. Utilisée quand aucune texture n'est disponible. Couleurs différentes selon l'orientation (debug visuel).

### Analyse ligne par ligne

```c
void	draw_colored_wall(t_game *game, t_ray *ray, t_wall_draw_params *params)
{
```

```c
	uint32_t	color;
	int			y;
```
**Variables locales** :
- `color` : Couleur unie pour toute la colonne
- `y` : Coordonnée écran en cours

```c
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			color = 0xFF0000FF;
		else
			color = 0x0000FFFF;
	}
```
**Murs verticaux (side == 0)** :

**Est (step_x > 0)** : `0xFF0000FF`
- Rouge pur avec alpha = 0xFF (opaque)
- Format: [FF][00][00][FF] = Rouge opaque

**Ouest (step_x ≤ 0)** : `0x0000FFFF`
- Bleu pur avec alpha = 0xFF (opaque)
- Format: [00][00][FF][FF] = Bleu opaque

```c
	else
	{
		if (ray->step_y > 0)
			color = 0x00FF00FF;
		else
			color = 0xFFFFFFFF;
	}
```
**Murs horizontaux (side == 1)** :

**Sud (step_y > 0)** : `0x00FF00FF`
- Vert pur avec alpha = 0xFF (opaque)
- Format: [00][FF][00][FF] = Vert opaque

**Nord (step_y ≤ 0)** : `0xFFFFFFFF`
- Blanc avec alpha = 0xFF (opaque)
- Format: [FF][FF][FF][FF] = Blanc opaque

```c
	y = params->draw_start;
	while (y <= params->draw_end)
	{
		mlx_put_pixel(game->img, params->x, y, color);
		y++;
	}
```
**Boucle de dessin** :
- Même logique que `draw_textured_wall()`
- Mais couleur constante au lieu de texture
- Beaucoup plus simple et rapide

```c
}
```

### Code couleur par orientation

```
Vue de dessus avec couleurs :

        BLANC (Nord)
           ↑
           │
    BLEU ──┼── ROUGE
    (Ouest)│  (Est)
           │
           ↓
         VERT (Sud)

Aide au debug :
- Rouge = Mur Est (lever du soleil)
- Bleu = Mur Ouest (océan)
- Vert = Mur Sud (végétation)
- Blanc = Mur Nord (neige)
```

---

## ❓ QUESTIONS FRÉQUENTES

### Q1 : Comment fonctionne le texture mapping ?
**R :**
```c
1. calculate_texture_x() → Colonne texture (tex_x)
2. Pour chaque pixel écran y :
   - Calculer tex_y correspondant
   - Extraire couleur texture[tex_x][tex_y]
   - Dessiner pixel écran
```

### Q2 : Pourquoi flipper certaines textures ?
**R :**
```c
// Évite les textures "miroir" incohérentes
// Selon la direction d'approche du rayon
if (side == 0 && dir.x > 0) flip;  // Est
if (side == 1 && dir.y < 0) flip;  // Nord
```

### Q3 : Comment gérer des textures de tailles différentes ?
**R :**
```c
step = texture->height / line_height;
tex_y = (int)tex_pos & (texture->height - 1);

// Scaling automatique + tiling pour débordements
```

### Q4 : Que se passe-t-il avec tex_y hors limites ?
**R :**
```c
tex_y = tex_pos & (height - 1);
// Modulo optimisé → wraparound automatique
// tex_y reste toujours dans [0, height-1]
```

### Q5 : Pourquoi des couleurs de debug différentes ?
**R :**
```c
Rouge = Est, Bleu = Ouest, Vert = Sud, Blanc = Nord
// Identification visuelle immédiate de l'orientation
// Aide au debug du raycasting
```

### Q6 : Comment optimiser le texture mapping ?
**R :**
- Textures puissance de 2 (64×64, 128×128)
- Modulo optimisé avec `& (size-1)`
- Précalcul de `tex_x` une fois par colonne
- Bounds checking optimisé

### Q7 : Que signifie 0xFF00FFFF (magenta) ?
**R :**
```c
0xFF00FFFF = [FF][00][FF][FF] = Rouge + Bleu = Magenta
// Couleur de debug pour accès hors limites
// Très visible, indique un bug
```

### Q8 : Comment fonctionne le scaling vertical ?
**R :**
```c
step = texture_height / screen_height;

Texture plus grande que mur → step > 1 → échantillonnage
Texture plus petite que mur → step < 1 → interpolation
```

### Q9 : Pourquoi wall_x - floor(wall_x) ?
**R :**
```c
// Normalise la position sur le mur en [0.0, 1.0)
wall_x = 3.7 → 3.7 - 3 = 0.7 (70% du mur)
// Indépendant de la position absolue dans la map
```

### Q10 : Comment débugger les problèmes de texture ?
**R :**
1. Vérifier les couleurs debug (magenta = hors limites)
2. Tester avec `draw_colored_wall()` d'abord
3. Vérifier `tex_x` et `tex_y` dans les limites
4. Confirmer le format de texture (RGB/RGBA)

---

## 📊 DIAGRAMMES

### Pipeline texture mapping

```
Rayon → Mur → Position sur mur → Texture
  │       │         │             │
  │       │         │             ↓
  │       │         │      ┌─────────────┐
  │       │         │      │   tex_x     │ ← calculate_texture_x()
  │       │         │      │ (constant)  │
  │       │         │      └─────────────┘
  │       │         │             │
  │       │         └─────────────┼───→ wall_x (0.0 - 1.0)
  │       │                       │
  │       └───────────────────────┼───→ line_height
  │                               │
  └───────────────────────────────┼───→ side, step_x, step_y
                                  │
                                  ↓
                          draw_textured_wall()
                                  │
                    ┌─────────────┼─────────────┐
                    │             │             │
                    ↓             ↓             ↓
            Calcul step     Position init    Boucle Y
               │               tex_pos          │
               │                  │            │
               └──────────────────┬────────────┘
                                  │
                                  ↓
                             tex_y = tex_pos
                                  │
                                  ↓
                      get_texture_color(tex_x, tex_y)
                                  │
                                  ↓
                            mlx_put_pixel()
```

### Mapping coordonnées

```
Texture 64×64                     Écran 1024×768
┌─────────────────┐              ┌──────────────────┐
│ 0   tex_x   63  │              │0      x     1023│
│ ┌─────────────┐ │              │                  │
│0│             │ │              │                  │
│ │   TEXTURE   │ │     MAP      │     ÉCRAN        │
│ │             │ │    ────→     │                  │
│ │             │ │              │      ████        │ ← line_height
│ │             │ │              │      ████        │
│ └─────────────┘ │              │      ████        │
│63              │              │                  │
└─────────────────┘              │767               │
                                 └──────────────────┘

wall_x ∈ [0.0, 1.0) → tex_x ∈ [0, 63]
tex_pos progression → tex_y ∈ [0, 63] (avec wrap)
```

### Gestion des formats texture

```
Format RGB (3 bytes/pixel) :
┌──┬──┬──┬──┬──┬──┬──┬──┬──┐
│R │G │B │R │G │B │R │G │B │
└──┴──┴──┴──┴──┴──┴──┴──┴──┘
 0  1  2  3  4  5  6  7  8
    Pixel 0   Pixel 1   Pixel 2

Format RGBA (4 bytes/pixel) :
┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
│R │G │B │A │R │G │B │A │R │G │B │A │
└──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
 0  1  2  3  4  5  6  7  8  9 10 11
     Pixel 0       Pixel 1       Pixel 2

Calcul index : (y × width + x) × bytes_per_pixel
```

### Scaling et step

```
AGRANDISSEMENT (mur proche, step < 1.0) :
Texture ──→ Écran
64 px   ──→ 128 px
step = 64/128 = 0.5

Y écran │ tex_pos │ tex_y
────────┼─────────┼──────
   0    │   0.0   │   0
   1    │   0.5   │   0   ← Même pixel texture
   2    │   1.0   │   1
   3    │   1.5   │   1   ← Même pixel texture
   4    │   2.0   │   2

RÉTRÉCISSEMENT (mur loin, step > 1.0) :
Texture ──→ Écran
64 px   ──→ 32 px
step = 64/32 = 2.0

Y écran │ tex_pos │ tex_y
────────┼─────────┼──────
   0    │   0.0   │   0
   1    │   2.0   │   2   ← Saut de pixels
   2    │   4.0   │   4
   3    │   6.0   │   6
```

### Couleurs debug par orientation

```
Vue 3D de la map avec couleurs :

           ┌──── BLANC (Nord) ────┐
           │                     │
           │                     │
    BLEU   │         P           │ ROUGE
   (Ouest) │         │           │ (Est)
           │         │           │
           │         ↓           │
           └──── VERT (Sud) ─────┘

Code couleur :
Nord  : 0xFFFFFFFF (Blanc)
Sud   : 0x00FF00FF (Vert)
Est   : 0xFF0000FF (Rouge)
Ouest : 0x0000FFFF (Bleu)
```

### Texture flipping

```
SANS FLIP (vue normale) :
┌─────┐
│ABC│ │ ← Texture vue de face
│DEF│ │
│GHI│ │
└─────┘

AVEC FLIP (vue inversée) :
┌─────┐
│ │CBA│ ← Texture vue de dos (miroir)
│ │FED│
│ │IHG│
└─────┘

Code flip : tex_x = width - tex_x - 1
Exemple : tex_x=0 → width-0-1 = 63 (dernière colonne)
```

---

## 🎓 CHECKLIST POUR LA CORRECTION

### Comprendre calculate_texture_x()
- [ ] Expliquer la différence side 0 vs side 1
- [ ] Comprendre wall_x - floor(wall_x)
- [ ] Expliquer les conditions de flip
- [ ] Mapping 0.0-1.0 vers 0-width

### Comprendre init_color_components()
- [ ] Différence RGB vs RGBA
- [ ] Extraction séquentielle R, G, B, A
- [ ] Gestion alpha par défaut

### Comprendre get_texture_color()
- [ ] Calcul index 2D → 1D
- [ ] Bounds checking et sécurité
- [ ] Codes couleur debug (blanc, magenta)
- [ ] Conversion finale 32 bits

### Comprendre draw_textured_wall()
- [ ] Calcul du step (scaling)
- [ ] Position initiale tex_pos
- [ ] Boucle mapping pixel par pixel
- [ ] Modulo pour wraparound

### Comprendre draw_colored_wall()
- [ ] Système de fallback
- [ ] Couleurs debug par orientation
- [ ] Simplicité vs texture

### Questions pièges possibles
- [ ] "Comment éviter l'effet fish-eye ?"
  → Utiliser perp_wall_dist, pas la distance réelle
  
- [ ] "Pourquoi flipper les textures ?"
  → Cohérence visuelle selon direction d'approche
  
- [ ] "Comment gérer différentes tailles de texture ?"
  → Step = texture_height / line_height
  
- [ ] "Que fait le modulo & (height-1) ?"
  → Wraparound optimisé pour textures répétitives
  
- [ ] "Comment débugger les textures incorrectes ?"
  → Couleurs magenta = hors limites, couleurs unies = fallback

### Concepts clés à maîtriser
- [ ] **Texture mapping** : Correspondance 2D texture ↔ écran
- [ ] **Scaling** : Adaptation taille avec step
- [ ] **Bounds checking** : Sécurité accès mémoire
- [ ] **Format handling** : RGB vs RGBA
- [ ] **Debug visual** : Couleurs par orientation

---

## 🎯 FIN DE L'EXPLICATION

Ce fichier couvre **100% du code** de `wall_utils.c` avec des explications détaillées adaptées pour une correction de projet 42.

**Fichiers d'explication disponibles** :
1. ✅ `EXECUTION_GUIDE.md` - Vue d'ensemble du raycasting
2. ✅ `MAIN_EXPLICATION.md` - Explication de main.c
3. ✅ `INIT_EXPLICATION.md` - Explication de init.c
4. ✅ `GAME_LOOP_EXPLICATION.md` - Explication de game_loop.c
5. ✅ `KEYBOARD_EXPLICATION.md` - Explication de keyboard.c
6. ✅ `MOVEMENT_UTILS_EXPLICATION.md` - Explication de movement_utils.c
7. ✅ `MOVEMENTS_EXPLICATION.md` - Explication de movements.c
8. ✅ `RENDER_EXPLICATION.md` - Explication de render.c
9. ✅ `WALL_UTILS_EXPLICATION.md` - Explication de wall_utils.c (ce fichier)

**Pipeline de rendu complet documenté** :
- Raycasting → Calcul dimensions → Mapping texture → Dessin pixel
- Toute la chaîne de rendu graphique est couverte

**Prochaines étapes suggérées** :
- Comprendre le lien raycasting → texture mapping
- Maîtriser les concepts de scaling et bounds checking
- Visualiser l'impact des flips de texture

Bon courage pour la correction ! 🚀