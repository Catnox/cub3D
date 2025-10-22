# 🎮 Guide d'Explication - Exécution du cub3D

## 📋 Vue d'ensemble de votre partie

Vous devez expliquer **tout ce qui se passe après le parsing**, c'est-à-dire :
1. L'initialisation du jeu (MLX42, structures)
2. La boucle de jeu principale
3. Le raycasting (algorithme DDA)
4. Le rendu graphique
5. La gestion des contrôles (clavier/souris)
6. Le nettoyage et la fermeture

---

## 🚀 1. INITIALISATION DU JEU

### Fonction principale : `init_game()` dans `src/core/init.c`

**Ordre d'initialisation (IMPORTANT) :**
```c
1. Parsing du fichier .cub (fait par votre binôme)
2. Trouver la position du joueur
3. Initialiser MLX42 et créer la fenêtre
4. Créer l'image principale (buffer de rendu)
5. Afficher l'image dans la fenêtre
6. Configurer le callback de redimensionnement
```

### Questions pièges possibles :

**Q: Pourquoi initialiser MLX APRÈS le parsing ?**
- R: Si le parsing échoue, pas besoin d'initialiser MLX (économie de ressources + logique)
- R: Les textures ont besoin de MLX pour être chargées (mlx_load_png)

**Q: Que se passe-t-il si la fenêtre est redimensionnée ?**
- R: `resize_callback()` est appelé
- R: On supprime l'ancienne image et on en crée une nouvelle aux nouvelles dimensions
- R: La résolution du jeu s'adapte dynamiquement

**Q: Pourquoi pas de `game->is_running` ?**
- R: MLX42 gère déjà la boucle avec `mlx_loop()`, pas besoin d'un flag supplémentaire

---

## 🔄 2. BOUCLE DE JEU PRINCIPALE

### Fonction : `game_loop()` dans `src/core/game_loop.c`

**Architecture de la boucle :**
```
MLX Loop (infinie)
    ↓
game_update() appelé chaque frame
    ↓
1. handle_continuous_input() - Gestion des touches maintenues
2. render_frame() - Rendu complet de l'image
```

**Enregistrement des hooks MLX :**
```c
mlx_key_hook()        → handle_keyboard()      (presse/relâche touche)
mlx_loop_hook()       → game_update()          (chaque frame)
mlx_close_hook()      → on_close()             (fermeture fenêtre)
```

### Questions pièges :

**Q: Différence entre `mlx_key_hook` et `handle_continuous_input` ?**
- `mlx_key_hook` : Détecte UNE FOIS la presse/relâche (événement unique)
- `handle_continuous_input` : Vérifie l'état des touches CHAQUE FRAME avec `mlx_is_key_down()`
- Permet un mouvement fluide (maintenir W pour avancer continuellement)

**Q: Pourquoi `game_update()` est static ?**
- R: Fonction interne, pas besoin de l'exposer dans le header
- R: Encapsulation, seulement utilisée dans game_loop.c

**Q: Quelle est la fréquence d'appel de game_update() ?**
- R: ~60 FPS (dépend du monitor refresh rate)
- R: MLX42 gère le framerate automatiquement (vsync)

---

## 🔍 3. RAYCASTING - ALGORITHME DDA

### Le cœur du moteur 3D : `cast_rays()` dans `src/raycasting/raycasting.c`

**Principe général :**
```
Pour chaque colonne X de l'écran (0 à WINDOW_WIDTH) :
    1. Calculer la direction du rayon
    2. Effectuer l'algorithme DDA (Digital Differential Analysis)
    3. Calculer la distance au mur
    4. Dessiner la colonne du mur
```

### 3.1. Initialisation du rayon : `init_ray()`

**Calculs clés :**
```c
// Position de la caméra sur l'axe X (0.0 = gauche, 1.0 = droite)
double cameraX = 2 * x / (double)WINDOW_WIDTH - 1;

// Direction du rayon = direction du joueur + plan de caméra
ray->dir.x = player->dir.x + player->plane.x * cameraX;
ray->dir.y = player->dir.y + player->plane.y * cameraX;

// Delta_dist = distance à parcourir pour traverser une case
ray->delta_dist.x = fabs(1 / ray->dir.x);
ray->delta_dist.y = fabs(1 / ray->dir.y);
```

### Questions pièges :

**Q: Qu'est-ce que le plan de caméra (plane) ?**
- R: Vecteur perpendiculaire à la direction du joueur
- R: Représente la largeur du champ de vision (FOV)
- R: Sa longueur détermine l'angle de vue (typiquement 0.66 pour ~66°)

