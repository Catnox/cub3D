# cub3D - Projet 42

## 📋 Todo List

### 🏗️ Structure du projet
- [ ] Créer l'architecture des dossiers (src/, includes/, libs/)
- [ ] Configurer le Makefile complet
- [ ] Créer les fichiers headers (.h)
- [ ] Définir les structures principales

### 📖 Parsing du fichier .cub
- [ ] Parser les textures (NO, SO, WE, EA)
- [ ] Parser les couleurs (F, C) 
- [ ] Valider le format des couleurs RGB (0-255)
- [ ] Parser la carte
- [ ] Vérifier que la carte est fermée par des murs
- [ ] Vérifier qu'il y a exactement un joueur (N, S, E, W)
- [ ] Gérer les erreurs de parsing
- [ ] Libérer la mémoire en cas d'erreur

### 🎮 Initialisation du jeu
- [ ] Initialiser MLX
- [ ] Créer la fenêtre
- [ ] Charger les textures
- [ ] Initialiser la position du joueur
- [ ] Initialiser la direction du joueur
- [ ] Configurer le plan de la caméra

### 🔍 Raycasting
- [ ] Implémenter l'algorithme DDA (Digital Differential Analyzer)
- [ ] Calculer la distance aux murs
- [ ] Détecter les collisions avec les murs
- [ ] Distinguer les murs horizontaux et verticaux
- [ ] Calculer la hauteur des murs à l'écran

### 🎨 Rendu graphique
- [ ] Dessiner le sol avec la couleur spécifiée
- [ ] Dessiner le plafond avec la couleur spécifiée
- [ ] Appliquer les textures aux murs
- [ ] Gérer l'orientation des textures selon la direction du mur
- [ ] Optimiser le rendu pour éviter les scintillements

### ⌨️ Contrôles
- [ ] Déplacement avant/arrière (W, S)
- [ ] Déplacement latéral (A, D)
- [ ] Rotation de la caméra (flèches gauche/droite)
- [ ] Gestion de la fermeture de fenêtre (ESC, croix rouge)
- [ ] Éviter les déplacements dans les murs

### 🧠 Gestion mémoire
- [ ] Libérer toutes les textures
- [ ] Libérer la carte
- [ ] Libérer les structures MLX
- [ ] Gérer les fuites mémoire
- [ ] Tester avec valgrind

### ✅ Tests et validation
- [ ] Tester avec différents fichiers .cub
- [ ] Tester les cas d'erreur
- [ ] Vérifier les performances
- [ ] Tester sur différentes résolutions
- [ ] Validation avec les tests de l'école

### 📝 Bonus (optionnel)
- [ ] Collision avec les murs
- [ ] Minimap
- [ ] Portes qui s'ouvrent/ferment
- [ ] Sprites animés
- [ ] Rotation de la souris

## 🔧 Commandes utiles

```bash
# Compilation
make

# Nettoyage
make clean
make fclean

# Recompilation complète
make re

# Tests
./cub3D maps/test.cub
```

## 📁 Structure recommandée

```
cub3D/
├── src/
│   ├── main.c
│   ├── parsing/
│   ├── raycasting/
│   ├── rendering/
│   └── utils/
├── includes/
│   └── cub3d.h
├── maps/
│   └── *.cub
├── textures/
│   └── *.xpm
├── libs/
│   └── MLX42/
└── Makefile
```

## 🎯 Critères d'évaluation
- Gestion d'erreurs robuste
- Code propre et normé
- Pas de fuites mémoire
- Performance fluide
- Respect du sujet
