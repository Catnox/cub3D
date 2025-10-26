# 🎬 Explication Détaillée de init.c - Initialisation du jeu cub3D

## 🎯 Vue d'ensemble

Le fichier `init.c` contient **3 fonctions essentielles** qui préparent le jeu avant son lancement :

```
1. resize_callback()       → Gère le redimensionnement de la fenêtre
2. init_game_state_n_mlx() → Initialise MLX42 et crée la fenêtre
3. init_game()             → Orchestre TOUTE l'initialisation (fonction maître)
```

**Ordre d'appel** :
```
main()
  └─ init_game()
      ├─ parse_cub_file()          [Parsing - fait par votre binôme]
      ├─ find_player_position()    [Parsing - fait par votre binôme]
      ├─ init_game_state_n_mlx()   [ICI dans init.c]
      ├─ mlx_image_to_window()     [ICI dans init.c]
      └─ mlx_resize_hook()         [ICI dans init.c]
           └─ resize_callback()    [Callback enregistré]
```

---

## 🔧 FONCTION 1 : `resize_callback()`

### 📝 Signature et but

```c
static void resize_callback(int32_t width, int32_t height, void *param)
```

**Rôle** : Appelée automatiquement par MLX42 quand l'utilisateur redimensionne la fenêtre

**Paramètres** :
- `width` : Nouvelle largeur de la fenêtre (en pixels)
- `height` : Nouvelle hauteur de la fenêtre (en pixels)
- `param` : Pointeur générique vers nos données (ici `t_game *game`)

**Pourquoi `static` ?**
- Fonction privée au fichier `init.c`
- Pas besoin de l'exposer dans le header
- Encapsulation (bonne pratique)

### 🔍 Code annoté ligne par ligne

```c
static void	resize_callback(int32_t width, int32_t height, void *param)
{
	t_game		*game;
	mlx_image_t	*new_img;
```

**Déclarations** :
- `game` : Pointeur vers notre structure principale
- `new_img` : Nouvelle image aux nouvelles dimensions

---

```c
	game = (t_game *)param;
```

**Cast du pointeur générique** :
- MLX42 utilise `void *` pour être flexible
- On le reconvertit en `t_game *` pour accéder à nos données

```c
// Explication du void * :
void *param = &game;           // MLX stocke comme void *
t_game *game = (t_game *)param; // On reconvertit

// Pourquoi MLX utilise void * ?
// Car la bibliothèque ne connaît pas nos structures
// Elle peut ainsi fonctionner avec n'importe quel type
```

---

```c
	if (!game || !game->mlx)
		return ;
```

**Vérification de sécurité** :
- `!game` : Le pointeur game est NULL ?
- `!game->mlx` : Le contexte MLX n'existe pas ?
- Si oui → Sortir immédiatement (rien à faire)

**Pourquoi ces vérifications ?**
```c
// Scénario 1 : game = NULL (ne devrait pas arriver)
if (!game)  // Évite un segfault

// Scénario 2 : MLX pas initialisé
// (peut arriver si on ferme pendant l'init)
if (!game->mlx)  // Évite mlx_delete_image() sur NULL
```

---

```c
	if (width <= 0 || height <= 0)
		return ;
```

**Validation des dimensions** :
- Fenêtre trop petite ou dimensions invalides ?
- Si oui → Ne rien faire (garder les anciennes dimensions)

**Cas possibles** :
```c
// width = 0, height = 0  → Fenêtre minimisée (ignoré)
// width = -1             → Valeur invalide (ignoré)
// width = 1, height = 1  → Fenêtre minuscule mais valide (accepté)
```

---

```c
	mlx_delete_image(game->mlx, game->img);
```

**Suppression de l'ancienne image** :
- L'ancienne image a les anciennes dimensions
- Il faut la supprimer avant d'en créer une nouvelle
- `mlx_delete_image()` libère la mémoire GPU

**Détails importants** :
```c
// L'image est stockée dans la mémoire GPU (VRAM)
// mlx_delete_image() fait :
// 1. Libère la texture OpenGL
// 2. Libère le buffer CPU
// 3. Met à jour les structures internes MLX
```

---

```c
	new_img = mlx_new_image(game->mlx, width, height);
	if (!new_img)
		return ;
```

**Création de la nouvelle image** :
- Dimensions = nouvelles dimensions de la fenêtre
- Si échec (mémoire insuffisante) → Sortir