**Q: Pourquoi 2 * x / width - 1 ?**
- R: Convertit les pixels (0 à width) en coordonnées (-1 à +1)
- R: -1 = bord gauche, 0 = centre, +1 = bord droit
- R: Nécessaire pour le calcul de la direction du rayon

**Q: C'est quoi delta_dist exactement ?**
- R: Distance que le rayon parcourt pour passer d'une ligne de grille X à la suivante
- R: Calculé avec l'inverse de la direction (1/dir) car c'est proportionnel
- R: `fabs()` car on veut toujours une distance positive

### 3.2. Algorithme DDA : `perform_dda()` dans `src/raycasting/dda.c`

**Principe de l'algorithme :**
```
1. Déterminer dans quelle direction avancer (step_x, step_y)
2. Calculer la distance initiale jusqu'à la prochaine ligne de grille
3. BOUCLE tant qu'on n'a pas touché un mur :
   - Avancer vers la ligne de grille la plus proche
   - Vérifier si c'est un mur
```

**Code simplifié :**
```c
while (ray->hit == 0)
{
    // Avancer horizontalement ou verticalement ?
    if (ray->side_dist.x < ray->side_dist.y)
    {
        ray->side_dist.x += ray->delta_dist.x;  // Avancer en X
        ray->map_x += ray->step_x;
        ray->side = 0;  // Collision verticale (mur Est/Ouest)
    }
    else
    {
        ray->side_dist.y += ray->delta_dist.y;  // Avancer en Y
        ray->map_y += ray->step_y;
        ray->side = 1;  // Collision horizontale (mur Nord/Sud)
    }
    
    // Vérifier si on a touché un mur
    if (game->map->grid[ray->map_y][ray->map_x] == '1')
        ray->hit = 1;
}
```

### Questions pièges IMPORTANTES :

**Q: Pourquoi appelle-t-on ça DDA (Digital Differential Analysis) ?**
- R: Algorithme qui avance case par case dans une grille
- R: "Digital" car travaille sur grille discrète (entiers)
- R: "Differential" car incrémente par petits pas (delta)

**Q: Pourquoi comparer side_dist.x et side_dist.y ?**
- R: On choisit la prochaine ligne de grille la plus PROCHE
- R: C'est l'essence du DDA : toujours avancer vers le prochain bord le plus près
- R: Garantit qu'on ne rate aucune case

**Q: C'est quoi la différence entre side 0 et side 1 ?**
- side = 0 : collision sur un mur vertical (Nord/Sud, texture east/west)
- side = 1 : collision sur un mur horizontal (Est/Ouest, texture north/south)
- Utilisé pour choisir la bonne texture et créer l'effet d'ombrage

**Q: Comment éviter les boucles infinies ?**
- R: On vérifie les limites de la map avant d'accéder au tableau
- R: La map doit être entourée de murs (validation du parsing)

### 3.3. Calcul de la distance : `calculate_distance()`

**Correction de la distorsion fisheye :**
```c
if (ray->side == 0)
    perp_wall_dist = (ray->map_x - player->pos.x + 
                     (1 - ray->step_x) / 2) / ray->dir.x;
else
    perp_wall_dist = (ray->map_y - player->pos.y + 
                     (1 - ray->step_y) / 2) / ray->dir.y;
```

### Question piège CRUCIALE :

**Q: Pourquoi "perpendicular" wall distance et pas distance euclidienne ?**
- R: Distance euclidienne = √(Δx² + Δy²) → effet fisheye (distorsion)
- R: Distance perpendiculaire = projection sur le plan de la caméra
- R: Simule une caméra réelle avec un plan de projection plat
- **DESSIN AU TABLEAU** : Montrer la différence entre les deux distances

---

## 🎨 4. RENDU GRAPHIQUE

### Fonction principale : `render_frame()` dans `src/rendering/render.c`

**Pipeline de rendu :**
```
1. draw_floor_ceiling()     → Remplir le sol et plafond
2. cast_rays()               → Pour chaque colonne X
   ↓
3. calculate_draw_bounds()   → Hauteur du mur à l'écran
4. draw_walls()              → Dessiner la bande de texture
```

### 4.1. Sol et plafond : `draw_floor_ceiling()`

**Principe simple :**
```c
for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
        if (y < height/2)
            pixel = ceiling_color;  // Moitié haute
        else
            pixel = floor_color;    // Moitié basse
```

**Conversion des couleurs :**
```c
// Format RGBA (Red Green Blue Alpha) en 32-bit
color = (r << 24) | (g << 16) | (b << 8) | a;
```

