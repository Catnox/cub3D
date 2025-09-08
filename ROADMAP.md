# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    ROADMAP.md                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/30 15:30:00 by radubos           #+#    #+#              #
#    Updated: 2025/08/30 15:30:00 by radubos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# 🎮 cub3D - Roadmap Complète Structurée

> Créer une représentation graphique 3D réaliste de l'intérieur d'un labyrinthe depuis une perspective à la première personne en utilisant le raycasting.

---

## 🚀 PHASE 1 : BASE OBLIGATOIRE (2-3 semaines)
### ✅ Objectif : Validation 42 + bonus de base

#### 1.1 Setup & Structure
- [x] **Architecture projet** : `src/`, `includes/`, `libs/`, `maps/`, `textures/`
- [x] **Libft** ajoutée à la racine `libft/` (norme 42)
- [ ] **MLX42 (Codam)** configurée avec support RGBA + opacité
- [x] **Headers** avec structures principales
- [x] **Makefile** opérationnel avec toutes les libs

#### 1.2 Parsing robuste (.cub)
- [ ] **Validation fichier** : extension `.cub` + ouverture sécurisée
- [ ] **Textures** : NO, SO, WE, EA (vérifier chemins + fichiers .xpm)
- [ ] **Couleurs** : F, C (RGB [0,255], format strict R,G,B)
- [ ] **Map parsing** : caractères `0,1,N,S,E,W` + espaces autorisés
- [ ] **Validation map** : fermée par murs, un seul joueur, flood-fill
- [ ] **Gestion erreurs** : messages clairs + nettoyage mémoire

#### 1.3 Raycasting Core
- [ ] **Algorithme DDA** : détection murs efficace (Digital Differential Analyzer)
- [ ] **Calcul distances** : correction fish-eye effect
- [ ] **Orientation murs** : distinction N/S/E/W pour textures appropriées
- [ ] **Coordonnées textures** : mapping précis pixel → texture

#### 1.4 Rendu MLX42
- [ ] **Sol/Plafond** : couleurs unies F/C
- [ ] **Murs texturés** : orientation correcte selon direction
- [ ] **Optimisation** : 60 FPS stable minimum
- [ ] **Fenêtre** : résolution fixe (recommandé : 1024x768)

#### 1.5 Contrôles de base
- [ ] **WASD** : déplacements fluides sans lag
- [ ] **Flèches gauche/droite** : rotation caméra
- [ ] **ESC** : fermeture propre du programme
- [ ] **Croix rouge** : hook de fermeture MLX
- [ ] **Collisions** : impossible de passer dans les murs

---

## ⭐ PHASE 2 : BONUS 42 OFFICIEL (1 semaine)
### ✅ Objectif : Points bonus maximum pour 42

#### 2.1 Fonctionnalités bonus obligatoires
- [ ] **Collisions réalistes** : sliding le long des murs (pas stop brutal)
- [ ] **Minimap** : affichage temps réel, position joueur, FOV
- [ ] **Portes** : symbole 'D' sur map, ouverture/fermeture avec espace
- [ ] **Souris** : rotation caméra (mlx_cursor_hook)
- [ ] **Animations simples** : textures qui changent dans le temps

---

## 🎯 PHASE 3 : GAMEPLAY CORE (2-3 semaines)
### 🎮 Objectif : Transformer en vrai jeu FPS

#### 3.1 HUD et Interface
- [ ] **Barre de vie** : système HP (ex: 100 points max)
- [ ] **Compteur munitions** : affichage arme actuelle + réserve
- [ ] **Score** : points par ennemi tué + items collectés
- [ ] **Crosshair** : réticule centré pour viser
- [ ] **Messages temporaires** : pickup items, game over, level complete

#### 3.2 Système d'Armes
- [ ] **Arme de poing** : munitions infinies, dégâts faibles (20 HP)
- [ ] **Fusil** : munitions limitées, dégâts élevés (60 HP)
- [ ] **Animation tir** : 4 frames (repos → feu → recul → repos)
- [ ] **Raycast shooting** : détection hits via raycasting précis
- [ ] **Système rechargement** : temps de cooldown entre tirs

#### 3.3 Système d'Ennemis
- [ ] **Sprites ennemis** : affichage 2D dans 3D (technique billboard)
- [ ] **IA basique** : patrouille entre waypoints
- [ ] **Détection joueur** : line of sight + distance
- [ ] **Système vie ennemis** : HP variables selon type
- [ ] **Animation mort** : sprite explosion + disparition

---

## 🤖 PHASE 4 : IA AVANCÉE + AUDIO (2 semaines)
### 🧠 Objectif : IA intelligente + immersion audio

#### 4.1 IA A* (Pathfinding Algorithm)
- [ ] **Implémentation A*** : 
  - Structure nœud (pos, g_cost, h_cost, parent)
  - Heuristique Manhattan pour labyrinthe
  - Open/closed lists optimisées
