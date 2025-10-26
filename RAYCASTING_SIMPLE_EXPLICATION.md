# 🔥 RAYCASTING EXPLIQUÉ SIMPLEMENT

## 🎮 **L'IDÉE DE BASE**

Imaginez que vous êtes dans un labyrinthe avec une **lampe de poche** :
- Vous balayez de **gauche à droite** avec votre lampe
- Pour chaque direction, vous regardez **jusqu'où va la lumière** avant de toucher un mur
- Plus le mur est **proche** → colonne **haute** sur l'écran
- Plus le mur est **loin** → colonne **basse** sur l'écran

## 📏 **LES 3 MESURES IMPORTANTES**

### 1. **DELTA_DIST** : "Combien j'avance par case"
```
Si mon rayon va direction (0.5, 0.8) :
- delta_dist.x = |1/0.5| = 2  ← Pour traverser 1 case en X, je parcours 2 unités
- delta_dist.y = |1/0.8| = 1.25  ← Pour traverser 1 case en Y, je parcours 1.25 unités
```

**En gros** : C'est ma "vitesse" pour traverser la grille.

### 2. **STEP** : "Dans quelle direction j'avance"
```
Si mon rayon va vers la droite : step_x = +1
Si mon rayon va vers la gauche : step_x = -1
Si mon rayon va vers le haut : step_y = -1  
Si mon rayon va vers le bas : step_y = +1
```

**En gros** : C'est ma "direction" dans la grille (+1 ou -1).

### 3. **SIDE_DIST** : "Combien il reste jusqu'à la prochaine ligne"
```
Je suis à la position (2.3, 1.7) et je vais vers la droite :
- side_dist.x = distance jusqu'à la ligne x=3
- side_dist.y = distance jusqu'à la ligne y=1 ou y=2
```

**En gros** : C'est la distance pour atteindre la prochaine "ligne de grille".

## 🚶‍♂️ **COMMENT ÇA MARCHE (ALGORITHME DDA)**

### **Étape 1 : Je regarde mes deux distances**
```
side_dist.x = 1.2  (distance ligne verticale)
side_dist.y = 0.8  (distance ligne horizontale)
```

### **Étape 2 : Je prends la plus petite**
```
0.8 < 1.2 → Je vais vers la ligne horizontale
```

### **Étape 3 : J'avance dans cette direction**
```
map_y = map_y + step_y  (nouvelle case Y)
side_dist.y = side_dist.y + delta_dist.y  (prochaine ligne Y)
```

### **Étape 4 : Je vérifie si c'est un mur**
```
grid[map_y][map_x] == '1' ?  → MUR TROUVÉ !
Sinon → Je recommence à l'étape 1
```

## 🎨 **VISUALISATION SIMPLE**

```
Grille 4x4, joueur à (1.3, 1.8) :

     0   1   2   3
   ┌───┬───┬───┬───┐
 0 │   │   │   │   │
   ├───┼───┼───┼───┤
 1 │   │ P │ 2 │ 4 │  P=start
   ├───┼───┼───┼───┤  Nombres = ordre des étapes
 2 │   │ 1 │ 3 │ ■ │  ■=mur trouvé !
   └───┴───┴───┴───┘

Étapes DDA :
1. Compare distances → va en Y → case (1,2)
2. Compare distances → va en X → case (2,2)  
3. Compare distances → va en Y → case (2,1)
4. Compare distances → va en X → case (3,1)
5. Compare distances → va en Y → case (3,2) → MUR !
```

## 💡 **POURQUOI GAUCHE/DROITE EST IMPORTANT**

### **Direction vers la DROITE** :
```c
step_x = +1
side_dist.x = (prochaine_case - position_actuelle) × delta_dist.x
```

### **Direction vers la GAUCHE** :
```c
step_x = -1  
side_dist.x = (position_actuelle - case_actuelle) × delta_dist.x
```

**Pourquoi différent ?** 
- Droite : Je vais vers x=3, x=4, x=5...
- Gauche : Je vais vers x=1, x=0, x=-1...
- Les calculs de distance sont **inversés** !

## 🔑 **RÉSUMÉ ULTRA SIMPLE**

1. **Je lance un rayon** pour chaque colonne d'écran
2. **Je calcule 3 trucs** : vitesse (delta_dist), direction (step), distance initiale (side_dist)
3. **Je avance case par case** en prenant toujours le chemin le plus court
4. **Je m'arrête** quand je touche un mur
5. **Je calcule la distance** pour dessiner la hauteur du mur

## 🎯 **PHRASE MAGIQUE POUR LA CORRECTION**

> "Le DDA, c'est comme marcher dans une grille en prenant toujours le chemin le plus court vers la prochaine intersection, jusqu'à taper dans un mur. Les 3 mesures (delta_dist, step, side_dist) me disent juste : ma vitesse, ma direction, et où est la prochaine intersection."

## 🧠 **QUESTIONS FRÉQUENTES SIMPLIFIÉES**

### Q: "C'est quoi delta_dist concrètement ?"
**R:** Si tu marches en diagonale, delta_dist te dit combien tu avances au total pour faire 1 pas en X ou 1 pas en Y.

### Q: "Pourquoi side_dist change selon la direction ?"
**R:** Parce que si tu vas à droite, tu regardes la distance jusqu'au mur de droite. Si tu vas à gauche, tu regardes la distance jusqu'au mur de gauche !

### Q: "Comment on sait qu'on a touché un mur ?"
**R:** On regarde dans la grille : `grid[y][x] == '1'` = mur. Simple !

### Q: "Pourquoi comparer side_dist.x et side_dist.y ?"
**R:** Pour aller au plus proche ! Tu prends toujours le chemin le plus court vers la prochaine intersection.

## 🎮 **ANALOGIE FINALE**

**Le raycasting, c'est comme jouer au jeu du "plus chaud/plus froid" :**
- Tu avances case par case dans la grille
- À chaque étape, tu regardes : "Où est la prochaine intersection la plus proche ?"
- Tu y vas directement
- Tu répètes jusqu'à tomber sur un mur
- La distance parcourue = hauteur du mur à l'écran

**C'est tout !** 🚀

---

## 📝 **POUR LA CORRECTION**

**Si le correcteur demande le raycasting en 30 secondes :**

1. "Je lance 1024 rayons, un par colonne d'écran"
2. "Chaque rayon traverse la grille case par case avec l'algorithme DDA"
3. "DDA = je prends toujours le chemin le plus court vers la prochaine intersection"
4. "Je m'arrête au premier mur et je calcule la distance"
5. "Distance proche = mur haut, distance loin = mur bas"

**Fini !** ✅