### Questions pièges :

**Q: Pourquoi dessiner sol/plafond en premier ?**
- R: Les murs vont écraser une partie de l'image (optimisation inutile certes)
- R: Assure qu'il n'y a pas de pixels non initialisés

**Q: Pourquoi le bit shifting (<<) pour les couleurs ?**
- R: MLX42 attend un uint32_t en format RGBA
- R: `<<` décale les bits : r<<24 place rouge sur les 8 bits de poids fort
- R: L'opérateur `|` combine les 4 composantes en un seul entier

### 4.2. Calcul de la hauteur du mur : `calculate_draw_bounds()`

**Formule clé :**
```c
line_height = (int)(WINDOW_HEIGHT / perp_wall_dist);

draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;  // Haut du mur
draw_end = line_height / 2 + WINDOW_HEIGHT / 2;     // Bas du mur
```

### Questions pièges :

**Q: Pourquoi diviser par la distance ?**
- R: Plus le mur est loin, plus il paraît petit (perspective)
- R: C'est une projection en perspective : hauteur_écran = hauteur_réelle / distance
- R: WINDOW_HEIGHT est la hauteur de référence à distance = 1

**Q: Que se passe-t-il si draw_start < 0 ou draw_end > height ?**
- R: Le mur est trop près ou trop haut
- R: On clamp les valeurs dans `draw_walls()` pour éviter les segfaults
- R: On dessine seulement la partie visible

### 4.3. Texture du mur : `draw_textured_wall()`

**Sélection de la texture :**
```c
if (ray->side == 0)  // Mur vertical
    texture = (ray->step_x > 0) ? textures.east : textures.west;
else                 // Mur horizontal
    texture = (ray->step_y > 0) ? textures.south : textures.north;
```

**Mapping de texture :**
```c
// Position X sur le mur (0.0 à 1.0)
double wallX = (side == 0) ? 
    player->pos.y + perp_wall_dist * ray->dir.y :
    player->pos.x + perp_wall_dist * ray->dir.x;
wallX -= floor(wallX);  // Garder seulement la partie décimale

// Pixel X dans la texture
int tex_x = (int)(wallX * texture->width);

// Pour chaque pixel Y du mur à l'écran
for (int y = draw_start; y < draw_end; y++)
{
    // Position Y dans la texture (interpolation)
    int tex_y = (y - draw_start) * texture->height / line_height;
    
    // Récupérer et dessiner le pixel
    uint32_t color = get_texture_color(texture, tex_x, tex_y);
    mlx_put_pixel(game->img, x, y, color);
}
```

### Questions pièges IMPORTANTES :

**Q: Comment fonctionne le mapping de texture ?**
- R: On projette la texture 2D sur la surface 3D du mur
- R: `wallX` = position horizontale où le rayon a touché le mur (entre 0 et 1)
- R: On interpole verticalement selon la hauteur du mur à l'écran

**Q: Pourquoi `wallX - floor(wallX)` ?**
- R: `floor(wallX)` donne la partie entière (ex: 3.7 → 3)
- R: Soustraire laisse seulement la partie décimale (3.7 - 3 = 0.7)
- R: Nécessaire car on veut la position DANS la case (0 à 1), pas la case elle-même

**Q: Que contient texture->pixels ?**
- R: Tableau de uint8_t contenant les pixels RGBA
- R: Format: [R, G, B, A, R, G, B, A, ...]
- R: `bytes_per_pixel` = 3 (RGB) ou 4 (RGBA)

**Q: Comment obtenir un pixel spécifique de la texture ?**
```c
int pixel_index = (tex_y * texture->width + tex_x) * texture->bytes_per_pixel;
r = texture->pixels[pixel_index];
g = texture->pixels[pixel_index + 1];
b = texture->pixels[pixel_index + 2];
a = texture->pixels[pixel_index + 3];  // Si RGBA
```

---

## 🎮 5. GESTION DES CONTRÔLES

### 5.1. Clavier : `handle_keyboard()` et `handle_continuous_input()`

**Deux types de gestion :**

**A) Événements uniques (key press/release) :**
```c
void handle_keyboard(mlx_key_data_t keydata, void *param)
{
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
        mlx_close_window(game->mlx);
}
```

**B) Input continu (maintenir la touche) :**
```c
void handle_continuous_input(t_game *game)
{
    if (mlx_is_key_down(game->mlx, MLX_KEY_W))
        move_player(game, 1);  // Avancer
    if (mlx_is_key_down(game->mlx, MLX_KEY_S))
        move_player(game, -1); // Reculer
    // etc...
}
```