- [ ] **Grille de navigation** : conversion map → graph de déplacement
- [ ] **Comportements IA** :
  - **Patrouille** : parcours de waypoints définis
  - **Chasse** : A* vers position joueur
  - **Fuite** : A* vers position sûre si vie < 25%
- [ ] **Optimisation** : calcul A* étalé sur plusieurs frames

#### 4.2 Audio System (OpenAL recommandé)
```bash
# Setup OpenAL macOS
brew install openal-soft
# Flags Makefile : -framework OpenAL ou -lopenal
```
- [ ] **Wrapper OpenAL** : init, cleanup, play, stop
- [ ] **Musique background** : loop ambient (.ogg/.wav format)
- [ ] **SFX catalogue** :
  - Tirs d'armes (pistolet, fusil)
  - Ouverture/fermeture portes
  - Pas du joueur sur différents sols
  - Cris/morts ennemis
  - Pickup d'items
- [ ] **Audio 3D positionnel** : distance + direction pour immersion
- [ ] **Volume dynamique** : ajustable via config

#### 4.3 Ressources Audio Libres
- **Musique** : freesound.org, opengameart.org, incompetech.com
- **SFX** : zapsplat.com, BBC Sound Effects Library
- **Format** : .wav (non compressé) pour compatibilité

---

## 🔄 PHASE 5 : GÉNÉRATION PROCÉDURALE (2 semaines)
### 🎲 Objectif : Maps infinies + rejouabilité maximale

#### 5.1 Algorithme Recommandé : **Recursive Backtracking Maze**
**Pourquoi ce choix :**
- Simple à implémenter vs BSP
- Garantit solution (pas de zones isolées)
- Paramétrable (taille, densité murs)
- Résultats cohérents

#### 5.2 Implémentation étape par étape
- [ ] **Grille base** : 
  ```c
  char **create_empty_grid(int width, int height);
  // Calloc + remplir de '1' (murs partout)
  ```
- [ ] **Algorithme carving** :
  ```c
  void carve_maze(char **grid, int x, int y);
  // Récursif : marquer '0', choisir direction random, continuer
  ```
- [ ] **Spawn placement** :
  - Joueur : zone accessible aléatoire
  - Ennemis : minimum 5 cases du joueur
  - Items : distribution équilibrée
- [ ] **Export .cub** : sauvegarder map générée

#### 5.3 Système de niveaux
- [ ] **Seed system** : `srand(seed)` pour reproductibilité
- [ ] **Difficulté progressive** :
  - Level 1 : 15x15, 2 ennemis
  - Level 5 : 25x25, 8 ennemis + items
  - Level 10+ : 35x35, boss fights
- [ ] **Validation automatique** : vérifier map générée est jouable

---

## 🎨 PHASE 6 : POLISH & FEATURES AVANCÉES (2 semaines)
### ✨ Objectif : Finitions + expérience immersive

#### 6.1 Contrôles Avancés
- [ ] **Course** : LEFT_SHIFT + vitesse × 1.5 (stamina system optionnel)
- [ ] **Strafe amélioré** : normalisations diagonales pour vitesse constante
- [ ] **Vue haut/bas** : 
  - Pitch angle (-30° à +30°)
  - Modification hauteur horizon
- [ ] **Smoothing movements** : interpolation pour fluidité

#### 6.2 Animations Avancées
- [ ] **Portes coulissantes** :
  - 8-10 frames d'ouverture progressive
  - Timer auto-fermeture (5 secondes)
  - Collision detection pendant animation
  - Son synchronisé
- [ ] **Murs animés** :
  - Changement couleur selon progression niveau
  - Textures animées (eau, lave, néons)
  - Frame rate indépendant du jeu
- [ ] **Système particules** : impacts balles, explosions ennemis

#### 6.3 Système d'Items Complet
- [ ] **Nouveaux symboles map** :
  - 'T' : Trésor (+100 points)
  - 'H' : Kit médical (+25 HP)
  - 'A' : Munitions (+10 balles)
  - 'K' : Clés (ouvre portes verrouillées)
- [ ] **Pickup mechanics** : collision player + disappear + effet
- [ ] **Inventaire** : gestion clés, munitions par type d'arme
- [ ] **High score** : sauvegarde persistante (fichier)

---

## 🌙 PHASE 7 : THÈMES & MODDING (1-2 semaines)
### 🎭 Objectif : Customisation + ambiance immersive

#### 7.1 Système de Thèmes
- [ ] **Config JSON** : `themes/theme_name.json`
- [ ] **Thèmes disponibles** :
  - **Wolfenstein Classic** : textures historiques, gris/brun
  - **Sci-fi Cyberpunk** : néons, bleu/violet, métallique
  - **Horror Survival** : sombre, rouge/noir, rouille
- [ ] **Hot-swap** : changement thème sans redémarrage

