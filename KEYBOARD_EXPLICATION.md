# KEYBOARD.C - EXPLICATION DÉTAILLÉE

## 📋 TABLE DES MATIÈRES

1. [Vue d'ensemble](#vue-densemble)
2. [Fonction on_close()](#fonction-on_close)
3. [Fonction handle_keyboard()](#fonction-handle_keyboard)
4. [Fonction handle_movement_rotation()](#fonction-handle_movement_rotation)
5. [Fonction handle_continuous_input()](#fonction-handle_continuous_input)
6. [Système d'input MLX42](#système-dinput-mlx42)
7. [Questions fréquentes](#questions-fréquentes)
8. [Diagrammes](#diagrammes)

---

## 🎯 VUE D'ENSEMBLE

### Rôle du fichier
`keyboard.c` gère **tous les événements clavier** du jeu :
- **Événements ponctuels** : ESC pour quitter
- **Mouvements continus** : WASD pour se déplacer
- **Rotations continues** : ← → pour tourner
- **Fermeture de fenêtre** : Nettoyage propre

### Fonctions principales
```
keyboard.c
├── on_close()                    (public)  - Fermeture fenêtre
├── handle_keyboard()             (public)  - Événements ponctuels
├── handle_movement_rotation()    (static)  - Mouvements/rotations
└── handle_continuous_input()     (public)  - Coordinateur input
```

### Deux systèmes d'input

| Type | Fonction | Utilisé pour | Fréquence |
|------|----------|--------------|-----------|
| **Événementiel** | `handle_keyboard()` | Actions uniques (ESC) | Par appui/relâchement |
| **Polling** | `handle_continuous_input()` | Mouvements fluides (WASD) | ~60 fois/seconde |

### Dépendances
```c
#include "cub3d.h"
```
- **MLX42** : `mlx_is_key_down()`, `mlx_key_data_t`
- **Movements** : `move_player()`, `rotate_player()`
- **Utils** : `cleanup_game()`

---

## 🚪 FONCTION ON_CLOSE()

### Signature
```c
void on_close(void *param)
```

### Vue d'ensemble
Cette fonction est appelée quand l'utilisateur ferme la fenêtre (clic sur ❌). Elle assure un **nettoyage propre** avant de quitter.

### Analyse ligne par ligne

```c
void	on_close(void *param)
{
```
**Fonction publique** : Appelée par MLX42 via `mlx_close_hook()`
**`void *param`** : Pointeur générique (standard MLX42 pour les callbacks)

```c
	t_game	*game;
```
**Déclaration** : Variable locale pour stocker le pointeur vers la structure du jeu.

```c
	game = (t_game *)param;
```
**Cast du paramètre** :
- `param` est un `void*` (pointeur générique)
- On le convertit en `t_game*` pour accéder à la structure
- Ce pointeur a été passé lors de l'enregistrement : `mlx_close_hook(game->mlx, on_close, game)`

```c
	cleanup_game(game);
```
**Nettoyage complet** :
- Libère les textures MLX42
- Libère la map et ses structures
- Ferme la fenêtre MLX42
- Libère toute la mémoire allouée

**Ordre important** : Nettoyer AVANT de terminer le programme

```c
	exit(1);
```
**Terminaison du programme** :
- `exit(1)` : Code de retour 1 (par convention pour fermeture utilisateur)
- Termine immédiatement le programme
- Toute la mémoire a déjà été libérée par `cleanup_game()`

```c
}
```

### Quand est-elle appelée ?

**Scénarios** :
1. ✅ Clic sur le bouton ❌ de la fenêtre
2. ✅ Alt+F4 (Windows) ou Cmd+W (macOS)
3. ✅ Commande système de fermeture

**Enregistrement dans game_loop.c** :
```c
mlx_close_hook(game->mlx, on_close, game);
```

### Pourquoi exit(1) et pas exit(0) ?

```c
exit(1);  // Code de retour 1
```

**Convention** :
- `exit(0)` : Terminaison normale du programme
- `exit(1)` : Terminaison par fermeture utilisateur

Dans notre cas, l'utilisateur ferme la fenêtre manuellement, donc `exit(1)` est approprié.

---

## ⌨️ FONCTION HANDLE_KEYBOARD()

### Signature
```c
void handle_keyboard(mlx_key_data_t keydata, void *param)
```

### Vue d'ensemble
Gère les **événements clavier ponctuels** (appui/relâchement de touches). Actuellement utilisée uniquement pour la touche **ESC** (quitter le jeu).

### Analyse ligne par ligne

```c
void	handle_keyboard(mlx_key_data_t keydata, void *param)
{
```
**Fonction publique** : Appelée par MLX42 via `mlx_key_hook()`

**Paramètres** :
1. `mlx_key_data_t keydata` : Structure contenant les infos de l'événement clavier
2. `void *param` : Pointeur générique vers game

**Structure mlx_key_data_t** :
```c
typedef struct mlx_key_data
{
    mlx_key_t      key;       // MLX_KEY_ESCAPE, MLX_KEY_W, etc.
    mlx_action_t   action;    // MLX_PRESS, MLX_RELEASE, MLX_REPEAT
    mlx_modifier_t modifier;  // MLX_SHIFT, MLX_CONTROL, MLX_ALT
} mlx_key_data_t;
```

```c
	t_game	*game;
```
**Déclaration** : Variable locale pour la structure du jeu.

```c
	game = (t_game *)param;
```
**Cast du paramètre** : Conversion de `void*` vers `t_game*`

```c
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
```
**Vérification double condition** :

**Condition 1** : `keydata.key == MLX_KEY_ESCAPE`
- Vérifie que la touche pressée est **ESC**
- `MLX_KEY_ESCAPE` : Constante MLX42 pour la touche Escape

**Condition 2** : `keydata.action == MLX_PRESS`
- Vérifie que c'est un **appui** (pas un relâchement)
- `MLX_PRESS` : Constante MLX42 pour l'action "appuyer"

**Pourquoi les deux ?**
- Sans `keydata.action == MLX_PRESS`, le code s'exécuterait aussi au **relâchement** de ESC
- On veut quitter DÈS l'appui, pas au relâchement

**Actions possibles** :
```c
MLX_PRESS    // Touche enfoncée
MLX_RELEASE  // Touche relâchée
MLX_REPEAT   // Touche maintenue (répétition)
```

```c
		cleanup_game(game);
```
**Nettoyage complet** :
- Libère les textures MLX42 (`mlx_delete_texture()`)
- Libère l'image principale (`mlx_delete_image()`)
- Ferme la fenêtre MLX42 (`mlx_terminate()`)
- Libère la map et toutes ses structures
- Libère la structure game

**Ordre critique** :
1. ✅ Textures AVANT mlx_terminate
2. ✅ Images AVANT mlx_terminate
3. ✅ mlx_terminate AVANT free de game
4. ✅ Map et autres structures à la fin

```c
		exit(0);
```
**Terminaison normale** :
- `exit(0)` : Code de retour 0 (succès)
- Différence avec `on_close()` qui utilise `exit(1)`
- ESC = terminaison volontaire normale → exit(0)

```c
	}
}
```

### Différence exit(0) vs exit(1)

```c
// Dans handle_keyboard() (ESC)
cleanup_game(game);
exit(0);  // ✅ Terminaison NORMALE (utilisateur quitte via ESC)

// Dans on_close() (fermeture fenêtre)
cleanup_game(game);
exit(1);  // ⚠️  Terminaison par fermeture fenêtre
```

**Convention Unix** :
- `0` : Succès / Terminaison normale
- `1` : Erreur / Terminaison anormale

### Pourquoi pas gérer WASD ici ?

```c
// ❌ MAUVAIS - Ne pas faire ça
void handle_keyboard(mlx_key_data_t keydata, void *param)
{
    if (keydata.key == MLX_KEY_W && keydata.action == MLX_PRESS)
        move_player(game, 1);  // Mouvement saccadé !
}
```

**Problème** :
- Appelé UNE SEULE FOIS à l'appui
- Mouvement unique et saccadé
- Pas de mouvement continu

```c
// ✅ BON - Utiliser le polling
void handle_continuous_input(t_game *game)
{
    if (mlx_is_key_down(game->mlx, MLX_KEY_W))
        move_player(game, 1);  // Appelé 60 fois/sec = fluide
}
```

**Avantage** :
- Appelé CHAQUE FRAME (~60 fois/sec)
- Mouvement fluide et continu
- Permet appuis simultanés (W + D)

---

## 🎮 FONCTION HANDLE_MOVEMENT_ROTATION()

### Signature
```c
static void handle_movement_rotation(t_game *game)
```

### Vue d'ensemble
Fonction **statique** qui gère tous les **mouvements** (WASD) et **rotations** (← →) du joueur. Utilise le **polling** pour des déplacements fluides.

### Analyse ligne par ligne

```c
static void	handle_movement_rotation(t_game *game)
{
```
**`static`** : Fonction privée au fichier (portée locale)
- ✅ Encapsulation : Pas besoin de l'exposer à d'autres fichiers
- ✅ Organisation : Fonction auxiliaire de `handle_continuous_input()`

**Paramètre** : `t_game *game` - Structure complète du jeu

```c
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move_player(game, 1);
```
**Mouvement AVANT (W)** :

**`mlx_is_key_down()`** : Fonction MLX42 de polling
- Retourne `true` si la touche est **actuellement enfoncée**
- Retourne `false` sinon
- Vérifié ~60 fois par seconde (chaque frame)

**`move_player(game, 1)`** :
- Fonction dans `movements.c`
- Paramètre `1` : Code pour "avancer"
- Déplace le joueur dans la direction de son regard
- Applique `MOVE_SPEED` (0.08) pour un mouvement fluide
- Vérifie les collisions avec les murs

**Pseudo-code de move_player(game, 1)** :
```c
void move_player(t_game *game, int direction)
{
    if (direction == 1)  // Avancer
    {
        new_x = game->player->x + game->player->dir_x * MOVE_SPEED;
        new_y = game->player->y + game->player->dir_y * MOVE_SPEED;
        
        if (is_walkable(game, new_x, new_y))
        {
            game->player->x = new_x;
            game->player->y = new_y;
        }
    }
}
```

```c
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move_player(game, -1);
```
**Mouvement ARRIÈRE (S)** :

**`move_player(game, -1)`** :
- Paramètre `-1` : Code pour "reculer"
- Déplace le joueur dans la direction **opposée** à son regard
- Même logique que avancer, mais direction inversée

**Différence avec W** :
```
W (1)  : Position += Direction * MOVE_SPEED
S (-1) : Position -= Direction * MOVE_SPEED
```

```c
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move_player(game, 2);
```
**Mouvement GAUCHE (A)** :

**`move_player(game, 2)`** :
- Paramètre `2` : Code pour "strafe gauche"
- Déplace le joueur **perpendiculairement** à son regard (à gauche)
- Utilise le vecteur de plan de la caméra

**Calcul du strafe gauche** :
```c
// Direction de regard
dir_x = cos(angle)
dir_y = sin(angle)

// Vecteur perpendiculaire gauche (rotation -90°)
strafe_left_x = -dir_y
strafe_left_y = +dir_x

// Déplacement
new_x = x + strafe_left_x * MOVE_SPEED
new_y = y + strafe_left_y * MOVE_SPEED
```

```c
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move_player(game, 3);
```
**Mouvement DROITE (D)** :

**`move_player(game, 3)`** :
- Paramètre `3` : Code pour "strafe droite"
- Déplace le joueur **perpendiculairement** à son regard (à droite)
- Inverse du strafe gauche

**Calcul du strafe droite** :
```c
// Vecteur perpendiculaire droite (rotation +90°)
strafe_right_x = +dir_y
strafe_right_y = -dir_x
```

```c
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate_player(game, -ROT_SPEED);
```
**Rotation GAUCHE (←)** :

**`rotate_player(game, -ROT_SPEED)`** :
- Fonction dans `movements.c`
- `-ROT_SPEED` : Vitesse de rotation négative (sens horaire inversé)
- `ROT_SPEED` défini dans `cub3d.h` (typiquement 0.025 radians)

**Qu'est-ce qu'une rotation ?**
```c
// Rotation du vecteur direction
old_dir_x = player->dir_x;
player->dir_x = player->dir_x * cos(angle) - player->dir_y * sin(angle);
player->dir_y = old_dir_x * sin(angle) + player->dir_y * cos(angle);

// Rotation du vecteur plan (caméra)
old_plane_x = player->plane_x;
player->plane_x = player->plane_x * cos(angle) - player->plane_y * sin(angle);
player->plane_y = old_plane_x * sin(angle) + player->plane_y * cos(angle);
```

**Pourquoi -ROT_SPEED ?**
- En trigonométrie, angle positif = rotation anti-horaire
- Flèche gauche = tourner à gauche = rotation horaire
- Donc on utilise un angle **négatif**

```c
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_player(game, ROT_SPEED);
```
**Rotation DROITE (→)** :

**`rotate_player(game, ROT_SPEED)`** :
- `+ROT_SPEED` : Vitesse de rotation positive (sens anti-horaire)
- Flèche droite = tourner à droite = rotation anti-horaire
- Angle positif

```c
}
```

### Codes de direction pour move_player()

```c
move_player(game, direction);

direction == 1   → Avancer    (W)
direction == -1  → Reculer    (S)
direction == 2   → Gauche     (A)
direction == 3   → Droite     (D)
```

### Appuis simultanés

**Avantage du polling** :
```c
// L'utilisateur appuie sur W + D simultanément
if (mlx_is_key_down(game->mlx, MLX_KEY_W))  // true
    move_player(game, 1);                    // Avance
if (mlx_is_key_down(game->mlx, MLX_KEY_D))  // true
    move_player(game, 3);                    // Strafe droite

// Résultat : Déplacement en DIAGONALE (avant-droite)
```

**Combinaisons possibles** :
- W + A = Avant-gauche
- W + D = Avant-droite
- S + A = Arrière-gauche
- S + D = Arrière-droite
- W + ← = Avancer et tourner à gauche
- Etc.

---

## 🎯 FONCTION HANDLE_CONTINUOUS_INPUT()

### Signature
```c
void handle_continuous_input(t_game *game)
```

### Vue d'ensemble
Fonction **publique** qui coordonne tous les inputs continus. C'est le **point d'entrée principal** pour le traitement des touches maintenues.

### Analyse ligne par ligne

```c
void	handle_continuous_input(t_game *game)
{
```
**Fonction publique** :
- Appelée depuis `game_update()` dans `game_loop.c`
- Fréquence : ~60 fois par seconde (chaque frame)

```c
	handle_movement_rotation(game);
```
**Appel de la fonction auxiliaire** :
- Délègue le traitement des mouvements/rotations
- Organisation modulaire du code

```c
}
```

### Pourquoi cette fonction existe ?

**Architecture actuelle** :
```c
handle_continuous_input(game)
    └─→ handle_movement_rotation(game)
```

**Pourquoi ne pas appeler directement handle_movement_rotation() ?**

**Raison 1 : Extensibilité**
```c
// Facile d'ajouter d'autres types d'input
void handle_continuous_input(t_game *game)
{
    handle_movement_rotation(game);
    handle_combat(game);        // Ajout futur
    handle_interactions(game);  // Ajout futur
}
```

**Raison 2 : Point d'entrée unique**
```c
// Dans game_loop.c
game_update(void *param)
{
    // Un seul appel pour tout l'input continu
    handle_continuous_input(game);
    render_frame(game);
}
```

**Raison 3 : Séparation des responsabilités**
- `handle_continuous_input()` : Coordinateur
- `handle_movement_rotation()` : Implémentation spécifique

### Ordre d'appel dans le programme

```
game_loop()
  ↓
mlx_loop_hook(game->mlx, game_update, game)
  ↓
[BOUCLE INFINIE - 60 FPS]
  ↓
game_update(game)
  ↓
handle_continuous_input(game)  ← NOUS SOMMES ICI
  ↓
handle_movement_rotation(game)
  ↓
  ├─→ mlx_is_key_down(W) ? → move_player(1)
  ├─→ mlx_is_key_down(S) ? → move_player(-1)
  ├─→ mlx_is_key_down(A) ? → move_player(2)
  ├─→ mlx_is_key_down(D) ? → move_player(3)
  ├─→ mlx_is_key_down(←) ? → rotate_player(-ROT_SPEED)
  └─→ mlx_is_key_down(→) ? → rotate_player(ROT_SPEED)
```

---

## 🔄 SYSTÈME D'INPUT MLX42

### Deux approches d'input

#### 1. Événementiel (mlx_key_hook)

```c
void handle_keyboard(mlx_key_data_t keydata, void *param)
{
    // Appelé UNE FOIS par appui/relâchement
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
        exit(0);
}
```

**Caractéristiques** :
- ✅ Déclenché par événements (appui, relâchement)
- ✅ Reçoit des informations détaillées (action, modifier)
- ✅ Idéal pour actions ponctuelles
- ❌ Pas adapté pour mouvements continus

**Quand l'utiliser ?**
- Quitter le jeu (ESC)
- Pause (P)
- Tir unique (SPACE)
- Menu (M)
- Screenshot (F12)

#### 2. Polling (mlx_is_key_down)

```c
void handle_continuous_input(t_game *game)
{
    // Appelé CHAQUE FRAME (~60 fois/sec)
    if (mlx_is_key_down(game->mlx, MLX_KEY_W))
        move_player(game, 1);
}
```

**Caractéristiques** :
- ✅ Vérifié en continu (chaque frame)
- ✅ Permet mouvements fluides
- ✅ Supporte appuis simultanés
- ❌ Pas d'info sur l'événement (juste l'état)

**Quand l'utiliser ?**
- Mouvements (WASD)
- Rotations (← →)
- Zoom continu
- Actions maintenues

### Comparaison pratique

**Scénario** : Joueur maintient W pendant 1 seconde

```
┌─────────────────────────────────────────────────────────┐
│              mlx_key_hook (Événementiel)                │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  T=0ms    : handle_keyboard() → keydata.action = PRESS │
│  T=16ms   : (rien)                                      │
│  T=32ms   : (rien)                                      │
│  ...                                                    │
│  T=1000ms : handle_keyboard() → keydata.action = RELEASE│
│                                                         │
│  Total : 2 appels (PRESS + RELEASE)                    │
│  Résultat : Mouvement saccadé (1 seul déplacement)     │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│           mlx_is_key_down (Polling)                     │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  T=0ms    : mlx_is_key_down(W) = true  → move_player()  │
│  T=16ms   : mlx_is_key_down(W) = true  → move_player()  │
│  T=32ms   : mlx_is_key_down(W) = true  → move_player()  │
│  ...                                                    │
│  T=984ms  : mlx_is_key_down(W) = true  → move_player()  │
│  T=1000ms : mlx_is_key_down(W) = false → (rien)         │
│                                                         │
│  Total : ~60 appels (chaque frame)                     │
│  Résultat : Mouvement fluide et continu                │
└─────────────────────────────────────────────────────────┘
```

### Structure mlx_key_data_t

```c
typedef struct mlx_key_data
{
    mlx_key_t      key;       // Touche concernée
    mlx_action_t   action;    // Type d'action
    mlx_modifier_t modifier;  // Touches modificatrices
} mlx_key_data_t;
```

**Champs détaillés** :

#### key (mlx_key_t)
```c
MLX_KEY_ESCAPE, MLX_KEY_ENTER, MLX_KEY_SPACE
MLX_KEY_W, MLX_KEY_A, MLX_KEY_S, MLX_KEY_D
MLX_KEY_LEFT, MLX_KEY_RIGHT, MLX_KEY_UP, MLX_KEY_DOWN
MLX_KEY_0 à MLX_KEY_9
MLX_KEY_F1 à MLX_KEY_F12
// ... et beaucoup d'autres
```

#### action (mlx_action_t)
```c
MLX_PRESS    // Touche vient d'être enfoncée
MLX_RELEASE  // Touche vient d'être relâchée
MLX_REPEAT   // Touche maintenue (répétition auto)
```

#### modifier (mlx_modifier_t)
```c
MLX_SHIFT    // Shift enfoncé
MLX_CONTROL  // Ctrl enfoncé
MLX_ALT      // Alt enfoncé
MLX_SUPER    // Windows/Cmd enfoncé
```

**Exemple d'utilisation des modifiers** :
```c
void handle_keyboard(mlx_key_data_t keydata, void *param)
{
    // Ctrl + S pour sauvegarder
    if (keydata.key == MLX_KEY_S && 
        keydata.action == MLX_PRESS &&
        keydata.modifier & MLX_CONTROL)
    {
        save_game(game);
    }
}
```

### Enregistrement des hooks

**Dans game_loop.c** :
```c
void game_loop(t_game *game)
{
    // Hook événementiel
    mlx_key_hook(game->mlx, handle_keyboard, game);
    
    // Hook de boucle (pour le polling)
    mlx_loop_hook(game->mlx, game_update, game);
    
    mlx_loop(game->mlx);
}
```

**Flux complet** :
```
mlx_key_hook(game->mlx, handle_keyboard, game)
    ↓
[Utilisateur appuie sur ESC]
    ↓
handle_keyboard(keydata, game)
    ↓
if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    ↓
cleanup_game(game) → exit(0)


mlx_loop_hook(game->mlx, game_update, game)
    ↓
[Chaque frame, ~60 FPS]
    ↓
game_update(game)
    ↓
handle_continuous_input(game)
    ↓
handle_movement_rotation(game)
    ↓
if (mlx_is_key_down(game->mlx, MLX_KEY_W))
    ↓
move_player(game, 1)
```

---

## ❓ QUESTIONS FRÉQUENTES

### Q1 : Pourquoi deux systèmes d'input différents ?
**R :**
```c
// ✅ Événementiel pour actions uniques
handle_keyboard() → ESC = quitter (1 fois)

// ✅ Polling pour mouvements fluides
handle_continuous_input() → W = avancer (60 fois/sec)
```

**Avantages** :
- Séparation des préoccupations
- Performance optimisée
- Code plus clair

### Q2 : Pourquoi handle_movement_rotation() est static ?
**R :**
```c
static void handle_movement_rotation(t_game *game)
```
- ✅ Encapsulation : Fonction interne au fichier
- ✅ Organisation : Auxiliaire de `handle_continuous_input()`
- ✅ Pas besoin de l'exposer dans le header

### Q3 : Que signifient les codes de move_player() ?
**R :**
```c
move_player(game, 1)   // Avancer (W)
move_player(game, -1)  // Reculer (S)
move_player(game, 2)   // Gauche (A)
move_player(game, 3)   // Droite (D)
```

**Pourquoi pas une enum ?**
```c
// Alternative possible (mais non utilisée)
typedef enum e_direction
{
    DIR_FORWARD = 1,
    DIR_BACKWARD = -1,
    DIR_LEFT = 2,
    DIR_RIGHT = 3
} t_direction;
```

### Q4 : Pourquoi exit(0) dans handle_keyboard() et exit(1) dans on_close() ?
**R :**

```c
// ESC = terminaison NORMALE
handle_keyboard() → cleanup_game() → exit(0)

// Fermeture fenêtre = terminaison par utilisateur
on_close() → cleanup_game() → exit(1)
```

**Convention Unix** :
- `0` : Succès
- `1-255` : Codes d'erreur

### Q5 : Peut-on détecter plusieurs touches simultanément ?
**R :** ✅ **OUI** avec le polling !

```c
void handle_movement_rotation(t_game *game)
{
    // Ces deux conditions peuvent être vraies en même temps
    if (mlx_is_key_down(game->mlx, MLX_KEY_W))  // true
        move_player(game, 1);                    // Avance
    
    if (mlx_is_key_down(game->mlx, MLX_KEY_D))  // true
        move_player(game, 3);                    // Strafe droite
    
    // Résultat : Déplacement en DIAGONALE (avant-droite)
}
```

**Impossible avec mlx_key_hook** :
```c
// ❌ Un seul événement à la fois
void handle_keyboard(mlx_key_data_t keydata, void *param)
{
    // keydata contient UNE SEULE touche
}
```

### Q6 : Quelle est la vitesse de rotation (ROT_SPEED) ?
**R :**
```c
#define ROT_SPEED 0.025  // Dans cub3d.h
```

**En degrés** :
```
0.025 radians = 1.43 degrés par frame

À 60 FPS :
0.025 * 60 = 1.5 radians/sec = 86 degrés/sec

Rotation complète (360°) :
360° / 86° ≈ 4.2 secondes
```

### Q7 : Pourquoi cleanup_game() avant exit() ?
**R :**

```c
// ✅ BON
cleanup_game(game);  // Libère mémoire + ferme MLX42
exit(0);             // Termine proprement

// ❌ MAUVAIS
exit(0);             // Termine immédiatement
// cleanup_game() jamais appelé → MEMORY LEAKS
```

**Ordre dans cleanup_game()** :
1. Libérer textures MLX42
2. Libérer images MLX42
3. mlx_terminate()
4. Libérer map
5. Libérer game

### Q8 : mlx_is_key_down() coûte-t-il cher en performances ?
**R :** ✅ **NON**, très rapide !

```c
// Pseudo-code interne de MLX42
bool mlx_is_key_down(mlx_t *mlx, mlx_key_t key)
{
    return mlx->key_states[key];  // Simple accès tableau
}
```

**Complexité** : O(1) (accès direct)

**À 60 FPS, 6 vérifications** :
```
6 appels * 60 FPS = 360 vérifications/sec
Temps par vérification : ~1-5 nanosecondes
Impact : NÉGLIGEABLE
```

### Q9 : Peut-on ajouter d'autres touches facilement ?
**R :** ✅ **OUI** !

```c
// Ajouter le sprint (SHIFT)
static void handle_movement_rotation(t_game *game)
{
    float speed = MOVE_SPEED;
    
    // Si SHIFT enfoncé, doubler la vitesse
    if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT))
        speed *= 2.0;
    
    if (mlx_is_key_down(game->mlx, MLX_KEY_W))
        move_player_with_speed(game, 1, speed);
    // ...
}
```

### Q10 : Pourquoi handle_continuous_input() semble vide ?
**R :**

```c
void handle_continuous_input(t_game *game)
{
    handle_movement_rotation(game);  // Seule ligne
}
```

**Raisons** :
1. **Extensibilité** : Facile d'ajouter d'autres handlers
2. **Organisation** : Point d'entrée clair
3. **Modularité** : Séparation des responsabilités

**Évolution future** :
```c
void handle_continuous_input(t_game *game)
{
    handle_movement_rotation(game);
    handle_combat(game);        // Nouveau
    handle_interactions(game);  // Nouveau
}
```

---

## 📊 DIAGRAMMES

### Architecture globale

```
┌─────────────────────────────────────────────────────────┐
│                     KEYBOARD.C                          │
│                                                         │
│  ┌───────────────────────────────────────────────────┐ │
│  │           on_close(void *param)                   │ │
│  │           ↓                                       │ │
│  │    cleanup_game(game) → exit(1)                  │ │
│  │                                                   │ │
│  │    Appelé par : mlx_close_hook()                 │ │
│  │    Quand : Fermeture fenêtre                     │ │
│  └───────────────────────────────────────────────────┘ │
│                                                         │
│  ┌───────────────────────────────────────────────────┐ │
│  │    handle_keyboard(keydata, param)                │ │
│  │           ↓                                       │ │
│  │    if ESC → cleanup_game() → exit(0)             │ │
│  │                                                   │ │
│  │    Appelé par : mlx_key_hook()                   │ │
│  │    Quand : Appui/relâchement touche              │ │
│  └───────────────────────────────────────────────────┘ │
│                                                         │
│  ┌───────────────────────────────────────────────────┐ │
│  │    handle_continuous_input(game)                  │ │
│  │           ↓                                       │ │
│  │    handle_movement_rotation(game)                │ │
│  │           ↓                                       │ │
│  │    ├─→ W → move_player(1)                        │ │
│  │    ├─→ S → move_player(-1)                       │ │
│  │    ├─→ A → move_player(2)                        │ │
│  │    ├─→ D → move_player(3)                        │ │
│  │    ├─→ ← → rotate_player(-ROT_SPEED)             │ │
│  │    └─→ → → rotate_player(ROT_SPEED)              │ │
│  │                                                   │ │
│  │    Appelé par : game_update() (60 FPS)           │ │
│  └───────────────────────────────────────────────────┘ │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Flux d'événements

```
┌─────────────────────────────────────────────────────────┐
│                  Événement Utilisateur                  │
└──────────────────────┬──────────────────────────────────┘
                       │
        ┌──────────────┼──────────────┐
        │              │              │
        ↓              ↓              ↓
   [Appui ESC]   [Maintien W]   [Fermeture ❌]
        │              │              │
        ↓              ↓              ↓
  mlx_key_hook    mlx_loop_hook  mlx_close_hook
        │              │              │
        ↓              ↓              ↓
 handle_keyboard  game_update    on_close
        │              │              │
        ↓              ↓              │
 keydata.key==ESC  handle_cont..     │
        │              │              │
        ↓              ↓              │
   cleanup_game    handle_movem..    │
        │              │              │
        ↓              ↓              │
     exit(0)    mlx_is_key_down(W)   │
                       │              │
                       ↓              │
                  move_player(1)     │
                                     ↓
                               cleanup_game
                                     ↓
                                  exit(1)
```

### Cycle de vie d'une touche (W maintenu)

```
T=0ms     [USER] Appuie sur W
             ↓
          MLX42 détecte l'événement
             ↓
          ┌─────────────────────────────────┐
          │ mlx_key_hook déclenché         │
          │ handle_keyboard() appelé       │
          │ → keydata.action = MLX_PRESS   │
          │ → Pas de traitement (pas ESC)  │
          └─────────────────────────────────┘
             ↓
          [W est maintenant dans l'état "enfoncé"]
             ↓
─────────────────────────────────────────────────
T=0ms     Frame 1
          game_update() → handle_continuous_input()
             ↓
          mlx_is_key_down(W) = true ✓
             ↓
          move_player(game, 1)
             ↓
          Position X += dir_x * 0.08
          Position Y += dir_y * 0.08
─────────────────────────────────────────────────
T=16ms    Frame 2
          game_update() → handle_continuous_input()
             ↓
          mlx_is_key_down(W) = true ✓
             ↓
          move_player(game, 1)
             ↓
          Position X += dir_x * 0.08
          Position Y += dir_y * 0.08
─────────────────────────────────────────────────
          ... (58 frames identiques)
─────────────────────────────────────────────────
T=1000ms  [USER] Relâche W
             ↓
          MLX42 détecte l'événement
             ↓
          ┌─────────────────────────────────┐
          │ mlx_key_hook déclenché         │
          │ handle_keyboard() appelé       │
          │ → keydata.action = MLX_RELEASE │
          │ → Pas de traitement (pas ESC)  │
          └─────────────────────────────────┘
             ↓
          [W n'est plus dans l'état "enfoncé"]
             ↓
─────────────────────────────────────────────────
T=1016ms  Frame 61
          game_update() → handle_continuous_input()
             ↓
          mlx_is_key_down(W) = false ✗
             ↓
          move_player() PAS appelé
             ↓
          Position inchangée
─────────────────────────────────────────────────
```

### Mapping des touches

```
┌─────────────────────────────────────────────────────────┐
│                     CLAVIER                             │
├─────────────────────────────────────────────────────────┤
│                                                         │
│    [ESC]                                                │
│      ↓                                                  │
│   Quitter le jeu (exit 0)                              │
│                                                         │
│    [W]     [A]     [S]     [D]                         │
│     ↓       ↓       ↓       ↓                          │
│   Avant  Gauche  Arrière  Droite                       │
│     │       │       │       │                          │
│     └───────┴───────┴───────┘                          │
│              ↓                                          │
│        move_player()                                    │
│              ↓                                          │
│    Position mise à jour                                │
│                                                         │
│    [←]                    [→]                          │
│     ↓                      ↓                           │
│  Tourner gauche       Tourner droite                   │
│     │                      │                           │
│     └──────────────────────┘                           │
│              ↓                                          │
│       rotate_player()                                   │
│              ↓                                          │
│    Direction mise à jour                               │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Comparaison événementiel vs polling

```
┌─────────────────────────────────────────────────────────┐
│              ÉVÉNEMENTIEL (mlx_key_hook)                │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Avantages:                                             │
│    ✅ Informations détaillées (action, modifier)        │
│    ✅ Efficace pour actions ponctuelles                 │
│    ✅ Pas de vérification inutile                       │
│                                                         │
│  Inconvénients:                                         │
│    ❌ Pas adapté pour mouvements fluides                │
│    ❌ Pas d'appuis simultanés faciles                   │
│    ❌ Appelé 2 fois (press + release)                   │
│                                                         │
│  Utilisé pour:                                          │
│    • ESC (quitter)                                      │
│    • P (pause)                                          │
│    • M (menu)                                           │
│    • Actions uniques                                    │
│                                                         │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│               POLLING (mlx_is_key_down)                 │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Avantages:                                             │
│    ✅ Mouvements fluides (60 FPS)                       │
│    ✅ Appuis simultanés naturels                        │
│    ✅ Simple à implémenter                              │
│                                                         │
│  Inconvénients:                                         │
│    ❌ Pas d'info sur l'événement                        │
│    ❌ Vérifié même si touche pas enfoncée              │
│    ❌ Moins précis pour timing exact                    │
│                                                         │
│  Utilisé pour:                                          │
│    • WASD (mouvements)                                  │
│    • ← → (rotations)                                    │
│    • Actions continues                                  │
│    • Combinaisons de touches                            │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Ordre des vérifications dans handle_movement_rotation()

```
handle_movement_rotation(game)
        │
        ├─→ [1] mlx_is_key_down(MLX_KEY_W) ?
        │         ├─ true  → move_player(game, 1)
        │         └─ false → skip
        │
        ├─→ [2] mlx_is_key_down(MLX_KEY_S) ?
        │         ├─ true  → move_player(game, -1)
        │         └─ false → skip
        │
        ├─→ [3] mlx_is_key_down(MLX_KEY_A) ?
        │         ├─ true  → move_player(game, 2)
        │         └─ false → skip
        │
        ├─→ [4] mlx_is_key_down(MLX_KEY_D) ?
        │         ├─ true  → move_player(game, 3)
        │         └─ false → skip
        │
        ├─→ [5] mlx_is_key_down(MLX_KEY_LEFT) ?
        │         ├─ true  → rotate_player(game, -ROT_SPEED)
        │         └─ false → skip
        │
        └─→ [6] mlx_is_key_down(MLX_KEY_RIGHT) ?
                  ├─ true  → rotate_player(game, ROT_SPEED)
                  └─ false → skip

⚠️  TOUTES les vérifications sont faites, même si plusieurs vraies
⚠️  Permet les appuis simultanés (ex: W + D = diagonale)
```

---

## 🎓 CHECKLIST POUR LA CORRECTION

### Comprendre on_close()
- [ ] Expliquer quand elle est appelée (fermeture fenêtre)
- [ ] Expliquer le cast `(t_game *)param`
- [ ] Expliquer l'ordre : cleanup PUIS exit
- [ ] Expliquer pourquoi `exit(1)` (fermeture utilisateur)

### Comprendre handle_keyboard()
- [ ] Expliquer la structure `mlx_key_data_t`
- [ ] Expliquer la double condition (key && action)
- [ ] Expliquer pourquoi `exit(0)` ici (terminaison normale)
- [ ] Différencier de `handle_continuous_input()`

### Comprendre handle_movement_rotation()
- [ ] Expliquer pourquoi elle est `static`
- [ ] Expliquer `mlx_is_key_down()` (polling)
- [ ] Expliquer les codes de `move_player()` (1, -1, 2, 3)
- [ ] Expliquer les appuis simultanés (W + D)

### Comprendre handle_continuous_input()
- [ ] Expliquer son rôle de coordinateur
- [ ] Expliquer pourquoi elle existe (extensibilité)
- [ ] Expliquer la fréquence d'appel (~60 FPS)

### Système d'input MLX42
- [ ] Différencier événementiel vs polling
- [ ] mlx_key_hook : événements ponctuels
- [ ] mlx_is_key_down : mouvements fluides
- [ ] Expliquer quand utiliser chaque approche

### Questions pièges possibles
- [ ] "Pourquoi deux systèmes d'input ?"
  → Événementiel pour actions uniques, polling pour mouvements fluides
  
- [ ] "Que signifient les paramètres de move_player ?"
  → 1=avant, -1=arrière, 2=gauche, 3=droite
  
- [ ] "Pourquoi exit(0) vs exit(1) ?"
  → 0=terminaison normale (ESC), 1=fermeture fenêtre
  
- [ ] "Peut-on appuyer sur W et D en même temps ?"
  → OUI avec polling, chaque if est indépendant
  
- [ ] "Combien de fois handle_continuous_input est appelée ?"
  → ~60 fois/sec (chaque frame)

### Concepts clés à maîtriser
- [ ] **Événementiel** : Réagir à un événement ponctuel
- [ ] **Polling** : Vérifier l'état en continu
- [ ] **mlx_key_data_t** : Structure d'événement clavier
- [ ] **mlx_is_key_down()** : Vérification d'état de touche
- [ ] **Callback** : Fonction passée à MLX42

---

## 🔍 DEBUGGING

### ESC ne ferme pas le jeu
**Symptôme** : Appuyer sur ESC ne fait rien

**Causes possibles** :
```c
// ❌ Condition incorrecte
if (keydata.key == MLX_KEY_ESCAPE)  // Manque keydata.action
    exit(0);
```

**Solution** :
```c
// ✅ Vérifier aussi l'action
if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    exit(0);
```

### Mouvements ne fonctionnent pas
**Symptôme** : WASD ne font rien

**Causes possibles** :
```c
// ❌ handle_continuous_input() pas appelée
void game_update(void *param)
{
    t_game *game = (t_game *)param;
    // handle_continuous_input(game);  // OUBLIÉ
    render_frame(game);
}
```

**Solution** :
```c
// ✅ Appeler handle_continuous_input()
void game_update(void *param)
{
    t_game *game = (t_game *)param;
    handle_continuous_input(game);  // Avant render_frame
    render_frame(game);
}
```

### Mouvements saccadés
**Symptôme** : Le joueur se déplace par à-coups

**Causes possibles** :
```c
// ❌ Utiliser mlx_key_hook pour les mouvements
void handle_keyboard(mlx_key_data_t keydata, void *param)
{
    if (keydata.key == MLX_KEY_W && keydata.action == MLX_PRESS)
        move_player(game, 1);  // Appelé 1 seule fois
}
```

**Solution** :
```c
// ✅ Utiliser mlx_is_key_down dans handle_continuous_input
void handle_movement_rotation(t_game *game)
{
    if (mlx_is_key_down(game->mlx, MLX_KEY_W))
        move_player(game, 1);  // Appelé 60 fois/sec
}
```

### Memory leaks à la fermeture
**Symptôme** : Valgrind signale des fuites

**Causes possibles** :
```c
// ❌ Exit sans cleanup
void on_close(void *param)
{
    exit(1);  // Mémoire pas libérée
}
```

**Solution** :
```c
// ✅ Cleanup avant exit
void on_close(void *param)
{
    t_game *game = (t_game *)param;
    cleanup_game(game);  // Libère tout
    exit(1);
}
```

### Crash au déplacement
**Symptôme** : Segfault quand on appuie sur WASD

**Causes possibles** :
```c
// ❌ move_player() reçoit un paramètre invalide
move_player(game, 999);  // Code inconnu
```

**Vérification dans movements.c** :
```c
void move_player(t_game *game, int direction)
{
    if (direction == 1)
        // ...
    else if (direction == -1)
        // ...
    else if (direction == 2)
        // ...
    else if (direction == 3)
        // ...
    else
        return;  // Sécurité
}
```

### Rotation inversée
**Symptôme** : ← tourne à droite et → tourne à gauche

**Causes possibles** :
```c
// ❌ Signes inversés
if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
    rotate_player(game, ROT_SPEED);   // MAUVAIS signe
if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
    rotate_player(game, -ROT_SPEED);  // MAUVAIS signe
```

**Solution** :
```c
// ✅ Signes corrects
if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
    rotate_player(game, -ROT_SPEED);  // Négatif = gauche
if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
    rotate_player(game, ROT_SPEED);   // Positif = droite
```

---

## 📝 RÉSUMÉ EXÉCUTIF

### keyboard.c en 3 phrases
1. **on_close()** et **handle_keyboard()** gèrent les événements ponctuels (fermeture, ESC)
2. **handle_movement_rotation()** vérifie l'état des touches WASD/flèches en polling (~60 FPS)
3. **handle_continuous_input()** coordonne tous les inputs continus pour des mouvements fluides

### Points clés pour la correction
- ✅ Deux systèmes : événementiel (ESC) + polling (WASD)
- ✅ Polling = ~60 vérifications/sec pour mouvements fluides
- ✅ Appuis simultanés possibles (W + D = diagonale)
- ✅ Cleanup AVANT exit pour éviter memory leaks
- ✅ exit(0) pour ESC, exit(1) pour fermeture fenêtre

### Touches gérées
```
ESC     → Quitter (événementiel)
W       → Avancer (polling)
S       → Reculer (polling)
A       → Gauche (polling)
D       → Droite (polling)
←       → Tourner gauche (polling)
→       → Tourner droite (polling)
❌      → Fermeture fenêtre (événement système)
```

### Dépendances
```
keyboard.c utilise:
  ← MLX42         : mlx_is_key_down, mlx_key_data_t
  ← movements.c   : move_player, rotate_player
  ← utils.c       : cleanup_game
```

### Ligne du temps d'exécution
```
game_loop() → mlx_key_hook(handle_keyboard)
           → mlx_close_hook(on_close)
           → mlx_loop_hook(game_update)
                  ↓
           [Boucle infinie]
                  ↓
           game_update() (60 FPS)
                  ↓
           handle_continuous_input()
                  ↓
           handle_movement_rotation()
                  ↓
           Vérifications WASD + flèches
```

---

## 🎯 FIN DE L'EXPLICATION

Ce fichier couvre **100% du code** de `keyboard.c` avec des explications détaillées adaptées pour une correction de projet 42.

**Fichiers d'explication disponibles** :
1. ✅ `EXECUTION_GUIDE.md` - Vue d'ensemble du raycasting
2. ✅ `MAIN_EXPLICATION.md` - Explication de main.c
3. ✅ `INIT_EXPLICATION.md` - Explication de init.c
4. ✅ `GAME_LOOP_EXPLICATION.md` - Explication de game_loop.c
5. ✅ `KEYBOARD_EXPLICATION.md` - Explication de keyboard.c (ce fichier)

**Prochaines étapes suggérées** :
- Relire les 5 fichiers markdown
- Comprendre la chaîne d'appels : main → init → game_loop → keyboard
- Tester l'explication à haute voix
- Préparer des réponses aux questions sur les deux systèmes d'input

Bon courage pour la correction ! 🚀