### Questions pièges :

**Q: Pourquoi deux systèmes différents ?**
- ESC : une seule pression suffit → event hook
- WASD : mouvement fluide → check continu chaque frame
- Évite la répétition de touches (key repeat OS)

**Q: Quelle est la différence entre MLX_PRESS et MLX_DOWN ?**
- MLX_PRESS : événement déclenché UNE FOIS à la pression
- mlx_is_key_down() : état actuel (pressé ou non) vérifié chaque frame

### 5.2. Déplacement : `move_player()` dans `src/controls/movement_utils.c`

**Algorithme :**
```c
1. Calculer nouvelle position = pos actuelle + direction * vitesse
2. Vérifier collision avec les murs
3. Si pas de collision : mettre à jour la position
4. Gestion séparée X et Y (permet de glisser le long des murs)
```

**Détection de collision avec buffer :**
```c
int check_collision_with_buffer(t_game *game, double x, double y)
{
    // Buffer de sécurité = marge autour du joueur
    double buffer = 0.2;
    
    // Vérifier les 4 coins du carré autour du joueur
    if (map[(int)(y - buffer)][(int)(x - buffer)] == '1') return 1;
    if (map[(int)(y - buffer)][(int)(x + buffer)] == '1') return 1;
    if (map[(int)(y + buffer)][(int)(x - buffer)] == '1') return 1;
    if (map[(int)(y + buffer)][(int)(x + buffer)] == '1') return 1;
    
    return 0;
}
```

### Questions pièges :

**Q: Pourquoi un buffer de collision ?**
- R: Sans buffer, le joueur peut être "dans" le mur (position exacte sur '1')
- R: Le buffer crée une "hitbox" autour du joueur
- R: Plus réaliste : le joueur a une taille physique

**Q: Pourquoi séparer les collisions X et Y ?**
- R: Si collision en diagonal, permet de continuer dans une direction
- R: Effet de "glissement" le long des murs (wall sliding)
- R: Plus fluide pour le gameplay

**Q: Que se passe-t-il si MOVE_SPEED est trop grand ?**
- R: Le joueur peut traverser les murs fins (tunneling)
- R: La détection de collision check seulement la position finale
- R: Solution : utiliser une vitesse raisonnable (0.08) ou raycasting pour collision

### 5.3. Rotation : `rotate_player()` dans `src/controls/movements.c`

**Rotation de la caméra (matrice de rotation 2D) :**
```c
void rotate_player(t_game *game, double angle)
{
    // Ancienne direction
    double old_dir_x = player->dir.x;
    double old_plane_x = player->plane.x;
    
    // Nouvelle direction (rotation)
    player->dir.x = old_dir_x * cos(angle) - player->dir.y * sin(angle);
    player->dir.y = old_dir_x * sin(angle) + player->dir.y * cos(angle);
    
    // Nouveau plan de caméra (rotation identique)
    player->plane.x = old_plane_x * cos(angle) - player->plane.y * sin(angle);
    player->plane.y = old_plane_x * sin(angle) + player->plane.y * cos(angle);
}
```

### Questions pièges IMPORTANTES :

**Q: C'est quoi cette formule mathématique ?**
- R: Matrice de rotation 2D classique
- R: `[x', y'] = [x*cos(θ) - y*sin(θ), x*sin(θ) + y*cos(θ)]`
- R: Fait tourner un vecteur de `angle` radians

**Q: Pourquoi tourner aussi le plane ?**
- R: Le plan de caméra doit rester perpendiculaire à la direction
- R: Ils tournent ensemble pour garder la cohérence du FOV
- R: Si on tournait seulement dir, le FOV serait déformé

**Q: Pourquoi stocker old_dir_x et old_plane_x ?**
- R: Car on a besoin de l'ancienne valeur de X pour calculer Y
- R: Si on ne sauvegarde pas, on utiliserait la nouvelle valeur de X (erreur)
- R: Les deux calculs dépendent des valeurs AVANT rotation

---

## 🧹 6. NETTOYAGE ET FERMETURE

### Fonction : `cleanup_game()` dans `src/utils/cleanup.c`

**ORDRE CRITIQUE du nettoyage :**
```c
1. Nettoyer les textures (mlx_delete_texture)
2. Nettoyer la map et ses données
3. Libérer le joueur
4. Terminer MLX (mlx_terminate) ← TOUJOURS EN DERNIER
```

### Questions pièges :