**Que fait `mlx_new_image()` ?**
```c
mlx_image_t *mlx_new_image(mlx_t *mlx, uint32_t width, uint32_t height)
{
    1. Allouer un buffer RGBA (width * height * 4 bytes)
    2. Créer une texture OpenGL
    3. Initialiser à noir (0x000000FF)
    4. Retourner la structure mlx_image_t
}

// Structure mlx_image_t :
typedef struct mlx_image
{
    uint8_t  *pixels;    // Buffer RGBA
    uint32_t width;      // Largeur
    uint32_t height;     // Hauteur
    // ... autres champs internes
} mlx_image_t;
```

---

```c
	game->img = new_img;
```

**Mise à jour du pointeur** :
- Remplacer l'ancienne image par la nouvelle
- Maintenant `game->img` pointe vers l'image redimensionnée

---

```c
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
		mlx_delete_image(game->mlx, game->img);
}
```

**Affichage de la nouvelle image** :
- `mlx_image_to_window()` : Attache l'image à la fenêtre
- Position (0, 0) = coin supérieur gauche
- Si échec → Supprimer l'image (nettoyage)

**Paramètres de `mlx_image_to_window()` :**
```c
int mlx_image_to_window(mlx_t *mlx, mlx_image_t *img, int32_t x, int32_t y)
//                       ^           ^                ^          ^
//                       contexte    image            pos X      pos Y

// x, y = position de l'image dans la fenêtre
// (0, 0) = coin supérieur gauche
// (100, 50) = décalé de 100px à droite, 50px en bas
```

### 🎯 Vue d'ensemble du processus

```
Utilisateur redimensionne fenêtre
         ↓
MLX détecte l'événement
         ↓
Appel de resize_callback(new_width, new_height, game)
         ↓
1. Vérifier game et game->mlx
2. Vérifier width > 0 et height > 0
3. Supprimer ancienne image (old dimensions)
4. Créer nouvelle image (new dimensions)
5. Mettre à jour game->img
6. Afficher nouvelle image dans fenêtre
         ↓
Fenêtre maintenant à la nouvelle taille
Prochaine frame : rendu sur nouvelle image
```

### 💡 Questions pièges

**Q: Pourquoi supprimer l'ancienne image AVANT d'en créer une nouvelle ?**
- R: Pour libérer la mémoire GPU immédiatement
- R: Évite d'avoir deux images en mémoire simultanément
- R: Si on ne delete pas → memory leak

**Q: Que se passe-t-il si `mlx_new_image()` échoue ?**
- R: `new_img = NULL`
- R: On sort de la fonction (return)
- R: L'ancienne image a déjà été supprimée → écran noir
- R: Alternative possible : garder l'ancienne image si échec

**Q: Est-ce que le redimensionnement affecte le gameplay ?**
- R: OUI : La résolution change → plus/moins de rayons à caster
- R: Plus grand = plus de pixels = plus de calculs
- R: Le FOV reste le même, mais la précision change

**Q: `game->img` est-il automatiquement redessiné ?**
- R: NON : L'image est vide (noire) après `mlx_new_image()`
- R: La prochaine frame de `render_frame()` va la remplir
- R: C'est pour ça qu'on peut voir un flash noir au redimensionnement

---

## 🚀 FONCTION 2 : `init_game_state_n_mlx()`

### 📝 Signature et but

```c
int	init_game_state_n_mlx(t_game *game)
```

**Rôle** : Initialise MLX42 et crée la fenêtre + image principale

**Retour** :
- `0` : Succès
- `1` : Échec

### 🔍 Code annoté

```c
int	init_game_state_n_mlx(t_game *game)
{
	if (!game)
		return (1);
```

**Vérification basique** :
- Si `game` est NULL → impossible de continuer
- Retourner 1 (échec)

**Pourquoi cette vérification ?**
- Défensive programming (sécurité)
- Théoriquement, `game` ne devrait jamais être NULL ici
- Mais c'est une bonne pratique de vérifier

---

```c
	game->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D", true);
	if (!game->mlx)
		return (1);
```

**Initialisation de MLX42** :

**Fonction `mlx_init()` :**
```c
mlx_t *mlx_init(int32_t width, int32_t height, const char *title, bool resizable)
//              ^                ^               ^               ^
//              largeur          hauteur         titre fenêtre   redimensionnable?
```

**Paramètres** :
- `WINDOW_WIDTH` : Constante définie (1024 pixels)
- `WINDOW_HEIGHT` : Constante définie (768 pixels)
- `"cub3D"` : Titre de la fenêtre (barre de titre)
- `true` : La fenêtre peut être redimensionnée

