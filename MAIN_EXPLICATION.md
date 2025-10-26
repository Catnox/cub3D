# 📖 Explication Détaillée du Main - cub3D

## 🎯 Vue d'ensemble

Le `main()` est le point d'entrée de l'application cub3D. C'est une fonction simple mais cruciale qui orchestre les 4 étapes fondamentales du programme :

```
1. Allocation de la structure principale
2. Validation des arguments
3. Initialisation du jeu
4. Lancement de la boucle de jeu
```

---

## 📝 Code annoté ligne par ligne

```c
int	main(int argc, char **argv)
{
	t_game	*game;
```

### **Déclaration de la structure principale**

- `t_game *game` : Pointeur vers la structure qui contiendra **TOUT** le jeu
- Cette structure contient :
  - Le contexte MLX (`mlx_t *mlx`)
  - L'image de rendu (`mlx_image_t *img`)
  - La carte (`t_map *map`)
  - Le joueur (`t_player *player`)

---

```c
	game = ft_calloc(1, sizeof(t_game));
	if (!game)
		return (1);
```

### **Étape 1 : Allocation sécurisée avec `ft_calloc`**

#### Pourquoi `ft_calloc` et pas `malloc` ?

**`ft_calloc` = `malloc` + initialisation à zéro**

```c
// Différence entre malloc et calloc :

// Avec malloc :
game = malloc(sizeof(t_game));
// Contenu : ???????????????????
// Valeurs résiduelles (garbage) de la mémoire précédente

// Avec calloc :
game = ft_calloc(1, sizeof(t_game));
// Contenu : 0x00 00 00 00 00 00 00...
// Tous les bits à zéro
```

#### Avantages de `ft_calloc` :

1. **Sécurité** : Pas de valeurs résiduelles (garbage values)
   ```c
   // Sans calloc (malloc)
   game->mlx = ???          // Pointeur invalide aléatoire
   game->img = ???          // Pointeur invalide aléatoire
   game->map = ???          // Pointeur invalide aléatoire
   
   // Avec calloc
   game->mlx = NULL         // Explicitement NULL
   game->img = NULL         // Explicitement NULL
   game->map = NULL         // Explicitement NULL
   ```

2. **Détection d'erreurs** : Les pointeurs NULL sont faciles à vérifier
   ```c
   if (game->map)     // Sûr : on sait que c'est NULL si pas initialisé
       clean_map();
   ```

3. **Évite les bugs subtils** : Pas de segfault sur pointeurs sauvages
   ```c
   // Si game->mlx contient une adresse aléatoire (0x12345678)
   // et qu'on fait mlx_terminate(game->mlx) → SEGFAULT
   
   // Avec calloc, game->mlx = NULL
   // if (game->mlx) mlx_terminate(game->mlx); → Ne fait rien, c'est sûr
   ```

#### Vérification de l'allocation

```c
if (!game)
    return (1);
```

- Si `ft_calloc` échoue (mémoire insuffisante), elle retourne `NULL`
- On vérifie et on quitte immédiatement avec code d'erreur `1`
- **Important** : Pas besoin de `free(game)` car `game == NULL`

---

```c
	if (argc != 2)
	{
		printf("Error\n");
		ft_putendl_fd("Usage: ./cub3D <map.cub>", 2);
		return (free(game), 1);
	}
```

### **Étape 2 : Validation des arguments**

#### Vérification du nombre d'arguments

```bash
# Correct (argc = 2)
./cub3D maps/test.cub
# argv[0] = "./cub3D"
# argv[1] = "maps/test.cub"
# argc = 2 ✓

# Incorrect (argc = 1)
./cub3D
# argv[0] = "./cub3D"
# argc = 1 ✗

# Incorrect (argc = 3)
./cub3D maps/test.cub extra
# argv[0] = "./cub3D"
# argv[1] = "maps/test.cub"
# argv[2] = "extra"
# argc = 3 ✗
```

#### Messages d'erreur

1. **`printf("Error\n")`** : Message générique requis par le sujet
   - **IMPORTANT** : Le sujet impose ce format exact
   - Toujours afficher "Error\n" en cas de problème