**Q: Pourquoi cet ordre précis ?**
- R: Les textures utilisent MLX → doivent être libérées AVANT mlx_terminate
- R: mlx_terminate ferme la fenêtre et libère le contexte OpenGL
- R: Appeler des fonctions MLX après terminate = segfault

**Q: Que fait mlx_delete_texture() ?**
- R: Libère la mémoire de texture->pixels
- R: Détruit la texture OpenGL sous-jacente
- R: DOIT être appelé avant mlx_terminate()

**Q: Que se passe-t-il si on oublie le cleanup ?**
- R: Memory leaks (Valgrind va crier)
- R: Ressources OpenGL/GPU non libérées
- R: Mauvaise note à la correction 😅

---

## 💡 POINTS CLÉS À RETENIR

### Architecture globale :
```
main()
  └─ init_game()
      ├─ parse_cub_file() [Partie de votre binôme]
      ├─ init_game_state_n_mlx()
      └─ game_loop()
          └─ MLX Loop (infinie)
              └─ game_update() [chaque frame]
                  ├─ handle_continuous_input()
                  └─ render_frame()
                      ├─ draw_floor_ceiling()
                      └─ cast_rays()
                          ├─ init_ray()
                          ├─ perform_dda()
                          └─ draw_walls()
```

### Formules mathématiques importantes :
1. **CameraX** : `2 * x / width - 1` (normalisation pixel → [-1, 1])
2. **Direction rayon** : `dir + plane * cameraX` (rayon dans le FOV)
3. **Delta distance** : `|1 / ray_dir|` (distance entre lignes de grille)
4. **Distance perpendiculaire** : Correction fisheye
5. **Hauteur mur** : `height / distance` (perspective)
6. **Rotation 2D** : `[cos θ, -sin θ; sin θ, cos θ]` (matrice rotation)

### Optimisations possibles (bonus points) :
- Multi-threading du raycasting
- Sprites (ennemis, objets)
- Minimap
- Shadows/lighting
- Textured floor/ceiling

---

## 🎯 QUESTIONS PIÈGES FRÉQUENTES

### Générales :
1. **Pourquoi cub3D et pas un vrai 3D ?**
   - Raycasting = pseudo-3D (2.5D)
   - Pas de calcul de polygones 3D, juste projection 2D
   - Plus rapide, utilisé dans Wolfenstein 3D (1992)

2. **Quelle est la complexité du raycasting ?**
   - O(n * m) où n = largeur écran, m = nombre de cases traversées
   - Généralement très rapide (DDA optimisé)

3. **Pourquoi utiliser MLX42 et pas OpenGL direct ?**
   - MLX42 simplifie l'accès à la fenêtre et aux pixels
   - Utilise OpenGL en backend (abstraction)
   - Parfait pour un projet pédagogique

### Techniques :
4. **Comment gérer plusieurs murs à distances différentes ?**
   - DDA s'arrête au premier mur touché
   - Un seul rayon par colonne suffit (pas de transparence)

5. **Peut-on avoir un FOV de 360° ?**
   - Non, le plan de caméra limite le FOV (~66° par défaut)
   - Pour 360°, il faudrait plusieurs passes de raycasting

6. **Que se passe-t-il si on divise par zéro dans les rayons ?**
   - ray->dir.x ou ray->dir.y = 0 → division par zéro
   - On utilise des valeurs très grandes (1e30) au lieu de diviser
   - Ou on check avant et utilise des formules alternatives

---

## 📚 RESSOURCES POUR APPROFONDIR

- [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) ← BIBLE DU RAYCASTING
- [Wolfenstein 3D Source Code](https://github.com/id-Software/wolf3d) ← Original game
- [MLX42 Documentation](https://github.com/codam-coding-college/MLX42)

---

## ✅ CHECKLIST AVANT LA CORRECTION

- [ ] Je comprends l'algorithme DDA en détail
- [ ] Je peux expliquer la différence entre distance euclidienne et perpendiculaire
- [ ] Je sais pourquoi on utilise un plan de caméra
- [ ] Je comprends le mapping de texture (wallX, tex_x, tex_y)
- [ ] Je peux expliquer la rotation 2D avec cos/sin
- [ ] Je connais l'ordre de nettoyage et pourquoi
- [ ] Je peux dessiner le schéma du raycasting au tableau
- [ ] Je sais gérer les questions sur les optimisations possibles

---

**Bon courage pour la correction ! 🚀**

*N'oubliez pas : si vous bloquez sur une question, reformulez et ramenez à ce que vous savez. Les correcteurs veulent voir que vous comprenez les concepts, pas que vous récitez.*
