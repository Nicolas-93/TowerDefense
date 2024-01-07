- Grid
  - Ne pas stocker les calculs des points, mais les recalculer par un accesseur devrait réduire la complexité du code
  - Optimiser l'affichage en dessinant un seul grand rectangle, et appeler un callback qui donne la couleur pour chaque case en paramètre. Si la couleur est différente de celle du fond ou qu'il y a survol, redessiner la case, sinon ne rien faire
  - Pour le survol, prendre la couleur renvoyée, et l'assombrir
  - Eviter les mallocs lignes par lignes

- Path
  - Refactor logic, variables names

- Mana

- Interface (s'inspirer de l'original...)
  - Boutons d'achat :
    - Afficher le coût (survol ?)
    - Fusion des gemmes => Gérer le niveau (flèches ?)
    - Placement tour
    - Création de gemmes
  - Afficher le stock de mana

- Initier les vagues de monstres

- Monstre
  - Subir les dégâts
    - Effets des tirs (ex: ralentissements)
  - Afficher les barres d'HP

- Game
  - Land et Inventory devrait prendre en paramètre leur position par rapport à la grille, ce n'est pas aux objets créés de décider où se placer

# Potentielles améliorations

- Tirs
  - Effet 3D
  - Trajectoire courbée