**Que fait `mlx_init()` ?**
```c
1. Initialise GLFW (gestionnaire de fenêtres)
2. Crée un contexte OpenGL (rendu 3D/2D)
3. Crée la fenêtre avec les dimensions spécifiées
4. Configure les callbacks de base (fermeture, etc.)
5. Retourne mlx_t * (contexte MLX) ou NULL si échec
```

**Vérification** :
```c
if (!game->mlx)  // mlx_init a échoué
    return (1);  // Impossible de créer la fenêtre
```

**Causes d'échec possibles** :
- Pas de serveur X (SSH sans X11 forwarding)
- Pilotes graphiques manquants
- Mémoire insuffisante
- OpenGL non supporté

---

```c
	game->img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->img)
		return (1);
```

**Création de l'image principale** :
- Buffer de pixels pour le rendu
- Dimensions identiques à la fenêtre
- Si échec → retourner 1

**Rôle de cette image** :
```c
// C'est notre "toile de peinture"
// On va dessiner pixel par pixel avec mlx_put_pixel()
// Exemple :
for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
        mlx_put_pixel(game->img, x, y, color);

// L'image est ensuite affichée dans la fenêtre
```

**Taille en mémoire** :
```c
// Résolution : 1024 x 768
// Format : RGBA (4 bytes par pixel)
// Taille : 1024 * 768 * 4 = 3,145,728 bytes ≈ 3 MB

// C'est alloué sur le GPU (VRAM)
```

---

```c
	return (0);
}
```

**Succès** :
- MLX initialisé ✓
- Fenêtre créée ✓
- Image créée ✓
- Retourner 0 (succès)

### 💡 Questions pièges

**Q: Pourquoi initialiser MLX APRÈS le parsing ?**
- R: Si le fichier .cub est invalide, pas besoin de fenêtre
- R: Principe "fail fast" : détecter les erreurs simples d'abord
- R: Économie de ressources (pas de fenêtre si pas de map)

**Q: Que se passe-t-il si on oublie de delete l'image/MLX ?**
- R: Memory leak (fuite mémoire GPU)
- R: Fenêtre reste ouverte après exit du programme
- R: Ressources OpenGL non libérées

**Q: Peut-on changer la résolution plus tard ?**
- R: OUI via `resize_callback()`
- R: Ou en recréant une image manuellement
- R: Mais la fenêtre reste aux dimensions initiales (sauf resize)

**Q: Pourquoi `true` pour resizable ?**
- R: Permet à l'utilisateur d'adapter la taille à son écran
- R: Flexibilité (petit écran vs grand écran)
- R: Si `false`, la fenêtre serait fixe

**Q: L'image est-elle automatiquement affichée ?**
- R: NON : `mlx_new_image()` crée seulement l'image
- R: Il faut `mlx_image_to_window()` pour l'afficher
- R: C'est fait dans `init_game()` (fonction maître)

---

## 🎮 FONCTION 3 : `init_game()` (Fonction Maître)

### 📝 Signature et but

```c
int	init_game(t_game *game, char *map_file)
```

**Rôle** : **ORCHESTRE TOUTE L'INITIALISATION** du jeu

**Paramètres** :
- `game` : Structure principale (déjà allouée dans main)
- `map_file` : Chemin vers le fichier .cub (ex: "maps/test.cub")