#### 7.2 Environnement Dynamique
- [ ] **Lune de sang** : UI element progression (rouge selon kill count)
- [ ] **Éclairage dynamique** :
  - Zones sombres (réduction visibilité)
  - Sources lumière (torches, néons)
  - Ombres simples
- [ ] **Effets météo** : si map a zones "extérieures"

#### 7.3 Modding System
- [ ] **Asset loading** : dossiers `mods/mod_name/`
- [ ] **Config override** : paramètres jeu modifiables
- [ ] **Texture packs** : remplacement assets facile

---

## 🧪 PHASE 8 : TESTS & OPTIMIZATION (1 semaine)
### 🔧 Objectif : Stabilité production + performance

#### 8.1 Testing Framework Automatisé
- [ ] **Script bash** : test_cub3d.sh
- [ ] **Maps de test** :
  - Valides : simples, complexes, edge cases
  - Invalides : erreurs parsing, maps ouvertes
- [ ] **Performance monitoring** : FPS counter + log
- [ ] **Memory leak detection** : valgrind + sanitizers

#### 8.2 Optimisations Finales
- [ ] **Frustum culling** : ne calculer que rayons visibles
- [ ] **Sprite LOD** : niveau détail selon distance ennemis
- [ ] **Memory pooling** : réutiliser allocations fréquentes
- [ ] **Profiling avancé** : identifier bottlenecks avec instruments

---

## 🛠️ STACK TECHNIQUE RECOMMANDÉE

### 📚 **Librairies**
- **MLX42 (Codam)** : rendu + events (support RGBA + transparence)
- **libft** : fonctions utilitaires
- **OpenAL Soft** : audio 3D cross-platform
- **Math.h** : fonctions trigonométriques

### 🏗️ **Architecture finale recommandée**
```
cub3D/
├── src/
│   ├── core/           # main.c, init.c, cleanup.c
│   ├── parsing/        # parse_cub.c, validate_map.c, parse_colors.c
│   ├── raycasting/     # dda.c, raycasting.c, distance.c
│   ├── rendering/      # render.c, textures.c, sprites.c
│   ├── controls/       # keyboard.c, mouse.c, movements.c
│   ├── gameplay/       # weapons.c, enemies.c, items.c, hud.c
│   ├── ai/             # astar.c, pathfinding.c, behaviors.c
│   ├── audio/          # audio_manager.c, openal_wrapper.c
│   ├── generation/     # maze_generation.c, level_manager.c
│   └── utils/          # utils.c, math_utils.c, cleanup_utils.c
├── includes/
│   ├── cub3d.h         # structures principales + prototypes
│   ├── parsing.h       # fonctions parsing
│   ├── rendering.h     # fonctions rendu
│   ├── gameplay.h      # fonctions jeu
│   └── audio.h         # fonctions audio
├── libft/              # ta libft (à la racine - norme 42)
├── libs/
│   └── MLX42/          # auto-download par Makefile
├── assets/
│   ├── textures/       # .xpm files
│   ├── sounds/         # .wav files
│   └── themes/         # theme configs
├── maps/               # .cub test files
└── tests/              # scripts de test
```

---

## 📊 PRIORITÉS DE DÉVELOPPEMENT

### 🔴 **Critiques (Phase 1-2)** - Validation 42
1. Parsing complet et robuste
2. Raycasting fonctionnel
3. Rendu basique + textures
4. Contrôles fluides
5. Bonus 42 (minimap, portes, souris)

### 🟡 **Importantes (Phase 3-4)** - Gameplay
1. HUD informatif
2. Système d'armes
3. Ennemis + IA basique
4. Audio immersif

### 🟢 **Bonus Fun (Phase 5-8)** - Innovation
1. Génération procédurale
2. IA A* avancée
3. Thèmes + modding
4. Optimisations poussées

---

## 💡 CONSEILS DE DÉVELOPPEMENT

### 🎯 **Méthodologie**
- **Test-driven** : tester chaque module séparément
- **Commits atomiques** : une feature = un commit
- **Branches par phase** : garder main stable
- **Code review** : relire code avant merge

### 🔧 **Outils recommandés**
- **Valgrind** : détection memory leaks
- **GDB** : debugging avancé
- **Instruments** (macOS) : profiling performance
- **Norminette** : respect norme 42

### 📈 **Métriques de succès**
- **Performance** : 60 FPS constant
- **Memory** : 0 leaks détectées
- **Robustesse** : 0 crash sur inputs invalides
- **Code quality** : Norminette OK + lisibilité

---

## 🏁 **GETTING STARTED - Prochaine Action**

### Étape immédiate :
1. **Créer structure dossiers** (Phase 1.1)
2. **Ajouter libft** dans libs/
3. **Premier fichier** : `src/core/main.c` avec parsing basique
4. **Test simple** : afficher "Hello cub3D" avec MLX42

### Premier objectif concret :
**Parser un fichier .cub simple et afficher les infos parsées**

Tu es prêt à commencer ? On attaque par quelle partie ? 🚀
