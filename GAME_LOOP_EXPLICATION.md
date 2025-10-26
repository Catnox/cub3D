# GAME_LOOP.C - EXPLICATION DÉTAILLÉE

## 📋 TABLE DES MATIÈRES

1. [Vue d'ensemble](#vue-densemble)
2. [Fonction game_update()](#fonction-game_update)
3. [Fonction game_loop()](#fonction-game_loop)
4. [Hooks MLX42](#hooks-mlx42)
5. [Flux d'exécution](#flux-dexécution)
6. [Questions fréquentes](#questions-fréquentes)
7. [Diagrammes](#diagrammes)

---

## 🎯 VUE D'ENSEMBLE

### Rôle du fichier
`game_loop.c` contient la **boucle principale du jeu** qui gère :
- L'enregistrement des événements (clavier, fermeture de fenêtre)
- La mise à jour continue du jeu (chaque frame)
- Le rendu graphique

### Fonctions principales
```
game_loop.c
├── game_update()     (static) - Appelée chaque frame
└── game_loop()       (public) - Configure et lance la boucle
```

### Dépendances
```c
#include "cub3d.h"
```
- **MLX42** : `mlx_key_hook()`, `mlx_close_hook()`, `mlx_loop_hook()`, `mlx_loop()`
- **Contrôles** : `handle_keyboard()`, `handle_continuous_input()`
- **Rendu** : `render_frame()`
- **Utils** : `on_close()`

---

## 🔄 FONCTION GAME_UPDATE()

### Signature
```c
static void game_update(void *param)
```

### Vue d'ensemble
Cette fonction est **appelée automatiquement par MLX42 à chaque frame**. Elle constitue le cœur de la boucle de jeu.

### Analyse ligne par ligne

```c
static void	game_update(void *param)
{
```
**`static`** : Fonction privée au fichier (portée locale).
**`void *param`** : Pointeur générique car MLX42 utilise un callback universel.

```c
	t_game	*game;
```
**Déclaration** : Variable locale pour stocker le pointeur vers la structure du jeu.

```c
	game = (t_game *)param;
```
**Cast du paramètre** :
- `param` est de type `void*` (pointeur générique)
- On le convertit en `t_game*` pour accéder à la structure
- Ce pointeur a été passé lors de l'enregistrement du hook avec `mlx_loop_hook()`

```c
	handle_continuous_input(game);
```
**Traitement des entrées continues** :
- Vérifie les touches actuellement enfoncées (`mlx_is_key_down()`)
- Gère les mouvements : W (avant), S (arrière), A (gauche), D (droite)
- Gère les rotations : ← (gauche), → (droite)
- Applique `MOVE_SPEED` et `ROT_SPEED` pour des déplacements fluides

**⚠️ Ordre important** : Les entrées AVANT le rendu pour que les changements de position soient visibles immédiatement.

```c
	render_frame(game);
```
**Rendu de la frame** :
- Lance le raycasting pour tous les rayons (WINDOW_WIDTH)
- Calcule les hauteurs de murs
- Dessine le plafond, les murs, le sol
- Applique les textures
- Met à jour l'image affichée à l'écran

```c
}
```

### Fréquence d'appel
- **MLX42** appelle cette fonction environ **60 fois par seconde** (60 FPS)
- Synchronisée avec le rafraîchissement de l'écran (VSync)

### Pourquoi static ?
```c
static void game_update(void *param)
```
- ✅ Évite les conflits de noms avec d'autres fichiers
- ✅ Limite la portée (principe d'encapsulation)
- ✅ Ne doit être appelée QUE par MLX42 via le hook

---

## 🎮 FONCTION GAME_LOOP()

### Signature
```c
void game_loop(t_game *game)
```

### Vue d'ensemble
Fonction **publique** qui configure tous les hooks MLX42 et lance la boucle principale du jeu.

### Analyse ligne par ligne

```c
void	game_loop(t_game *game)
{
```
**Fonction publique** : Accessible depuis `main.c`
**Paramètre** : `t_game *game` - Structure entièrement initialisée

```c
	mlx_key_hook(game->mlx, handle_keyboard, game);
```
**Hook clavier pour événements ponctuels** :

**Paramètres** :
1. `game->mlx` - Contexte MLX42
2. `handle_keyboard` - Fonction callback (dans `keyboard.c`)
3. `game` - Paramètre passé au callback

**Ce qui se passe** :
- MLX42 appelle `handle_keyboard()` lors d'un **appui** ou **relâchement** de touche
- Utilisé pour les actions uniques (ex : ESC pour quitter)

**Différence avec `handle_continuous_input()`** :
```
mlx_key_hook()              → Événement ponctuel (key press/release)
handle_continuous_input()   → État continu (key is down)
```

**Prototype de handle_keyboard()** :
```c
void handle_keyboard(mlx_key_data_t keydata, void *param)
{
    // keydata contient :
    // - keydata.key      (MLX_KEY_ESCAPE, etc.)
    // - keydata.action   (MLX_PRESS, MLX_RELEASE)
    // - keydata.modifier (SHIFT, CTRL, etc.)
}
```

```c
	mlx_close_hook(game->mlx, on_close, game);
```
**Hook fermeture de fenêtre** :

**Paramètres** :
1. `game->mlx` - Contexte MLX42
2. `on_close` - Fonction callback (dans `utils.c`)
3. `game` - Paramètre passé au callback

**Quand est-il déclenché ?** :
- Utilisateur clique sur le bouton ❌ de la fenêtre
- Alt+F4 sur Windows
- Cmd+W sur macOS

**Prototype de on_close()** :
```c
void on_close(void *param)
{
    t_game *game = (t_game *)param;
    cleanup_game(game);
    exit(0);
}
```

**Importance** :
- ✅ Libère la mémoire proprement
- ✅ Ferme les textures MLX42
- ✅ Termine le programme sans leaks

```c
	mlx_loop_hook(game->mlx, game_update, game);
```
**Hook de boucle principale** :

**Paramètres** :
1. `game->mlx` - Contexte MLX42
2. `game_update` - Fonction callback (fonction static au-dessus)
3. `game` - Paramètre passé au callback

**Ce qui se passe** :
- MLX42 appelle `game_update()` **en continu**, frame après frame
- Fréquence : ~60 FPS (synchronisé avec l'écran)
- C'est le **cœur de la boucle de jeu**

**Ordre d'exécution** :
```
Frame 1: game_update() → handle_continuous_input() → render_frame()
Frame 2: game_update() → handle_continuous_input() → render_frame()
Frame 3: game_update() → handle_continuous_input() → render_frame()
...
```

```c
	mlx_loop(game->mlx);
```
**Démarrage de la boucle MLX42** :

**Ce qui se passe** :
- Lance la boucle principale de MLX42
- **Fonction bloquante** : ne retourne PAS tant que la fenêtre n'est pas fermée
- Traite les événements du système d'exploitation
- Appelle les hooks enregistrés au-dessus

**Pseudo-code de mlx_loop()** :
```c
void mlx_loop(mlx_t *mlx)
{
    while (window_is_open)
    {
        // 1. Traiter les événements OS
        process_os_events();
        
        // 2. Appeler les hooks clavier si nécessaire
        if (key_event_occurred)
            call_key_hooks();
        
        // 3. Appeler le loop_hook (game_update)
        call_loop_hooks();
        
        // 4. Rafraîchir l'affichage
        refresh_display();
        
        // 5. Synchroniser avec VSync (~60 FPS)
        wait_for_vsync();
    }
}
```

**Pourquoi en dernier ?**
- ⚠️ `mlx_loop()` ne retourne jamais (sauf à la fermeture)
- Tout le code après cette ligne est inaccessible pendant l'exécution
- Les hooks DOIVENT être enregistrés AVANT de lancer la boucle

```c
}
```

### Résumé de la configuration
```
1. mlx_key_hook()      → Enregistre le gestionnaire de touches
2. mlx_close_hook()    → Enregistre le gestionnaire de fermeture
3. mlx_loop_hook()     → Enregistre la fonction de mise à jour
4. mlx_loop()          → Lance la boucle (bloquant)
```

---

## 🔗 HOOKS MLX42

### Types de hooks dans cub3D

| Hook | Fonction | Fréquence | Usage |
|------|----------|-----------|-------|
| **mlx_key_hook** | `handle_keyboard()` | Par événement | ESC pour quitter |
| **mlx_close_hook** | `on_close()` | À la fermeture | Cleanup et exit |
| **mlx_loop_hook** | `game_update()` | ~60 FPS | Mise à jour continue |

### Différence : mlx_key_hook vs handle_continuous_input

#### mlx_key_hook (événementiel)
```c
void handle_keyboard(mlx_key_data_t keydata, void *param)
{
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
        on_close(param);
}
```
**Caractéristiques** :
- ✅ Déclenché UNE FOIS par appui/relâchement
- ✅ Idéal pour actions ponctuelles (quitter, pause, tir)
- ✅ Reçoit des données détaillées (action, modifier)

#### handle_continuous_input (polling)
```c
void handle_continuous_input(t_game *game)
{
    if (mlx_is_key_down(game->mlx, MLX_KEY_W))
        move_forward(game);
    if (mlx_is_key_down(game->mlx, MLX_KEY_S))
        move_backward(game);
    // ...
}
```
**Caractéristiques** :
- ✅ Vérifié CHAQUE FRAME (~60 fois/seconde)
- ✅ Idéal pour mouvements fluides
- ✅ Permet appuis simultanés (W + D = diagonale)

### Exemple concret

**Scénario** : Joueur appuie sur W et maintient 1 seconde

```
mlx_key_hook()
├── T=0ms    : handle_keyboard() appelé → keydata.action = MLX_PRESS
└── T=1000ms : handle_keyboard() appelé → keydata.action = MLX_RELEASE

handle_continuous_input() (dans game_update)
├── T=0ms    : mlx_is_key_down(W) = true  → move_forward()
├── T=16ms   : mlx_is_key_down(W) = true  → move_forward()
├── T=32ms   : mlx_is_key_down(W) = true  → move_forward()
├── ...      : (environ 60 fois)
├── T=984ms  : mlx_is_key_down(W) = true  → move_forward()
└── T=1000ms : mlx_is_key_down(W) = false → pas de mouvement
```

**Résultat** :
- `handle_keyboard()` : Appelé **2 fois** (press + release)
- `handle_continuous_input()` : Appelé **~60 fois** (mouvement fluide)

---

## 🔄 FLUX D'EXÉCUTION

### Vue d'ensemble du programme

```
main()
  ↓
init_game()
  ↓
game_loop() ← NOUS SOMMES ICI
  ↓
  ├── mlx_key_hook()      (enregistre handle_keyboard)
  ├── mlx_close_hook()    (enregistre on_close)
  ├── mlx_loop_hook()     (enregistre game_update)
  └── mlx_loop()          (lance la boucle - BLOQUANT)
        ↓
        [BOUCLE INFINIE jusqu'à fermeture]
        ↓
        ├─→ Événement clavier ? → handle_keyboard()
        ├─→ Fermeture fenêtre ? → on_close()
        └─→ Chaque frame        → game_update()
                                      ↓
                                      ├── handle_continuous_input()
                                      └── render_frame()
```

### Détail d'une frame (16ms à 60 FPS)

```
┌─────────────────────────────────────────────────┐
│ Frame N (T=0ms)                                 │
├─────────────────────────────────────────────────┤
│ 1. MLX42 traite les événements OS               │
│    └─→ Mise à jour de l'état des touches        │
│                                                 │
│ 2. game_update() est appelé                     │
│    ├─→ handle_continuous_input(game)            │
│    │   ├─→ Vérifie mlx_is_key_down(W)           │
│    │   ├─→ Vérifie mlx_is_key_down(S)           │
│    │   ├─→ Vérifie mlx_is_key_down(A)           │
│    │   ├─→ Vérifie mlx_is_key_down(D)           │
│    │   ├─→ Vérifie mlx_is_key_down(←)           │
│    │   ├─→ Vérifie mlx_is_key_down(→)           │
│    │   └─→ Applique mouvements/rotations        │
│    │                                             │
│    └─→ render_frame(game)                       │
│        ├─→ Raycasting (1024 rayons)             │
│        ├─→ Calcul des hauteurs de murs          │
│        ├─→ Dessin plafond                       │
│        ├─→ Dessin murs avec textures            │
│        └─→ Dessin sol                            │
│                                                 │
│ 3. MLX42 met à jour l'affichage                 │
│                                                 │
│ 4. VSync (attendre prochain rafraîchissement)   │
└─────────────────────────────────────────────────┘
        ↓
┌─────────────────────────────────────────────────┐
│ Frame N+1 (T=16ms)                              │
└─────────────────────────────────────────────────┘
```

### Timeline avec événements

```
T=0ms     : Programme démarre
T=100ms   : init_game() terminé
T=101ms   : game_loop() appelé
T=102ms   : Hooks enregistrés
T=103ms   : mlx_loop() démarre
            ┌─────────────────────────────────┐
T=103ms   : │ Frame 1 : game_update()         │
T=119ms   : │ Frame 2 : game_update()         │
T=135ms   : │ Frame 3 : game_update()         │
            │                                 │
T=500ms   : │ [USER] Appuie sur W             │
            │ → handle_keyboard() appelé      │
T=516ms   : │ Frame 26: game_update()         │
            │   → mlx_is_key_down(W) = true   │
            │   → move_forward()              │
T=532ms   : │ Frame 27: game_update()         │
            │   → mlx_is_key_down(W) = true   │
            │   → move_forward()              │
            │ ... (W maintenu)                │
T=1500ms  : │ [USER] Relâche W                │
            │ → handle_keyboard() appelé      │
T=1516ms  : │ Frame 86: game_update()         │
            │   → mlx_is_key_down(W) = false  │
            │   → pas de mouvement            │
            │                                 │
T=5000ms  : │ [USER] Clique sur ❌            │
            │ → on_close() appelé             │
            │   → cleanup_game()              │
            │   → exit(0)                     │
            └─────────────────────────────────┘
T=5001ms  : Programme terminé
```

---

## ❓ QUESTIONS FRÉQUENTES

### Q1 : Pourquoi game_update() est static ?
**R :** 
```c
static void game_update(void *param)
```
- ✅ **Encapsulation** : Fonction interne au fichier, pas besoin de l'exposer
- ✅ **Sécurité** : Évite les appels directs depuis d'autres fichiers
- ✅ **Organisation** : Seul MLX42 doit appeler cette fonction via le hook

### Q2 : Pourquoi void* pour le paramètre ?
**R :**
```c
static void game_update(void *param)
{
    t_game *game = (t_game *)param;
```
- MLX42 utilise une signature générique pour tous les callbacks
- Permet de passer N'IMPORTE QUEL type de données
- On cast ensuite vers le type réel (`t_game*`)

**Prototype MLX42** :
```c
typedef void (*mlx_loopfunc)(void* param);
void mlx_loop_hook(mlx_t* mlx, mlx_loopfunc func, void* param);
```

### Q3 : Que se passe-t-il si j'inverse l'ordre dans game_update() ?
**R :**
```c
// ❌ MAUVAIS
render_frame(game);
handle_continuous_input(game);
```
**Problème** : Les mouvements sont visibles **1 frame plus tard** (latence de 16ms)

```c
// ✅ BON
handle_continuous_input(game);
render_frame(game);
```
**Avantage** : Les mouvements sont visibles **immédiatement**

### Q4 : mlx_loop() retourne-t-elle un jour ?
**R :** 
```c
mlx_loop(game->mlx);  // Bloque ici
// Ce code n'est jamais exécuté pendant le jeu
```
- ⚠️ **Fonction bloquante** : Ne retourne PAS tant que la fenêtre est ouverte
- Seule la fermeture de la fenêtre fait sortir de la boucle
- Dans notre cas, `on_close()` appelle `exit(0)` donc le programme termine

### Q5 : Combien de fois game_update() est-elle appelée ?
**R :**
- **Fréquence** : ~60 FPS (frames par seconde)
- **Par seconde** : 60 appels
- **Par minute** : 3600 appels
- **En 10 minutes de jeu** : 36 000 appels !

### Q6 : Pourquoi 3 hooks différents ?
**R :**

| Hook | Usage | Exemple |
|------|-------|---------|
| `mlx_key_hook` | Actions ponctuelles | ESC pour quitter |
| `mlx_close_hook` | Nettoyage à la fermeture | Libérer la mémoire |
| `mlx_loop_hook` | Mise à jour continue | Rendu + mouvements |

**Pourquoi ne pas tout mettre dans game_update() ?**
- ❌ Moins performant (vérifications inutiles chaque frame)
- ❌ Moins clair (mélange de responsabilités)
- ✅ Séparation des préoccupations

### Q7 : Que se passe-t-il si j'oublie un hook ?
**R :**

```c
// ❌ mlx_key_hook() oublié
mlx_close_hook(game->mlx, on_close, game);
mlx_loop_hook(game->mlx, game_update, game);
mlx_loop(game->mlx);
```
**Résultat** : ESC ne ferme plus le jeu (mais ❌ fonctionne toujours)

```c
// ❌ mlx_close_hook() oublié
mlx_key_hook(game->mlx, handle_keyboard, game);
mlx_loop_hook(game->mlx, game_update, game);
mlx_loop(game->mlx);
```
**Résultat** : Memory leaks à la fermeture (cleanup non appelé)

```c
// ❌ mlx_loop_hook() oublié
mlx_key_hook(game->mlx, handle_keyboard, game);
mlx_close_hook(game->mlx, on_close, game);
mlx_loop(game->mlx);
```
**Résultat** : Fenêtre noire figée (pas de rendu, pas de mouvements)

### Q8 : Comment fonctionne le passage de paramètre aux hooks ?
**R :**

```c
// Enregistrement du hook
mlx_loop_hook(game->mlx, game_update, game);
                                       ^^^^
                                       Ce pointeur est stocké
```

Puis, à chaque frame :
```c
// MLX42 fait (internement) :
void *stored_param = game;  // Le pointeur qu'on a passé
game_update(stored_param);   // Appel avec le paramètre
```

Dans game_update :
```c
static void game_update(void *param)
{
    t_game *game = (t_game *)param;  // On récupère notre pointeur
    // param pointe vers la MÊME structure game que dans main()
}
```

### Q9 : Puis-je appeler game_loop() plusieurs fois ?
**R :**
```c
// ❌ NON
game_loop(game);
game_loop(game);  // Jamais atteint (mlx_loop bloque)
```

`mlx_loop()` est bloquante, le code après ne s'exécute jamais.

### Q10 : Quelle est la différence avec une boucle while classique ?
**R :**

**Approche naïve (ne PAS faire)** :
```c
void bad_game_loop(t_game *game)
{
    while (1)
    {
        handle_continuous_input(game);
        render_frame(game);
    }
}
```
**Problèmes** :
- ❌ CPU à 100% (pas de limitation de FPS)
- ❌ Pas de traitement des événements OS (fenêtre freeze)
- ❌ Pas de synchronisation avec l'écran (tearing)

**Approche MLX42 (correct)** :
```c
void game_loop(t_game *game)
{
    mlx_loop_hook(game->mlx, game_update, game);
    mlx_loop(game->mlx);
}
```
**Avantages** :
- ✅ MLX42 gère le timing (VSync)
- ✅ Événements OS traités correctement
- ✅ FPS stable (~60)
- ✅ CPU optimisé

---

## 📊 DIAGRAMMES

### Architecture globale

```
┌─────────────────────────────────────────────────────────┐
│                        MAIN()                           │
│                          │                              │
│                          ↓                              │
│                    init_game()                          │
│                          │                              │
│                          ↓                              │
│                    game_loop()  ← game_loop.c           │
└──────────────────────────┬──────────────────────────────┘
                           │
         ┌─────────────────┼─────────────────┐
         ↓                 ↓                 ↓
    mlx_key_hook     mlx_close_hook    mlx_loop_hook
         │                 │                 │
         │                 │                 │
         ↓                 ↓                 ↓
  handle_keyboard      on_close        game_update
   (keyboard.c)        (utils.c)       (game_loop.c)
                                             │
                            ┌────────────────┴────────────┐
                            ↓                             ↓
                 handle_continuous_input          render_frame
                      (movements.c)                 (render.c)
```

### Cycle de vie d'une frame

```
        ┌──────────────────────────────────┐
        │     MLX42 Main Loop              │
        │     (mlx_loop)                   │
        └───────────────┬──────────────────┘
                        │
                        ↓
        ┌───────────────────────────────────┐
        │  1. Traitement événements OS      │
        │     - Clavier                     │
        │     - Souris                      │
        │     - Fenêtre                     │
        └───────────────┬───────────────────┘
                        │
                        ↓
        ┌───────────────────────────────────┐
        │  2. Appel hooks événementiels     │
        │     - mlx_key_hook (si touche)    │
        │     - mlx_close_hook (si fermé)   │
        └───────────────┬───────────────────┘
                        │
                        ↓
        ┌───────────────────────────────────┐
        │  3. Appel loop_hook               │
        │     → game_update()               │
        │       ├─→ handle_continuous_input │
        │       └─→ render_frame            │
        └───────────────┬───────────────────┘
                        │
                        ↓
        ┌───────────────────────────────────┐
        │  4. Mise à jour affichage         │
        │     - Swap buffers                │
        │     - Afficher l'image            │
        └───────────────┬───────────────────┘
                        │
                        ↓
        ┌───────────────────────────────────┐
        │  5. VSync                         │
        │     - Attendre ~16ms (60 FPS)     │
        └───────────────┬───────────────────┘
                        │
                        ↓ (retour au début)
```

### Flux de données dans game_update()

```
game_update(void *param)
         │
         ↓
    ┌─────────────────────┐
    │ Cast param → game   │
    └──────────┬──────────┘
               │
               ↓
    ┌──────────────────────────────────┐
    │ handle_continuous_input(game)    │
    │                                  │
    │ ┌──────────────────────────────┐ │
    │ │ mlx_is_key_down(W) ?         │ │
    │ │   ├─ true  → move_forward()  │ │
    │ │   └─ false → skip            │ │
    │ │                              │ │
    │ │ mlx_is_key_down(S) ?         │ │
    │ │   ├─ true  → move_backward() │ │
    │ │   └─ false → skip            │ │
    │ │                              │ │
    │ │ mlx_is_key_down(A) ?         │ │
    │ │   ├─ true  → move_left()     │ │
    │ │   └─ false → skip            │ │
    │ │                              │ │
    │ │ ... (D, ←, →)                │ │
    │ └──────────────────────────────┘ │
    │                                  │
    │ Résultat : Position/Direction    │
    │            du joueur mise à jour │
    └──────────┬───────────────────────┘
               │
               ↓
    ┌──────────────────────────────────┐
    │ render_frame(game)               │
    │                                  │
    │ ┌──────────────────────────────┐ │
    │ │ Pour chaque colonne (x)      │ │
    │ │   ├─ Lancer un rayon         │ │
    │ │   ├─ DDA (collision mur)     │ │
    │ │   ├─ Calculer hauteur mur    │ │
    │ │   ├─ Dessiner plafond        │ │
    │ │   ├─ Dessiner mur (texture)  │ │
    │ │   └─ Dessiner sol            │ │
    │ └──────────────────────────────┘ │
    │                                  │
    │ Résultat : Image rendue          │
    └──────────────────────────────────┘
               │
               ↓
         MLX42 affiche
```

### Interaction entre les hooks

```
┌─────────────────────────────────────────────────────────┐
│                      MLX42 Event Loop                   │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌──────────────────┐                                  │
│  │  Événement OS    │                                  │
│  └────────┬─────────┘                                  │
│           │                                            │
│           ├─→ [Touche pressée] ──→ mlx_key_hook       │
│           │                        │                   │
│           │                        ↓                   │
│           │                  handle_keyboard()         │
│           │                        │                   │
│           │                        └─→ if ESC: exit    │
│           │                                            │
│           ├─→ [Fenêtre fermée] ─→ mlx_close_hook      │
│           │                        │                   │
│           │                        ↓                   │
│           │                    on_close()              │
│           │                        │                   │
│           │                        └─→ cleanup + exit  │
│           │                                            │
│           └─→ [Chaque frame] ───→ mlx_loop_hook       │
│                                    │                   │
│                                    ↓                   │
│                              game_update()             │
│                                    │                   │
│                    ┌───────────────┴───────────┐       │
│                    ↓                           ↓       │
│          handle_continuous_input()      render_frame() │
│                    │                           │       │
│                    └───────────────┬───────────┘       │
│                                    │                   │
│                                    ↓                   │
│                           Affichage mis à jour         │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Comparaison mlx_key_hook vs polling

```
SCENARIO : Joueur maintient W pendant 1 seconde (60 frames)

┌─────────────────────────────────────────────────────────┐
│                    mlx_key_hook()                       │
│  (handle_keyboard avec mlx_key_data_t)                  │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Frame 0:   W appuyé  → handle_keyboard(MLX_PRESS)     │
│  Frame 1:   (rien)                                      │
│  Frame 2:   (rien)                                      │
│  Frame 3:   (rien)                                      │
│  ...                                                    │
│  Frame 59:  (rien)                                      │
│  Frame 60:  W relâché → handle_keyboard(MLX_RELEASE)   │
│                                                         │
│  Total appels: 2 (PRESS + RELEASE)                     │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│              handle_continuous_input()                  │
│         (mlx_is_key_down dans game_update)              │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Frame 0:   mlx_is_key_down(W) = true  → move_forward()│
│  Frame 1:   mlx_is_key_down(W) = true  → move_forward()│
│  Frame 2:   mlx_is_key_down(W) = true  → move_forward()│
│  Frame 3:   mlx_is_key_down(W) = true  → move_forward()│
│  ...                                                    │
│  Frame 59:  mlx_is_key_down(W) = true  → move_forward()│
│  Frame 60:  mlx_is_key_down(W) = false → (rien)        │
│                                                         │
│  Total appels: 60 (chaque frame)                       │
│  Résultat: Mouvement fluide et continu                 │
└─────────────────────────────────────────────────────────┘

Conclusion:
  - mlx_key_hook       → Événements ponctuels
  - continuous_input   → Mouvements fluides
```

### Ordre d'enregistrement des hooks

```
game_loop() appelé
      │
      ↓
┌────────────────────────────────────────┐
│ 1. mlx_key_hook(...)                   │  ← Enregistre handle_keyboard
├────────────────────────────────────────┤
│    MLX42 stocke:                       │
│    - Pointeur vers handle_keyboard     │
│    - Paramètre: game                   │
└────────────────────────────────────────┘
      │
      ↓
┌────────────────────────────────────────┐
│ 2. mlx_close_hook(...)                 │  ← Enregistre on_close
├────────────────────────────────────────┤
│    MLX42 stocke:                       │
│    - Pointeur vers on_close            │
│    - Paramètre: game                   │
└────────────────────────────────────────┘
      │
      ↓
┌────────────────────────────────────────┐
│ 3. mlx_loop_hook(...)                  │  ← Enregistre game_update
├────────────────────────────────────────┤
│    MLX42 stocke:                       │
│    - Pointeur vers game_update         │
│    - Paramètre: game                   │
└────────────────────────────────────────┘
      │
      ↓
┌────────────────────────────────────────┐
│ 4. mlx_loop(...)                       │  ← Lance la boucle
├────────────────────────────────────────┤
│    ⚠️  FONCTION BLOQUANTE             │
│                                        │
│    Tout le code après est inaccessible │
│    jusqu'à la fermeture de la fenêtre  │
└────────────────────────────────────────┘

⚠️  IMPORTANT: Les hooks DOIVENT être enregistrés AVANT mlx_loop()
```

---

## 🎓 CHECKLIST POUR LA CORRECTION

### Comprendre game_update()
- [ ] Expliquer pourquoi elle est `static`
- [ ] Expliquer le cast `(t_game *)param`
- [ ] Expliquer l'ordre : input PUIS rendu
- [ ] Expliquer la fréquence d'appel (~60 FPS)

### Comprendre game_loop()
- [ ] Expliquer les 3 hooks enregistrés
- [ ] Différencier mlx_key_hook et handle_continuous_input
- [ ] Expliquer pourquoi mlx_loop() est en dernier
- [ ] Expliquer que mlx_loop() est bloquante

### Hooks MLX42
- [ ] mlx_key_hook : événements ponctuels
- [ ] mlx_close_hook : nettoyage à la fermeture
- [ ] mlx_loop_hook : mise à jour continue

### Questions pièges possibles
- [ ] "Pourquoi game_update reçoit void* ?"
  → Signature générique de MLX42 pour tous les callbacks
  
- [ ] "Que se passe-t-il si on inverse input et rendu ?"
  → Latence d'une frame (mouvement visible 16ms plus tard)
  
- [ ] "Combien de fois game_update est appelée par seconde ?"
  → ~60 fois (60 FPS avec VSync)
  
- [ ] "Peut-on mettre du code après mlx_loop() ?"
  → Non, mlx_loop() ne retourne jamais (sauf à la fermeture)
  
- [ ] "Pourquoi 2 systèmes d'input (hook + polling) ?"
  → Hook pour actions ponctuelles, polling pour mouvements fluides

### Concepts clés à maîtriser
- [ ] **VSync** : Synchronisation avec le rafraîchissement de l'écran
- [ ] **Frame** : Une itération de la boucle de jeu
- [ ] **FPS** : Frames Per Second (images par seconde)
- [ ] **Callback** : Fonction passée en paramètre et appelée plus tard
- [ ] **Hook** : Point d'ancrage pour enregistrer des callbacks

---

## 🔍 DEBUGGING

### game_update() n'est jamais appelée
**Symptôme** : Fenêtre noire figée

**Causes possibles** :
```c
// ❌ Oublié d'enregistrer le loop_hook
game_loop(t_game *game)
{
    mlx_key_hook(game->mlx, handle_keyboard, game);
    mlx_close_hook(game->mlx, on_close, game);
    // mlx_loop_hook() MANQUANT
    mlx_loop(game->mlx);
}
```

**Solution** :
```c
// ✅ Ajouter le loop_hook
mlx_loop_hook(game->mlx, game_update, game);
```

### Segfault dans game_update()
**Symptôme** : Crash avec "Segmentation fault"

**Causes possibles** :
```c
static void game_update(void *param)
{
    t_game *game = (t_game *)param;
    // ❌ game est NULL ou invalide
    handle_continuous_input(game);  // CRASH
}
```

**Vérifications** :
```c
static void game_update(void *param)
{
    t_game *game;
    
    if (!param)  // Sécurité (ne devrait jamais arriver)
        return;
    game = (t_game *)param;
    // ...
}
```

### Mouvements saccadés
**Symptôme** : Le joueur avance par à-coups

**Cause possible** :
```c
// ❌ MOVE_SPEED trop élevé
#define MOVE_SPEED 1.0  // Le joueur avance d'une case entière par frame !
```

**Solution** :
```c
// ✅ MOVE_SPEED adapté pour 60 FPS
#define MOVE_SPEED 0.08  // Mouvement fluide
```

### CPU à 100%
**Symptôme** : Ventilateur à fond, ordinateur chaud

**Cause possible** :
```c
// ❌ Boucle infinie sans limitation
while (1)
{
    render_frame(game);  // Pas de VSync
}
```

**Solution** :
```c
// ✅ Utiliser MLX42 qui gère le VSync
mlx_loop_hook(game->mlx, game_update, game);
mlx_loop(game->mlx);  // Limité à 60 FPS automatiquement
```

### ESC ne ferme pas le jeu
**Symptôme** : Impossible de quitter avec ESC

**Cause possible** :
```c
// ❌ mlx_key_hook non enregistré
game_loop(t_game *game)
{
    // mlx_key_hook() MANQUANT
    mlx_close_hook(game->mlx, on_close, game);
    mlx_loop_hook(game->mlx, game_update, game);
    mlx_loop(game->mlx);
}
```

**Solution** :
```c
// ✅ Enregistrer le key_hook
mlx_key_hook(game->mlx, handle_keyboard, game);
```

### Memory leaks à la fermeture
**Symptôme** : Valgrind signale des fuites mémoire

**Cause possible** :
```c
// ❌ mlx_close_hook non enregistré
game_loop(t_game *game)
{
    mlx_key_hook(game->mlx, handle_keyboard, game);
    // mlx_close_hook() MANQUANT
    mlx_loop_hook(game->mlx, game_update, game);
    mlx_loop(game->mlx);
}
```

**Solution** :
```c
// ✅ Enregistrer le close_hook
mlx_close_hook(game->mlx, on_close, game);
```

---

## 📝 RÉSUMÉ EXÉCUTIF

### game_loop.c en 3 phrases
1. **game_update()** : Fonction appelée ~60 fois/sec qui met à jour les mouvements et le rendu
2. **game_loop()** : Configure les 3 hooks MLX42 (clavier, fermeture, update) puis lance la boucle principale
3. **mlx_loop()** : Boucle infinie bloquante qui appelle les hooks et gère l'affichage

### Points clés pour la correction
- ✅ game_update = cœur de la boucle (input + rendu)
- ✅ 3 hooks = 3 types d'événements (ponctuel, fermeture, continu)
- ✅ mlx_loop = fonction bloquante (ne retourne jamais)
- ✅ Ordre important : hooks AVANT mlx_loop()
- ✅ Fréquence : ~60 FPS (VSync automatique)

### Dépendances
```
game_loop.c utilise:
  ← MLX42         : mlx_key_hook, mlx_close_hook, mlx_loop_hook, mlx_loop
  ← keyboard.c    : handle_keyboard
  ← movements.c   : handle_continuous_input
  ← render.c      : render_frame
  ← utils.c       : on_close
```

### Ligne du temps d'exécution
```
main() → init_game() → game_loop()
                           ↓
                       [Enregistre hooks]
                           ↓
                       mlx_loop() ← BLOQUE ICI
                           ↓
                    [Boucle infinie]
                      Frame 1, 2, 3...
                           ↓
                    game_update() à chaque frame
                           ↓
                    [Jusqu'à fermeture]
```

---

## 🎯 FIN DE L'EXPLICATION

Ce fichier couvre **100% du code** de `game_loop.c` avec des explications détaillées adaptées pour une correction de projet 42.

**Fichiers d'explication disponibles** :
1. ✅ `EXECUTION_GUIDE.md` - Vue d'ensemble du raycasting
2. ✅ `MAIN_EXPLICATION.md` - Explication de main.c
3. ✅ `INIT_EXPLICATION.md` - Explication de init.c
4. ✅ `GAME_LOOP_EXPLICATION.md` - Explication de game_loop.c (ce fichier)

**Prochaines étapes suggérées** :
- Relire les 4 fichiers markdown
- Tester l'explication à haute voix
- Préparer des réponses aux questions pièges
- Vérifier la compréhension avec votre binôme

Bon courage pour la correction ! 🚀