**Retour** :
- `0` : Succès total
- `1` : Échec à une étape (avec message d'erreur)

### 🔍 Code annoté - Étape par étape

```c
int	init_game(t_game *game, char *map_file)
{
```

---

### **ÉTAPE 1 : Parsing du fichier .cub**

```c
	if (parse_cub_file(game, map_file) != 0)
		return (error_exit_init(game, "Failed to parse .cub file"));
```

**Fonction `parse_cub_file()`** (fait par votre binôme) :
```c
int parse_cub_file(t_game *game, char *map_file)
{
    1. Ouvrir et lire le fichier .cub
    2. Parser les textures (NO, SO, WE, EA)
    3. Parser les couleurs (F, C)
    4. Parser la map (grille de 0 et 1)
    5. Valider tout (textures existent, map fermée, etc.)
    
    return 0;  // Succès
    return 1;  // Échec
}
```

**Si échec** :
- Appel de `error_exit_init(game, "Failed to parse .cub file")`
- Cette fonction :
  1. Affiche "Error\nFailed to parse .cub file"
  2. Nettoie ce qui a été alloué
  3. Retourne 1

**Pourquoi parser EN PREMIER ?**
```
Parsing → Données du jeu (map, textures, couleurs)
   ↓
Sans ces données, impossible de créer le monde 3D
   ↓
Donc on vérifie d'abord que le fichier est valide
```

---

### **ÉTAPE 2 : Trouver la position du joueur**

```c
	if (find_player_position(game) != 0)
		return (error_exit_init(game, "Failed to find player position"));
```

**Fonction `find_player_position()`** (fait par votre binôme) :
```c
int find_player_position(t_game *game)
{
    // Parcourir la map et chercher N, S, E ou W
    for (int y = 0; y < map_height; y++)
        for (int x = 0; x < map_width; x++)
            if (map[y][x] == 'N' || map[y][x] == 'S' || ...)
            {
                game->player->pos.x = x + 0.5;  // Centre de la case
                game->player->pos.y = y + 0.5;
                set_player_direction(game->player, map[y][x]);
                return 0;  // Trouvé !
            }
    
    return 1;  // Pas de joueur trouvé (erreur)
}
```

**Initialisation de la direction** :
```c
// Si le joueur est 'N' (regarde vers le Nord)
player->dir.x = 0.0;   // Direction : vers le haut
player->dir.y = -1.0;

player->plane.x = 0.66;  // Plan de caméra : perpendiculaire
player->plane.y = 0.0;

// Pourquoi 0.66 ?
// C'est le FOV (Field Of View)
// 0.66 ≈ 66° de champ de vision
```

**Si échec** : Pas de N/S/E/W dans la map → erreur

---

### **ÉTAPE 3 : Initialiser MLX et créer la fenêtre**

```c
	if (init_game_state_n_mlx(game) != 0)
		return (error_exit_init(game,
				"Failed to initialize game state and MLX42"));
```

**Appel de la fonction vue précédemment** :
- Crée le contexte MLX (`mlx_init`)
- Crée l'image principale (`mlx_new_image`)
- Si échec → Nettoyage et sortie

**État de `game` à ce stade** :
```c
game->mlx     = [Contexte MLX valide]
game->img     = [Image 1024x768 allouée]
game->map     = [Map parsée avec textures et couleurs]
game->player  = [Position et direction initialisées]
```

---

### **ÉTAPE 4 : Afficher l'image dans la fenêtre**

```c
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
		return (error_exit_init(game, "Failed to display image"));
```

**Attacher l'image à la fenêtre** :
- `mlx_image_to_window()` : Rend l'image visible
- Position (0, 0) : Coin supérieur gauche
- Si échec (retour < 0) → Erreur

**Différence important** :
```c
// AVANT mlx_image_to_window()
Fenêtre : [Vide, noire]
Image : [Existe en mémoire mais pas affichée]

// APRÈS mlx_image_to_window()
Fenêtre : [Affiche game->img]
Image : [Visible, mise à jour automatiquement]

// Maintenant, tout mlx_put_pixel() sur game->img
// sera automatiquement affiché dans la fenêtre !
```

**Pourquoi peut-il échouer ?**
- Contexte MLX invalide
- Image NULL
- Erreur interne MLX/OpenGL

---

### **ÉTAPE 5 : Enregistrer le callback de redimensionnement**

```c
	mlx_resize_hook(game->mlx, resize_callback, game);
```

**Enregistrement du callback** :
- Quand la fenêtre est redimensionnée → appel de `resize_callback()`
- Le paramètre `game` sera passé au callback

**Syntaxe** :
```c
void mlx_resize_hook(mlx_t *mlx, mlx_resizefunc func, void *param)
//                   ^           ^                   ^
//                   contexte    fonction callback   données passées

// Notre cas :
mlx_resize_hook(game->mlx, resize_callback, game);
//              ^           ^                ^
//              contexte    notre fonction   données du jeu
```

**Mécanisme des callbacks** :
```
Utilisateur redimensionne fenêtre
         ↓
MLX détecte l'événement
         ↓
MLX appelle : resize_callback(new_width, new_height, game)
         ↓
Notre fonction s'exécute
         ↓
Fenêtre redimensionnée avec succès
```

---

```c
	return (0);
}
```

**SUCCÈS TOTAL** :
- ✅ Parsing réussi
- ✅ Joueur trouvé
- ✅ MLX initialisé
- ✅ Image affichée
- ✅ Callback enregistré
- → Le jeu est prêt à démarrer !

### 🔄 Diagramme du flux d'initialisation

```
init_game(game, "maps/test.cub")
    ↓
┌─────────────────────────────────────────────┐
│ ÉTAPE 1 : parse_cub_file()                  │
│  ├─ Lire fichier                            │
│  ├─ Parser textures (NO/SO/WE/EA)           │
│  ├─ Parser couleurs (F/C)                   │
│  ├─ Parser map (grid de 0/1)                │
│  └─ Valider tout                            │
│                                             │
│ Succès → game->map remplie                  │
│ Échec → error_exit_init() → return 1        │
└─────────────────────────────────────────────┘
    ↓
┌─────────────────────────────────────────────┐
│ ÉTAPE 2 : find_player_position()            │
│  ├─ Chercher N/S/E/W dans map               │
│  ├─ Définir pos.x, pos.y                    │
│  ├─ Définir dir (direction)                 │
│  └─ Définir plane (FOV)                     │
│                                             │
│ Succès → game->player initialisé            │
│ Échec → error_exit_init() → return 1        │
└─────────────────────────────────────────────┘
    ↓
┌─────────────────────────────────────────────┐
│ ÉTAPE 3 : init_game_state_n_mlx()          │
│  ├─ mlx_init() → Créer contexte             │
│  └─ mlx_new_image() → Créer buffer          │
│                                             │
│ Succès → game->mlx et game->img valides     │
│ Échec → error_exit_init() → return 1        │
└─────────────────────────────────────────────┘
    ↓
┌─────────────────────────────────────────────┐
│ ÉTAPE 4 : mlx_image_to_window()            │
│  └─ Attacher image à fenêtre                │
│                                             │
│ Succès → Image visible                      │
│ Échec → error_exit_init() → return 1        │
└─────────────────────────────────────────────┘
    ↓
┌─────────────────────────────────────────────┐
│ ÉTAPE 5 : mlx_resize_hook()                │
│  └─ Enregistrer callback                    │
│                                             │
│ Toujours succès                             │
└─────────────────────────────────────────────┘
    ↓
return (0) → Succès total !
    ↓
Retour dans main()
    ↓
game_loop(game) → Démarrer le jeu
```

### 💡 Questions pièges IMPORTANTES

**Q: Pourquoi cet ordre précis d'initialisation ?**
```
1. Parsing     → Besoin des données (map, textures)
2. Player      → Besoin de la map pour trouver N/S/E/W
3. MLX         → Besoin des textures pour les charger
4. Display     → Besoin de MLX et image pour afficher
5. Callbacks   → Besoin de tout le reste pour fonctionner
```
**R: Dépendances en cascade - chaque étape a besoin de la précédente**

**Q: Que fait `error_exit_init()` ?**
```c
int error_exit_init(t_game *game, char *msg)
{
    printf("Error\n");
    ft_putendl_fd(msg, 2);  // Afficher sur STDERR
    cleanup_game(game);     // Libérer ressources partielles
    return (1);             // Retourner échec
}
```

**Q: Pourquoi ne pas tout initialiser dans main() ?**
- R: Séparation des responsabilités (clean code)
- R: Main reste simple et lisible
- R: init_game() peut être testée séparément
- R: Plus facile à déboguer

**Q: Que se passe-t-il si on initialise MLX avant le parsing ?**
- R: Pas d'erreur technique, mais inefficace
- R: Si parsing échoue, on a créé une fenêtre pour rien
- R: Gaspillage de ressources
- R: Mauvaise UX (fenêtre s'ouvre puis erreur)

**Q: L'image est-elle dessinée à l'initialisation ?**
- R: NON : Elle est noire (pixels à 0)
- R: Le premier rendu sera fait par `render_frame()` dans la game loop
- R: On verra un flash noir puis le jeu apparaît

**Q: Peut-on initialiser plusieurs fenêtres ?**
- R: Techniquement OUI (plusieurs `mlx_init`)
- R: Mais pas dans ce projet (une seule fenêtre = un jeu)
- R: Chaque fenêtre aurait son propre contexte MLX

---

## 🎯 Ordre critique et dépendances

### Graphe des dépendances

```
game (allouée dans main)
    ↓
parse_cub_file()
    ├─ Alloue game->map
    ├─ Charge textures dans map->textures
    └─ Parse couleurs dans map->floor_color, map->ceiling_color
    ↓
find_player_position()
    ├─ Alloue game->player
    └─ Utilise game->map (a besoin du parsing)
    ↓
init_game_state_n_mlx()
    ├─ Crée game->mlx (contexte)
    └─ Crée game->img (image)
    ↓
mlx_image_to_window()
    └─ Utilise game->mlx et game->img (a besoin de MLX)
    ↓
mlx_resize_hook()
    └─ Utilise game->mlx (a besoin de MLX)
```

### Si on changeait l'ordre ?

| Changement | Conséquence |
|------------|-------------|
| MLX avant parsing | Fenêtre inutile si parsing échoue |
| Player avant parsing | Impossible de trouver N/S/E/W (map pas encore parsée) |
| Display avant MLX | Segfault (game->mlx est NULL) |
| Hooks avant display | Fonctionne mais illogique |

---

## 🧹 Gestion des erreurs et nettoyage

### Niveaux d'initialisation et nettoyage

```c
// Scénario 1 : Échec au parsing
init_game()
  └─ parse_cub_file() ÉCHOUE
      └─ error_exit_init()
          ├─ cleanup_game()
          │   └─ game->map (partiellement alloué)
          │   └─ game->player = NULL (pas encore alloué)
          │   └─ game->mlx = NULL (pas encore créé)
          └─ return 1

// Scénario 2 : Échec à find_player
init_game()
  ├─ parse_cub_file() ✓
  └─ find_player_position() ÉCHOUE
      └─ error_exit_init()
          ├─ cleanup_game()
          │   ├─ Nettoyer game->map (alloué)
          │   ├─ Nettoyer game->player (peut être partiellement alloué)
          │   └─ game->mlx = NULL (pas encore créé)
          └─ return 1

// Scénario 3 : Échec à MLX init
init_game()
  ├─ parse_cub_file() ✓
  ├─ find_player_position() ✓
  └─ init_game_state_n_mlx() ÉCHOUE
      └─ error_exit_init()
          ├─ cleanup_game()
          │   ├─ Nettoyer game->map (alloué)
          │   ├─ Nettoyer game->player (alloué)
          │   └─ game->mlx = NULL (échec création)
          └─ return 1

// Scénario 4 : Tout réussit !
init_game()
  ├─ parse_cub_file() ✓
  ├─ find_player_position() ✓
  ├─ init_game_state_n_mlx() ✓
  ├─ mlx_image_to_window() ✓
  ├─ mlx_resize_hook() ✓
  └─ return 0 → Succès !
```

### Fonction `cleanup_game()`

```c
void cleanup_game(t_game *game)
{
    // 1. Nettoyer textures (si map existe)
    if (game->map)
        clean_textures(game);  // mlx_delete_texture()
    
    // 2. Nettoyer map
    if (game->map)
    {
        if (game->map->grid)
            free_split(game->map->grid);  // Libérer chaque ligne
        free(game->map);
    }
    
    // 3. Nettoyer player
    if (game->player)
        free(game->player);
    
    // 4. Terminer MLX (TOUJOURS EN DERNIER)
    if (game->mlx)
        mlx_terminate(game->mlx);
}
```

**ORDRE CRUCIAL** :
1. Textures AVANT mlx_terminate (utilisent OpenGL)
2. Map et player (ordre indifférent)
3. MLX EN DERNIER (ferme tout le contexte)

---

## 📚 Résumé en 5 points

1. **`resize_callback()`** : Redimensionne l'image quand la fenêtre change
2. **`init_game_state_n_mlx()`** : Crée le contexte MLX et l'image principale
3. **`init_game()`** : Orchestre TOUTE l'initialisation dans le bon ordre
4. **Ordre critique** : Parsing → Player → MLX → Display → Callbacks
5. **Gestion d'erreur** : Nettoyage progressif selon ce qui a été alloué

---

## ✅ Checklist de compréhension

- [ ] Je comprends pourquoi `resize_callback` est static
- [ ] Je sais ce que fait `mlx_init()` et `mlx_new_image()`
- [ ] Je peux expliquer l'ordre des 5 étapes d'initialisation
- [ ] Je comprends pourquoi parser AVANT MLX
- [ ] Je sais ce que fait `error_exit_init()`
- [ ] Je peux dessiner le graphe des dépendances
- [ ] Je connais la différence entre `mlx_new_image()` et `mlx_image_to_window()`
- [ ] Je comprends le mécanisme des callbacks (hooks)

---

**L'initialisation est le fondement du jeu. Un bon `init_game()` = un jeu stable ! 🏗️**
