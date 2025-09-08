# 🎮 cub3D - Advanced Raycasting Engine

Un moteur de raycasting 3D avancé inspiré de Wolfenstein 3D, développé pour l'école 42.

## 🚀 Features

### ✅ Core (Obligatoire 42)
- Raycasting engine avec textures orientées
- Parsing robuste de fichiers .cub
- Contrôles fluides (WASD + flèches)
- Sol et plafond colorés

### ⭐ Bonus 42
- Minimap en temps réel
- Portes interactives
- Rotation à la souris
- Collisions réalistes

### 🎯 Advanced Features
- IA ennemis avec algorithme A*
- Système d'armes et HUD
- Audio 3D positionnel (OpenAL)
- Génération procédurale de niveaux
- Système de thèmes et modding

## 🛠️ Installation

```bash
# Cloner le repo
git clone <your-repo-url>
cd cub3D

# Ajouter ta libft dans libs/libft/
# Le Makefile téléchargera automatiquement MLX42

# Compiler
make

# Tester
./cub3D maps/test_simple.cub
```

## 🎮 Contrôles

| Touche | Action |
|--------|--------|
| W, A, S, D | Déplacements |
| ← → | Rotation caméra |
| Souris | Rotation libre |
| ESC | Quitter |
| ESPACE | Ouvrir portes |
| SHIFT | Course (bonus) |

## 📁 Structure du projet

```
cub3D/
├── src/
│   ├── core/           # Initialisation et boucle principale
│   ├── parsing/        # Lecture fichiers .cub
│   ├── raycasting/     # Algorithme DDA
│   ├── rendering/      # Affichage MLX42
│   ├── controls/       # Gestion inputs
│   ├── gameplay/       # Armes, ennemis, items
│   ├── ai/             # Intelligence artificielle A*
│   ├── audio/          # Système audio OpenAL
│   ├── generation/     # Génération procédurale
│   └── utils/          # Fonctions utilitaires
├── includes/           # Headers
├── libft/              # Libft à la racine (norme 42)
├── libs/
│   └── MLX42/          # MLX42 auto-téléchargée
├── assets/
│   ├── textures/       # Fichiers .xpm
│   ├── sounds/         # Fichiers audio
│   └── themes/         # Configurations thèmes
├── maps/               # Fichiers .cub de test
└── tests/              # Scripts de validation
```

## 📋 Todo List

- [x] Architecture projet
- [x] Headers et structures
- [x] Fichiers de base avec headers 42
- [ ] Parsing complet .cub
- [ ] Raycasting fonctionnel
- [ ] Rendu basique
- [ ] Contrôles
- [ ] Textures
- [ ] Bonus 42
- [ ] Gameplay avancé
- [ ] IA et audio
- [ ] Génération procédurale

## 🧪 Tests

```bash
# Lancer les tests automatiques
chmod +x tests/test_cub3d.sh
./tests/test_cub3d.sh

# Test manual
./cub3D maps/test_simple.cub
./cub3D maps/test_door.cub

# Memory leaks
valgrind ./cub3D maps/test_simple.cub
```

## 🎨 Système de thèmes

Les thèmes sont configurables via des fichiers JSON dans `assets/themes/` :
- `wolfenstein_classic.json` - Thème pierre classique
- `cyberpunk_neon.json` - Thème futuriste néon

## 📚 Ressources

- [MLX42 Documentation](https://github.com/codam-coding-college/MLX42)
- [Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html)
- [A* Algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm)
- [OpenAL Guide](https://www.openal.org/documentation/)

---

**Développé par radubos (@Catnox) - École 42 Mulhouse**
