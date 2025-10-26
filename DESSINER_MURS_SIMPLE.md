# 🎨 DESSINER LES MURS - EXPLIQUÉ SIMPLEMENT

## 🎯 **L'IDÉE DE BASE**

Imaginez que vous **coloriez** une photo au crayon :
- Vous avez trouvé un **mur à distance X**
- Plus le mur est **proche** → ligne **plus haute** à dessiner
- Plus le mur est **loin** → ligne **plus courte** à dessiner  
- Vous coloriez **colonne par colonne** de gauche à droite

## 📐 **ÉTAPE 1 : CALCULER LA HAUTEUR DU MUR**

### **La formule magique** :
```
hauteur_mur = hauteur_écran / distance_mur

Exemples :
- Mur à distance 1.0 → hauteur = 768/1.0 = 768 pixels (très haut)
- Mur à distance 2.0 → hauteur = 768/2.0 = 384 pixels (moyen)
- Mur à distance 4.0 → hauteur = 768/4.0 = 192 pixels (petit)
```

**En gros** : Plus c'est loin, plus c'est petit !

### **Où dessiner sur l'écran** :
```
centre_écran = 768 / 2 = 384

début_mur = centre - (hauteur_mur / 2)
fin_mur = centre + (hauteur_mur / 2)

Exemple mur hauteur 200 :
- début = 384 - 100 = 284
- fin = 384 + 100 = 484
→ Je dessine de la ligne 284 à la ligne 484
```

## 🎨 **ÉTAPE 2 : CHOISIR LA COULEUR/TEXTURE**

### **4 directions de mur** :
```
Nord (ray->side=1, step_y<0) → Texture Nord
Sud (ray->side=1, step_y>0)  → Texture Sud  
Est (ray->side=0, step_x>0)  → Texture Est
Ouest (ray->side=0, step_x<0) → Texture Ouest
```

### **Si pas de texture** :
```
Nord → Blanc (debug)
Sud → Vert (debug)
Est → Rouge (debug)  
Ouest → Bleu (debug)
```

## 🖼️ **ÉTAPE 3 : APPLIQUER LA TEXTURE**

### **Trouver la position X sur la texture** :
```
wall_x = position exacte où le rayon a touché le mur (entre 0 et 1)
tex_x = wall_x × largeur_texture

Exemple :
- Le rayon touche le mur à 30% de sa largeur
- Texture de 64 pixels de large
- tex_x = 0.3 × 64 = 19
→ J'utilise la colonne 19 de la texture
```

### **Étirer la texture verticalement** :
```
step = hauteur_texture / hauteur_mur_écran

Pour chaque pixel Y à l'écran :
- tex_y = position_actuelle × step
- Je prends le pixel texture[tex_x][tex_y]
- Je le dessine à l'écran[x][y]
```

## 🎮 **EXEMPLE COMPLET**

```
1. DDA trouve un mur à distance 2.5
2. Hauteur mur = 768/2.5 = 307 pixels
3. Position écran : lignes 230 à 537
4. Direction : Est → Texture Est
5. Position mur : tex_x = 45 (colonne 45 de la texture)
6. Pour chaque ligne 230→537 :
   - tex_y = calcul_position_texture
   - couleur = texture_est[45][tex_y]  
   - écran[x][y] = couleur
```

## 🎨 **VISUALISATION SIMPLE**

```
Écran (vue de côté) :

    0  ←─ Plafond (couleur unie)
  230  ┌──────────┐ ← Début mur
      │ ████████ │
      │ ████████ │ ← Mur texturé
      │ ████████ │
  537  └──────────┘ ← Fin mur
  768  ←─ Sol (couleur unie)

Colonne par colonne de gauche à droite !
```

## 🔧 **LES 3 ÉTAPES SIMPLES**

### 1. **Calculer les limites** :
```c
hauteur = écran_hauteur / distance_mur
début = centre - hauteur/2  
fin = centre + hauteur/2
```

### 2. **Choisir la texture** :
```c
if (côté_vertical && droite) → texture_est
if (côté_vertical && gauche) → texture_ouest  
if (côté_horizontal && haut) → texture_nord
if (côté_horizontal && bas) → texture_sud
```

### 3. **Dessiner pixel par pixel** :
```c
for (y = début; y <= fin; y++) {
    tex_y = calcul_position_texture(y)
    couleur = texture[tex_x][tex_y]
    pixel_écran[x][y] = couleur
}
```

## 💡 **POURQUOI ÇA MARCHE**

### **Distance → Hauteur** :
- C'est de la **perspective** ! 
- Comme dans la vraie vie : loin = petit, proche = grand

### **Texture → Réalisme** :
- On **étire** l'image de la texture sur la hauteur du mur
- Comme **coller un poster** sur un mur de différente taille

### **Colonne par colonne** :
- Chaque **rayon** = une **colonne** d'écran
- 1024 rayons = 1024 colonnes = image complète !

## 🎯 **PHRASE MAGIQUE POUR LA CORRECTION**

> "Pour dessiner un mur : je calcule sa hauteur avec distance, je choisis la texture selon la direction, puis j'étire cette texture sur la hauteur calculée, pixel par pixel."

## 🧠 **QUESTIONS FRÉQUENTES**

### Q: "Comment on calcule la hauteur ?"
**R:** hauteur = écran_hauteur / distance. Plus c'est loin, plus c'est petit !

### Q: "Comment on choisit la texture ?"
**R:** Nord/Sud/Est/Ouest selon la direction du mur touché par le rayon.

### Q: "Comment on applique la texture ?"
**R:** On prend une colonne de la texture et on l'étire sur toute la hauteur du mur.

### Q: "Pourquoi colonne par colonne ?"
**R:** Parce qu'on a 1 rayon par colonne ! Chaque rayon dessine sa colonne.

## 🎮 **ANALOGIE FINALE**

**Dessiner les murs, c'est comme faire du coloriage magique :**
1. Tu mesures la distance au mur (règle)
2. Tu calcules quelle taille dessiner (proportions)  
3. Tu choisis ton crayon/texture (direction)
4. Tu colories ta colonne de haut en bas (pixel par pixel)
5. Tu passes à la colonne suivante !

**Et voilà, ton monde 3D est dessiné !** 🚀

---

## 📝 **POUR LA CORRECTION - RÉSUMÉ 30 SECONDES**

1. **"Je calcule la hauteur du mur"** : `hauteur = écran / distance`
2. **"Je trouve où dessiner"** : `début/fin = centre ± hauteur/2`  
3. **"Je choisis la texture"** : Nord/Sud/Est/Ouest selon direction
4. **"J'étire la texture"** : Colonne texture → colonne écran
5. **"Je dessine pixel par pixel"** : De début à fin

**Terminé !** ✅