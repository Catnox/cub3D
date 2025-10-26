# MOVEMENTS.C - EXPLICATION DÉTAILLÉE

## 📋 TABLE DES MATIÈRES

1. [Vue d'ensemble](#vue-densemble)
2. [Fonction rotate_player](#fonction-rotate_player)
3. [Mathématiques de rotation](#mathématiques-de-rotation)
4. [Questions fréquentes](#questions-fréquentes)
5. [Diagrammes](#diagrammes)

---

## 🎯 VUE D'ENSEMBLE

### Rôle du fichier
`movements.c` contient uniquement la fonction de **rotation du joueur** :
- Calcul de rotation 2D avec matrice de rotation
- Mise à jour du vecteur direction
- Mise à jour du vecteur plan (caméra)

### Fonction unique
```
movements.c
└── rotate_player() - Rotation du joueur selon un angle donné
```

### Dépendances mathématiques
```c
#include <math.h>  // Pour cos() et sin()
```

---

## 🔄 FONCTION ROTATE_PLAYER

### Signature
```c
void rotate_player(t_game *game, double angle)
```

### Vue d'ensemble
Cette fonction effectue une **rotation 2D** du joueur en appliquant une matrice de rotation aux vecteurs :
- **Vecteur direction** : Direction du regard
- **Vecteur plan** : Plan de la caméra (perpendiculaire à direction)

### Analyse ligne par ligne

```c
void	rotate_player(t_game *game, double angle)
{
```
**Paramètres** :
- `game` : Structure principale contenant le joueur
- `angle` : Angle de rotation en **radians** (positif = anti-horaire)

```c
	double	old_dir_x;
	double	old_plane_x;
```
**Variables temporaires** :
- Sauvegarde des anciennes valeurs X
- Nécessaire car on modifie X et Y en même temps
- Évite de corrompre les calculs

### Rotation du vecteur direction

```c
	old_dir_x = game->player->dir.x;
```
**Sauvegarde direction X** :
- Stocke l'ancienne valeur avant modification
- Sera utilisée pour calculer la nouvelle direction Y

```c
	game->player->dir.x = game->player->dir.x * cos(angle)
		- game->player->dir.y * sin(angle);
```
**Nouvelle direction X** :
- Applique la formule de rotation 2D
- `new_x = old_x * cos(θ) - old_y * sin(θ)`
- Première ligne de la matrice de rotation

```c
	game->player->dir.y = old_dir_x * sin(angle)
		+ game->player->dir.y * cos(angle);
```
**Nouvelle direction Y** :
- Utilise l'**ancienne** valeur X (old_dir_x)
- `new_y = old_x * sin(θ) + old_y * cos(θ)`
- Deuxième ligne de la matrice de rotation

### Rotation du vecteur plan

```c
	old_plane_x = game->player->plane.x;
```
**Sauvegarde plan X** :
- Même logique que pour direction
- Évite la corruption des calculs

```c
	game->player->plane.x = game->player->plane.x * cos(angle)
		- game->player->plane.y * sin(angle);
```
**Nouveau plan X** :
- Identique à la rotation de direction
- Même matrice de rotation appliquée

```c
	game->player->plane.y = old_plane_x * sin(angle)
		+ game->player->plane.y * cos(angle);
```
**Nouveau plan Y** :
- Utilise l'**ancienne** valeur X du plan
- Complète la rotation du vecteur plan

```c
}
```

### Formule mathématique complète

**Matrice de rotation 2D** :
```
[cos(θ)  -sin(θ)] [x]   [x*cos(θ) - y*sin(θ)]
[sin(θ)   cos(θ)] [y] = [x*sin(θ) + y*cos(θ)]
```

**Application** :
```c
// Rotation direction
new_dir.x = old_dir.x * cos(θ) - old_dir.y * sin(θ)
new_dir.y = old_dir.x * sin(θ) + old_dir.y * cos(θ)

// Rotation plan (identique)
new_plane.x = old_plane.x * cos(θ) - old_plane.y * sin(θ)
new_plane.y = old_plane.x * sin(θ) + old_plane.y * cos(θ)
```

---

## 📐 MATHÉMATIQUES DE ROTATION

### Concepts de base

#### Vecteur direction
```
Direction du regard du joueur
- Vecteur unitaire (longueur = 1)
- Indique où le joueur regarde
- Utilisé pour avancer/reculer
```

#### Vecteur plan
```
Plan de projection de la caméra
- Perpendiculaire au vecteur direction
- Largeur du champ de vision
- Utilisé pour le raycasting et strafe
```

#### Relation direction ↔ plan
```
Si direction = (dx, dy)
Alors plan = (-dy, dx)  // Rotation 90° sens horaire

Exemple :
Direction = (1, 0)  → Regarde à droite
Plan      = (0, 1)  → Plan vertical
```

### Angles et directions

#### Convention d'angles
```
        270° (-π/2)
             ↑
             |
180° (π) ←---+---→ 0° (0)
             |
             ↓
         90° (π/2)

Angle positif = rotation anti-horaire
Angle négatif = rotation horaire
```

#### Exemples concrets

**Rotation de 90° (π/2 radians)** :
```
cos(π/2) = 0
sin(π/2) = 1

Direction (1,0) → (0,1)
- De "droite" vers "haut"
```

**Rotation de -90° (-π/2 radians)** :
```
cos(-π/2) = 0
sin(-π/2) = -1

Direction (1,0) → (0,-1)
- De "droite" vers "bas"
```

### Utilisation dans le jeu

#### ROT_SPEED typique
```c
#define ROT_SPEED 0.025  // ~1.43 degrés

// Rotation gauche (touche ←)
rotate_player(game, -ROT_SPEED);

// Rotation droite (touche →)
rotate_player(game, ROT_SPEED);
```

#### Vitesse de rotation
```
0.025 radians/frame à 60 FPS
= 0.025 * 60 = 1.5 radians/seconde
= 86 degrés/seconde

Tour complet (360°) en ~4.2 secondes
```

### Pourquoi sauvegarder old_x ?

**Problème sans sauvegarde** :
```c
// ❌ MAUVAIS
dir.x = dir.x * cos(θ) - dir.y * sin(θ);  // dir.x modifié
dir.y = dir.x * sin(θ) + dir.y * cos(θ);  // Utilise NOUVEAU dir.x !
```

**Solution avec sauvegarde** :
```c
// ✅ BON
old_x = dir.x;                             // Sauvegarde
dir.x = dir.x * cos(θ) - dir.y * sin(θ);  // Utilise ancien dir.x
dir.y = old_x * sin(θ) + dir.y * cos(θ);  // Utilise ancien dir.x sauvé
```

**Exemple numérique** :
```
Direction initiale : (1, 0)
Rotation 90° : cos(90°)=0, sin(90°)=1

Sans sauvegarde (❌) :
dir.x = 1*0 - 0*1 = 0
dir.y = 0*1 + 0*0 = 0  ← ERREUR ! Vecteur nul

Avec sauvegarde (✅) :
old_x = 1
dir.x = 1*0 - 0*1 = 0
dir.y = 1*1 + 0*0 = 1  ← CORRECT ! (0,1)
```

---

## ❓ QUESTIONS FRÉQUENTES

### Q1 : Pourquoi utiliser des radians et pas des degrés ?
**R :**
```c
cos(angle)  // angle DOIT être en radians
sin(angle)  // angle DOIT être en radians
```
- Les fonctions math.h utilisent les radians
- 1 radian ≈ 57.3 degrés
- π radians = 180 degrés

### Q2 : Pourquoi faire tourner direction ET plan ?
**R :**
- **Direction** : Pour que le joueur regarde dans la bonne direction
- **Plan** : Pour que le raycasting fonctionne correctement
- Le plan DOIT rester perpendiculaire à direction

### Q3 : Comment s'assurer que les vecteurs restent unitaires ?
**R :**
La rotation **preserve la longueur** des vecteurs :
```c
// Si |v| = 1 avant rotation
// Alors |v'| = 1 après rotation
// (propriété mathématique des matrices orthogonales)
```

### Q4 : Que se passe-t-il si on oublie de sauvegarder old_x ?
**R :**
```c
// Sans sauvegarde → Calculs corrompus
dir.x = new_value;
dir.y = utilise_new_value;  // ❌ Au lieu de old_value

// Résultat : Rotation incorrecte, vecteurs déformés
```

### Q5 : Comment inverser une rotation ?
**R :**
```c
// Rotation de +θ
rotate_player(game, theta);

// Rotation inverse de -θ
rotate_player(game, -theta);

// Résultat : Retour à la position initiale
```

### Q6 : La rotation affecte-t-elle la position du joueur ?
**R :** **NON** !
```c
void rotate_player(t_game *game, double angle)
{
    // Modifie SEULEMENT direction et plan
    // game->player->pos reste INCHANGÉ
}
```
- Position = où est le joueur
- Direction = où il regarde

### Q7 : Quelle est la différence entre rotation et mouvement ?
**R :**
```
Mouvement (movement_utils.c) :
- Change la POSITION du joueur
- Vérifie les collisions
- Utilise les vecteurs direction/plan

Rotation (movements.c) :
- Change la DIRECTION du joueur
- Pas de collision à vérifier
- Modifie les vecteurs eux-mêmes
```

### Q8 : Pourquoi les angles négatifs pour tourner à gauche ?
**R :**
```
Convention mathématique :
+ = anti-horaire = gauche (sur écran retourné)
- = horaire      = droite

Dans un repère Y vers le bas (écran) :
- = anti-horaire = gauche (intuitivement correct)
+ = horaire      = droite
```

### Q9 : Comment calculer l'angle actuel du joueur ?
**R :**
```c
double current_angle = atan2(game->player->dir.y, game->player->dir.x);
```
- `atan2()` donne l'angle d'un vecteur
- Retour en radians [-π, π]

### Q10 : La rotation peut-elle causer des bugs de précision ?
**R :** Théoriquement **OUI**, mais négligeable :
```c
// Après de NOMBREUSES rotations (milliers)
// Les erreurs d'arrondi peuvent s'accumuler
// → Vecteurs plus parfaitement unitaires

// Solution (si nécessaire) : Renormaliser périodiquement
double length = sqrt(dir.x*dir.x + dir.y*dir.y);
dir.x /= length;
dir.y /= length;
```

---

## 📊 DIAGRAMMES

### Matrice de rotation 2D

```
Matrice de rotation de θ radians :

    [cos(θ)  -sin(θ)]
R = [sin(θ)   cos(θ)]

Application :
[x']   [cos(θ)  -sin(θ)] [x]
[y'] = [sin(θ)   cos(θ)] [y]

Résultat :
x' = x*cos(θ) - y*sin(θ)
y' = x*sin(θ) + y*cos(θ)
```

### Exemple de rotation 45°

```
Rotation de 45° (π/4 radians) :
cos(45°) ≈ 0.707
sin(45°) ≈ 0.707

Direction initiale : (1, 0)

Calcul :
x' = 1*0.707 - 0*0.707 = 0.707
y' = 1*0.707 + 0*0.707 = 0.707

Direction finale : (0.707, 0.707)

Vérification :
√(0.707² + 0.707²) = √(0.5 + 0.5) = 1 ✓
```

### Évolution des vecteurs

```
Vue de dessus, rotation horaire (angle négatif) :

    Étape 1: Initial          Étape 2: -45°           Étape 3: -90°
    
      plan                      plan                    plan
       ↑                       ↗                       →
       │                      ╱                       │
   ────┼────→ direction   ────┼────→              ────┼────
       │                      ╲                       │
                              ↘                       ↓
                            direction               direction

Vecteurs toujours perpendiculaires et unitaires !
```

### Rotation dans le contexte du raycasting

```
                 Plan de projection
                      │
                      │ ← Largeur FOV
          Ray N ─────╱│╲───── Ray 0
                   ╱  │  ╲
                 ╱    │    ╲
               ╱      │      ╲
         Direction ───┼───→ Player
               ╲      │      ╱
                 ╲    │    ╱
                   ╲  │  ╱
          Ray W ─────╲│╱───── Ray E

Quand player tourne → Direction ET Plan tournent
                   → Tous les rayons suivent la rotation
```

### Flux d'exécution

```
[INPUT] Flèche gauche/droite appuyée
          ↓
keyboard.c:handle_movement_rotation()
          ↓
    mlx_is_key_down(MLX_KEY_LEFT) ?
          ↓ TRUE
movements.c:rotate_player(game, -ROT_SPEED)
          ↓
    Sauvegarde old_dir_x et old_plane_x
          ↓
    Applique matrice rotation sur direction
          ↓
    Applique matrice rotation sur plan
          ↓
    Joueur regarde dans nouvelle direction
          ↓
    [NEXT FRAME] Raycasting avec nouveaux vecteurs
```

### Comparaison avant/après rotation

```
AVANT rotation (direction = (1,0), plan = (0,1)) :

    Raycasting horizontal →
    ┌─────────────────────────┐
    │         Scene           │
    │                         │
    │    [Player regardant →] │
    │                         │
    └─────────────────────────┘

APRÈS rotation de 90° (direction = (0,1), plan = (-1,0)) :

    Raycasting vertical ↑
    ┌─────────────────────────┐
    │         Scene           │
    │                         │
    │    [Player regardant ↑] │
    │                         │
    └─────────────────────────┘

Le raycasting suit automatiquement la rotation !
```

### Ordre des variables dans le calcul

```
rotate_player(game, angle)
     │
     ├─→ [1] old_dir_x = dir.x        // Sauvegarde
     │
     ├─→ [2] dir.x = dir.x*cos - dir.y*sin    // Nouveau X
     │
     ├─→ [3] dir.y = old_dir_x*sin + dir.y*cos // Nouveau Y (avec ancien X)
     │
     ├─→ [4] old_plane_x = plane.x    // Sauvegarde
     │
     ├─→ [5] plane.x = plane.x*cos - plane.y*sin   // Nouveau X
     │
     └─→ [6] plane.y = old_plane_x*sin + plane.y*cos // Nouveau Y

⚠️  CRUCIAL : Utiliser old_x pour calculer nouveau Y
⚠️  Sinon les calculs sont corrompus !
```

---

## 🎓 CHECKLIST POUR LA CORRECTION

### Comprendre la rotation 2D
- [ ] Expliquer la matrice de rotation 2D
- [ ] Comprendre pourquoi sauvegarder old_x
- [ ] Identifier les deux vecteurs modifiés (direction + plan)
- [ ] Expliquer la préservation de la longueur

### Comprendre les paramètres
- [ ] Expliquer l'unité (radians vs degrés)
- [ ] Comprendre le sens de rotation (+ vs -)
- [ ] Identifier la vitesse typique (ROT_SPEED)

### Comprendre l'impact sur le jeu
- [ ] Position inchangée (seule direction change)
- [ ] Impact sur le raycasting
- [ ] Relation direction ↔ plan perpendiculaire

### Questions pièges possibles
- [ ] "Pourquoi sauvegarder old_dir_x ?"
  → Éviter la corruption lors du calcul simultané X/Y
  
- [ ] "Quelle est l'unité de l'angle ?"
  → Radians (requis par cos/sin)
  
- [ ] "Pourquoi faire tourner direction ET plan ?"
  → Direction pour le regard, plan pour le raycasting
  
- [ ] "La rotation change-t-elle la position ?"
  → NON, seule la direction de regard change
  
- [ ] "Comment inverser une rotation ?"
  → Appliquer l'angle opposé (-angle)

### Concepts clés à maîtriser
- [ ] **Matrice de rotation** : Transformation 2D orthogonale
- [ ] **Vecteurs unitaires** : Direction et plan de longueur 1
- [ ] **Radians** : Unité d'angle pour les fonctions math
- [ ] **Perpendiculaire** : Plan toujours ⊥ à direction
- [ ] **Préservation** : Rotation conserve les longueurs

### Debugging commun
- [ ] Vecteurs qui deviennent nuls → old_x non sauvegardé
- [ ] Rotation dans le mauvais sens → signe de l'angle
- [ ] Rotation trop rapide → ROT_SPEED trop élevé
- [ ] Perte de précision → Accumulation d'erreurs d'arrondi

---

## 🎯 FIN DE L'EXPLICATION

Ce fichier couvre **100% du code** de `movements.c` avec des explications détaillées adaptées pour une correction de projet 42.

**Fichiers d'explication disponibles** :
1. ✅ `EXECUTION_GUIDE.md` - Vue d'ensemble du raycasting
2. ✅ `MAIN_EXPLICATION.md` - Explication de main.c
3. ✅ `INIT_EXPLICATION.md` - Explication de init.c
4. ✅ `GAME_LOOP_EXPLICATION.md` - Explication de game_loop.c
5. ✅ `KEYBOARD_EXPLICATION.md` - Explication de keyboard.c
6. ✅ `MOVEMENT_UTILS_EXPLICATION.md` - Explication de movement_utils.c
7. ✅ `MOVEMENTS_EXPLICATION.md` - Explication de movements.c (ce fichier)

**Système complet de mouvement expliqué** :
- Input (keyboard.c) → Détection touches
- Translation (movement_utils.c) → Déplacement avec collisions
- Rotation (movements.c) → Changement de direction

**Prochaines étapes suggérées** :
- Comprendre le lien mathématique entre tous les fichiers
- Visualiser l'impact de la rotation sur le raycasting
- Tester les explications de matrices de rotation

Bon courage pour la correction ! 🚀