2. **`ft_putendl_fd("Usage: ...", 2)`** : Message d'aide
   - `ft_putendl_fd` : Affiche une ligne sur un file descriptor
   - `2` = `STDERR` (sortie d'erreur standard)
   - Pourquoi STDERR ? Pour séparer les erreurs de la sortie normale

```c
// Différence STDOUT (1) vs STDERR (2)
./cub3D > output.txt           // STDOUT redirigé
// "Error" et "Usage" visibles dans le terminal (STDERR)

./cub3D 2> errors.txt          // STDERR redirigé
// "Usage" dans errors.txt
```

#### Nettoyage avant de quitter

```c
return (free(game), 1);
```

**Opérateur virgule `,` en C** :
- Évalue les expressions de gauche à droite
- Retourne la valeur de la dernière expression

```c
// Équivalent à :
free(game);
return (1);

// Mais plus compact (norme 42)
```

**Pourquoi libérer `game` ici ?**
- On a alloué de la mémoire avec `ft_calloc`
- Même si on quitte, il faut libérer pour éviter les memory leaks
- `game` ne contient que des NULL (calloc), donc pas d'autres free nécessaires

---

```c
	if (init_game(game, argv[1]) != 0)
	{
		cleanup_game(game);
		free(game);
		return (1);
	}
```

### **Étape 3 : Initialisation du jeu**

#### Fonction `init_game()`

Cette fonction fait **TOUT** le travail d'initialisation :

```c
int init_game(t_game *game, char *map_file)
{
    1. parse_cub_file()      // Lire et valider le fichier .cub
    2. find_player_position() // Trouver N/S/E/W dans la map
    3. init_game_state_n_mlx() // Créer fenêtre MLX
    4. mlx_image_to_window()  // Afficher l'image
    5. mlx_resize_hook()      // Callback redimensionnement
    
    return 0;  // Succès
    return 1;  // Échec
}
```

#### Gestion des erreurs d'initialisation

**Si `init_game()` retourne 1 (échec)** :

```c
cleanup_game(game);  // Libérer les ressources partiellement allouées
free(game);          // Libérer la structure principale
return (1);          // Quitter avec erreur
```

**Pourquoi `cleanup_game()` AVANT `free(game)` ?**

```c
// cleanup_game() libère le contenu de game :
- game->map->textures (mlx_delete_texture)
- game->map->grid (free chaque ligne)
- game->map (free structure)
- game->player (free structure)
- game->mlx (mlx_terminate)

// PUIS free(game) libère l'enveloppe
free(game);  // Libère le pointeur lui-même
```

**Ordre critique** :
```
1. Nettoyer le CONTENU de game (cleanup_game)
2. Libérer le CONTENEUR game (free)
```

C'est comme vider une boîte avant de la jeter :
```
🗑️ Boîte (game)
   ├─ 📄 Papiers (map)
   ├─ 🖼️ Photos (textures)
   └─ 👤 Badge (player)

Étape 1 : Jeter papiers, photos, badge (cleanup_game)
Étape 2 : Jeter la boîte vide (free game)
```

#### Scénarios d'erreur possibles

| Erreur | Moment | État de game | Action |
|--------|--------|--------------|---------|
| Fichier .cub inexistant | `parse_cub_file()` | map = NULL | cleanup + free |
| Texture manquante | `parse_cub_file()` | map allouée | cleanup + free |
| Joueur manquant | `find_player_position()` | map + player | cleanup + free |
| Échec MLX | `init_game_state_n_mlx()` | Tout sauf mlx | cleanup + free |

---

```c
	game_loop(game);
	return (0);
}
```

### **Étape 4 : Lancement de la boucle de jeu**

#### `game_loop(game)`

C'est le **cœur battant** du jeu :

```c
void game_loop(t_game *game)
{
    // Enregistrer les hooks (callbacks)
    mlx_key_hook(game->mlx, handle_keyboard, game);
    mlx_close_hook(game->mlx, on_close, game);
    mlx_loop_hook(game->mlx, game_update, game);
    
    // Démarrer la boucle infinie MLX
    mlx_loop(game->mlx);  // ← Ne retourne JAMAIS (jusqu'à fermeture)
}
```

**`mlx_loop()` = Boucle infinie** :
```c
while (fenêtre_ouverte)
{
    1. Traiter les événements (clavier, souris, fermeture)
    2. Appeler game_update() (rendu, logique)
    3. Rafraîchir l'affichage (~60 FPS)
    4. Attendre le prochain frame (VSync)
}

// Quand l'utilisateur ferme la fenêtre :
cleanup_game(game);  // Appelé par on_close()
free(game);
exit(0);             // Fin du programme
```

#### Pourquoi `return (0)` après `game_loop()` ?

**Question piège fréquente !**

```c
game_loop(game);
return (0);  // ← Ce code n'est JAMAIS exécuté en pratique !
```

**Explication** :
- `mlx_loop()` est une boucle infinie
- Le programme se termine SEULEMENT quand on ferme la fenêtre
- La fermeture appelle `on_close()` qui fait `mlx_close_window()` et quitte
- `return (0)` est là pour satisfaire le compilateur (int main doit return)

**En pratique** :
```c
// Scénario réel d'exécution :
main()
  ├─ ft_calloc(game)
  ├─ Vérifier argc
  ├─ init_game()
  ├─ game_loop()
  │   └─ mlx_loop()  // Boucle infinie
  │       └─ [Utilisateur ferme fenêtre]
  │           └─ on_close()
  │               ├─ cleanup_game()
  │               ├─ free(game)
  │               └─ mlx_close_window()
  │                   └─ exit(0)  // FIN ICI
  │
  └─ return (0);  // ← JAMAIS ATTEINT
```

**Alternative sans MLX** (pour comprendre) :
```c
// Si on n'utilisait pas MLX, ce serait :
while (game_running)
{
    handle_input();
    update_game();
    render_frame();
}
// Ici on sortirait de la boucle
cleanup_game(game);
free(game);
return (0);  // Code atteint normalement
```

---

## 🔍 Analyse des choix de conception

### 1. Pourquoi allouer `game` sur le heap (malloc/calloc) ?

**Heap vs Stack** :

```c
// Option A : Stack (NON utilisé)
int main(void)
{
    t_game game;  // Alloué sur la pile
    // Problème : limité en taille, détruit à la fin de main
}

// Option B : Heap (UTILISÉ)
int main(void)
{
    t_game *game = ft_calloc(1, sizeof(t_game));  // Alloué sur le tas
    // Avantages :
    // - Pas de limite de taille (sauf RAM)
    // - Persiste même si on quitte main
    // - Peut être passé facilement entre fonctions
}
```

**Avantages du heap** :
- `t_game` contient beaucoup de données (map, textures, player...)
- Stack overflow si trop gros (typiquement ~8 MB max)
- MLX stocke des pointeurs vers game (callbacks) → doit persister

### 2. Pourquoi ne pas combiner validation + initialisation ?

**Séparation des responsabilités** :

```c
// Mauvais design (tout mélangé)
int init_and_validate(int argc, char **argv, t_game *game)
{
    if (argc != 2) return (1);
    if (init_game(...) != 0) return (1);
    // Difficile de savoir où ça a échoué
}

// Bon design (séparé et clair)
if (argc != 2)        // 1. Validation simple
    return (...);
if (init_game(...))   // 2. Initialisation complexe
    return (...);
game_loop(...);       // 3. Exécution
```

**Principe UNIX** : "Fail fast, fail early"
- Vérifier les erreurs simples d'abord
- Pas besoin d'allouer des ressources si argc est faux
- Économie de temps et de mémoire

### 3. Gestion d'erreur : pourquoi différents niveaux ?

```c
// Niveau 1 : Erreur d'allocation
if (!game)
    return (1);  // Pas de free, game est NULL

// Niveau 2 : Erreur d'argument
if (argc != 2)
    return (free(game), 1);  // Free game seulement

// Niveau 3 : Erreur d'initialisation
if (init_game(...))
{
    cleanup_game(game);  // Free TOUT ce qui a été alloué
    free(game);
    return (1);
}
```

**Progression logique** :
```
Allocation → Arguments → Initialisation → Exécution
    ↓            ↓              ↓              ↓
   NULL      game vide    game partiel    game complet
    ↓            ↓              ↓              ↓
rien à free  free game    cleanup+free    cleanup+free
```

---

## 📊 Diagramme de flux complet

```
┌─────────────────────────────────────────────────┐
│              DÉBUT DU PROGRAMME                 │
│                  main(argc, argv)               │
└────────────────┬────────────────────────────────┘
                 │
                 ▼
      ┌──────────────────────┐
      │ game = ft_calloc()   │
      └──────────┬───────────┘
                 │
                 ▼
           [game == NULL ?]
                 │
        ┌────────┼────────┐
        │ OUI            │ NON
        ▼                ▼
   return (1)     ┌──────────────┐
                  │ argc == 2 ?  │
                  └──────┬───────┘
                         │
                ┌────────┼────────┐
                │ NON            │ OUI
                ▼                ▼
        ┌─────────────┐   ┌──────────────────┐
        │ Error msg   │   │ init_game() == 0?│
        │ free(game)  │   └────────┬─────────┘
        │ return (1)  │            │
        └─────────────┘   ┌────────┼────────┐
                          │ NON            │ OUI
                          ▼                ▼
                  ┌──────────────┐  ┌─────────────┐
                  │ cleanup_game │  │ game_loop() │
                  │ free(game)   │  │             │
                  │ return (1)   │  │ [INFINI]    │
                  └──────────────┘  └─────┬───────┘
                                          │
                                          ▼
                                  [Fermeture fenêtre]
                                          │
                                          ▼
                                  ┌──────────────┐
                                  │  on_close()  │
                                  │ cleanup_game │
                                  │ free(game)   │
                                  │ exit(0)      │
                                  └──────────────┘
```

---

## 🎯 Points clés pour la correction

### Questions fréquentes et réponses

**Q1 : Pourquoi `ft_calloc` et pas `malloc` ?**
- R : Initialise tout à zéro (NULL, 0)
- R : Évite les valeurs résiduelles (garbage values)
- R : Sécurise les pointeurs (NULL au lieu d'adresses aléatoires)
- R : Facilite la détection d'erreurs (`if (game->map)`)

**Q2 : Que se passe-t-il si on oublie `free(game)` ?**
- R : Memory leak (fuite mémoire)
- R : Valgrind le détectera
- R : Perte de points à la correction

**Q3 : Pourquoi `cleanup_game()` AVANT `free(game)` ?**
- R : `cleanup_game()` libère le CONTENU de game
- R : `free(game)` libère le CONTENEUR
- R : Ordre inverse = segfault (accès à mémoire libérée)

**Q4 : `return (0)` après `game_loop()` est-il utile ?**
- R : Jamais exécuté (mlx_loop infinie)
- R : Requis par le compilateur (fonction int)
- R : Bonne pratique (code propre)

**Q5 : Peut-on avoir plusieurs fenêtres ?**
- R : Techniquement oui (plusieurs mlx_init)
- R : Pas dans ce projet (une seule fenêtre = un seul jeu)

**Q6 : Que contient exactement `t_game` ?**
```c
typedef struct s_game {
    mlx_t       *mlx;      // Contexte MLX42
    mlx_image_t *img;      // Buffer de rendu
    t_map       *map;      // Carte du jeu
    t_player    *player;   // Position/direction joueur
} t_game;
```

---

## 🧪 Tests et validation

### Test 1 : Pas d'argument
```bash
$ ./cub3D
Error
Usage: ./cub3D <map.cub>
$ echo $?
1
```

### Test 2 : Trop d'arguments
```bash
$ ./cub3D map.cub extra
Error
Usage: ./cub3D <map.cub>
$ echo $?
1
```

### Test 3 : Fichier invalide
```bash
$ ./cub3D invalid.cub
Error
Failed to parse .cub file
$ echo $?
1
```

### Test 4 : Exécution normale
```bash
$ ./cub3D maps/test.cub
[Fenêtre s'ouvre, jeu démarre]
[Utilisateur ferme la fenêtre]
$ echo $?
0
```

### Test 5 : Valgrind (memory leaks)
```bash
$ valgrind --leak-check=full ./cub3D maps/test.cub
[...]
definitely lost: 0 bytes in 0 blocks
indirectly lost: 0 bytes in 0 blocks
[...]
ERROR SUMMARY: 0 errors from 0 contexts
```

---

## 📚 Résumé en 3 points

1. **`ft_calloc`** : Allocation propre (tout à zéro)
2. **Validation précoce** : Vérifier argc avant d'initialiser
3. **Nettoyage progressif** : Libérer selon ce qui a été alloué

---

## ✅ Checklist de compréhension

- [ ] Je sais pourquoi on utilise `ft_calloc` au lieu de `malloc`
- [ ] Je comprends la différence entre STDOUT et STDERR
- [ ] Je peux expliquer l'ordre `cleanup_game()` puis `free(game)`
- [ ] Je sais pourquoi `return (0)` n'est jamais exécuté
- [ ] Je connais les 4 étapes du main (calloc, validation, init, loop)
- [ ] Je peux dessiner le diagramme de flux au tableau
- [ ] Je comprends le principe "fail fast, fail early"

---

**Le main est simple, mais chaque ligne a son importance. C'est le chef d'orchestre qui coordonne tout le programme ! 🎼**
