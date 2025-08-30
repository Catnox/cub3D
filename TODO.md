# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    TODO.md                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/30 15:30:00 by radubos           #+#    #+#              #
#    Updated: 2025/08/30 15:30:00 by radubos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# 🎯 cub3D - Plan d'action détaillé

## Phase 1 : Setup et Structure (Priorité 1) 🏗️

### 1.1 Environnement de développement
- [ ] Cloner/ajouter la libft dans `libs/libft/`
- [ ] Configurer MLX42 (voir Makefile)
- [ ] Créer l'architecture des dossiers :
  ```
  src/
  ├── parsing/
  ├── raycasting/
  ├── rendering/
  ├── controls/
  └── utils/
  includes/
  maps/ (fichiers de test)
  textures/ (fichiers .xpm)
  ```

### 1.2 Headers et structures
- [ ] Créer `includes/cub3d.h` avec :
  - [ ] Structure pour les textures
  - [ ] Structure pour la carte
  - [ ] Structure pour le joueur
  - [ ] Structure pour les rayons
  - [ ] Structure principale du jeu
  - [ ] Prototypes des fonctions

## Phase 2 : Parsing (Priorité 1) 📖

### 2.1 Validation du fichier
- [ ] Vérifier l'extension `.cub`
- [ ] Ouvrir et lire le fichier
- [ ] Gérer les erreurs d'ouverture

### 2.2 Parsing des éléments
- [ ] Parser les textures (NO, SO, WE, EA)
  - [ ] Vérifier que les fichiers .xpm existent
  - [ ] Éviter les doublons
- [ ] Parser les couleurs (F, C)
  - [ ] Format : R,G,B
  - [ ] Valeurs entre 0-255
  - [ ] Pas d'espaces parasites
- [ ] Parser la carte
  - [ ] Caractères autorisés : 0, 1, N, S, E, W, espaces
  - [ ] Exactement un joueur
  - [ ] Carte rectangulaire (remplir avec des espaces)

### 2.3 Validation de la carte
- [ ] Vérifier que la carte est fermée
- [ ] Flood fill pour détecter les trous
- [ ] Position du joueur accessible

## Phase 3 : Initialisation MLX (Priorité 1) 🎮

### 3.1 Setup MLX
- [ ] Initialiser MLX
- [ ] Créer la fenêtre (résolution fixe, ex: 1024x768)
- [ ] Charger les textures
- [ ] Configurer les hooks (clavier, fermeture)

### 3.2 Initialisation du joueur
- [ ] Position de départ (coordonnées de N/S/E/W)
- [ ] Direction initiale selon l'orientation
- [ ] Plan de la caméra (perpendiculaire à la direction)

## Phase 4 : Raycasting (Priorité 1) 🔍

### 4.1 Algorithme DDA
- [ ] Pour chaque colonne de pixels :
  - [ ] Calculer la direction du rayon
  - [ ] Trouver la première intersection
  - [ ] Avancer case par case jusqu'au mur
  - [ ] Calculer la distance

### 4.2 Détection des murs
- [ ] Distinguer murs nord/sud (textures NO/SO)
- [ ] Distinguer murs est/ouest (textures WE/EA)
- [ ] Calculer le point d'impact exact

## Phase 5 : Rendu (Priorité 1) 🎨

### 5.1 Calculs d'affichage
- [ ] Hauteur du mur à l'écran selon la distance
- [ ] Correction de l'effet "fish eye"
- [ ] Calcul des coordonnées de texture

### 5.2 Dessin
- [ ] Sol (couleur unie en bas)
- [ ] Plafond (couleur unie en haut)
- [ ] Murs avec textures
- [ ] Gestion de la transparence/limites

## Phase 6 : Contrôles (Priorité 1) ⌨️

### 6.1 Déplacements
- [ ] W/S : avant/arrière
- [ ] A/D : gauche/droite (strafe)
- [ ] Vérifier les collisions avant déplacement

### 6.2 Rotation
- [ ] Flèches gauche/droite
- [ ] Mise à jour direction + plan caméra
- [ ] Vitesse de rotation appropriée

### 6.3 Autres contrôles
- [ ] ESC : quitter proprement
- [ ] Croix rouge : fermeture fenêtre

## Phase 7 : Optimisation et Tests (Priorité 2) ✅

### 7.1 Performance
- [ ] Optimiser le raycasting
- [ ] Éviter les calculs inutiles
- [ ] Frame rate stable

### 7.2 Gestion mémoire
- [ ] Libérer toutes les allocations
- [ ] Pas de fuites (valgrind)
- [ ] Gestion d'erreurs propre

### 7.3 Tests
- [ ] Maps valides diverses
- [ ] Maps invalides (erreurs)
- [ ] Cas limites

## Phase 8 : Bonus (Optionnel) ⭐

- [ ] Minimap en temps réel
- [ ] Collisions réalistes avec les murs
- [ ] Portes animées
- [ ] Rotation à la souris
- [ ] Sprites/objets dans la carte

## 📝 Notes de développement

### Ordre recommandé :
1. **Parsing complet** (critique pour la suite)
2. **Setup MLX + fenêtre de base**
3. **Raycasting simple** (murs sans texture)
4. **Textures et rendu final**
5. **Contrôles et polish**

### Points d'attention :
- **Norminette** : respecter la norme 42
- **Gestion d'erreurs** : tous les malloc, open, etc.
- **Performance** : le jeu doit être fluide
- **Memory leaks** : absolument aucune fuite

### Fichiers de test utiles :
- Maps simples (carré 3x3)
- Maps complexes
- Maps avec erreurs (pour tester le parsing)

## 🚀 Getting Started

1. Commencer par le parsing (phase 2)
2. Créer des maps de test simples
3. Tester chaque fonction au fur et à mesure
4. Intégrer MLX42 une fois le parsing OK
