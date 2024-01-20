\pagebreak

# Manuel utilisateur

## Compilation

Pour compiler le projet, il suffit de lancer la commande `make` à la racine du projet. Cela créera un dossier `build` contenant les fichiers objets et l'exécutable `td`.

Vous pouvez également lancer la commande `make clean` pour supprimer les fichiers objets, ou `make distclean` pour supprimer également l'exécutable.

## Utilisation

Pour lancer le programme, il suffit de lancer la commande `./build/td` depuis la racine du projet (nécessaire pour le chargement correct des icônes)

Vous pouvez également utiliser ces options :

- ``-w``, ``--window=WIDTHxHEIGHT`` : Taille de la fenêtre, par défaut (800x480)

- ``-f``, ``--fullscreen`` : Maximise la taille de la fenêtre à la taille de l'écran

- ``-h``, ``--help`` : Affiche un message d'aide

Par exemple :

```shell
./build/td --window=500x500
```

## Documentation doxygen

Les fonctions étant documentées, vous pouvez générer la documentation, et tous les graphes de dépendances avec ``doxygen``. une cible ``make`` est prévue à cet effet :

```shell
make doc
```

Dans le répertoire ``doc`` se trouve une copie du dépôt [``doxygen-awesome``](https://github.com/jothepro/doxygen-awesome-css). C'est un thème CSS pour doxygen, celui par défaut de ``doxygen`` étant assez rudimentaire, désuet et peu agréable. Nous avons décidé de l'intégrer dans le rendu.
La documentation peut très bien être compilé sans.

\pagebreak

# Manuel technique

## Structures de données du projet

Lors de la phase de structuration du projet, et de ses différents modules, nous avons supposé l'existence de deux structures de données, afin d'aisément diviser en sous-problèmes chacun d'entre-eux. Nous avons réalisé ces structures en restant *générique*. C'est une des raisons que le développement du projet est dans une optique "orientée objet".

Nous appliquons ces conventions autant que nécessaire :

- Chaque nom de fonction d'un module est préfixée par le nom du module,

- Les fonctions (méthodes) d'une structure prennent en premier paramètre leur structure concernée sous le nom de ``self``.

- Les fonctions privées sont préfixées par un sous-tiret (*underscore*).

### ArrayList (tableau dynamique)

Cette structure de données a été tout d'abord pensée pour stocker les monstres du jeu, cependant, par la suite, nous avons estimé nécessaire l'utilisation d'une liste doublement chainée (voir la prochaine partie).
Actuellement, elle est utilisée pour stocker le chemin.
En effet, nous avons jugés plus aisé de stocker les positions de chacun de ses virages pour réaliser le déplacement des monstres.

Afin de l'implémenter nous utilisons un tableau d'octets (``uint8_t``) pour stocker les élements. Le prototype d'initialisation est assez simple :

```c
ArrayListError ArrayList_init(
    ArrayList* self,
    size_t element_size, /* Taille d'un élément stocké */
    size_t initial_capacity, /* Nombre d'éléments pouvant être
    initialement stockés */
    int (*cmp)(const void*, const void*) /* Fonction de comparaison pour trier 
    le tableau si nécessaire */
)
```

L'accès aux éléments du tableau peut se réaliser de deux façons différentes :

- Avec l'accesseur ``ArrayList_get``, qui permet une indexation semblable à Python (les indices négatifs permettent un accès inverse au tableau)

- En utilisant la macro-définition ``ARRAYLIST_DECLARE_ARRAY`` : qui permet de déclarer localement un pointeur vers le tableau stocké avec un type correct.

### Deque (liste doublement chainée)

Utilisée pour les monstres et tirs, la liste doublement chainée est simple d'utilisation, et permet une meilleure souplesse pour les suppressions.

#### Limites des tableaux dynamiques

Les monstres peuvent disparaitrent aussi bien en fin, qu'au début et millieu d'une liste. Or, l'usage d'un tableau empêche une suppresion en $O(1)$, c'est pourquoi l'usage d'un *deque* (*Double Ended QUEue*) est particulièrement intéressant.

D'autre part, chaque monstre possède une liste de ses prochains tirs arrivant vers lui, et chacun de ces tirs possèdent un pointeur vers leur cible (la trajectoire du monstre). Or, dans le cas d'un redimensionnement d'un tableau, celui-ci pourrait changer d'adresse, et rendre caduc tous les pointeurs des tirs, corrompant le reste du programme.

\pagebreak

#### Implémentation

##### ``sys/queue`` oui mais...

Afin d'implémenter de façon générique ce module, nous avons tout d'abord pensés à la bibliothèque ``sys/queue``. Elle permet de créer simplement les structures de nœuds, et à l'aide de ses multiples macros de les manipuler.
Il y a tout de même quelques inconvénients, mineurs, mais existants :

D'une part, il est nécessaire de définir deux structures pour chaque liste (entête et nœud), or, ceci rend son utilisation moins naturelle, et pose des questions au niveau de la modularisation : devons-nous créer un module par liste pour leurs définition ? Ajouter les définitions dans le module du type stocké ?

D'autre part, par défaut, ``sys/queue`` ne stocke pas le nombre d'éléments dans son entête, il faut user de délicatesses pour ajouter une variable à celle-ci, et la mettre à jour manuellement.

Le pré-requis initial est de ne pas devoir à utiliser de ``void*`` dans les nœuds, car cela nécessiterait que l'utilisateur alloue l'espace nécessaire à l'objet qu'il souhaite stocker, en plus de requérir un déréférencement supplémentaire et un défaut de localité en mémoire.
Or nous souhaitons que l'utilisation soit transparente pour l'utilisateur, comme avec ``sys/queue``.

Il est ainsi nécessaire de stocker les objets dans les nœuds. Mais comment faire si on ne connait pas la taille des objets, et donc de la structure de nœud à la compilation ?

##### Flexible Array Member

Normalisé par la norme C99, après l'existence de *hacks* avec la déclaration de tableaux de taille nulles, les *Flexible Array Member* (FAM) permetent la déclaration d'un tableau dans une structure, dont sa taille est inconnue à la compilation. Celui-ci doit être placé impérativement comme dernier membre d'une structure.
Cependant une contrainte naturelle s'ajoute : la structure ne peut plus être elle-même utilisée pour déclarer un tableau de ce type. En effet, le tableau fait partie intégrante de structure, et n'est pas un pointeur.

```c
typedef struct DequeNode {
    struct DequeNode* _prev;
    struct DequeNode* _next;
    uint8_t elem[]; // Flexible Array Member
} DequeNode;
```

En pratique, son utilisation revient à allouer l'espace pour les membres de la structure (l'opérateur ``sizeof`` calcule la taille de la structure, sans le FAM) auquel on ajoute la taille du tableau que l'on souhaite allouer :

```c
malloc(sizeof(DequeNode) + self->elem_size)
```

Nous pouvons désormais stocker nos données, passées par pointeur, dans nos nœuds à l'aide de simples ``memcpy`` :

```c
memcpy(node->elem, elem, self->elem_size);
```

\pagebreak

## Modules de jeu

Avant de commencer à détailler les différents modules, il est nécessaire de rappeler que dans l'optique d'organiser le plus efficacement possible les différents objets graphique, nous adoptons les conventions suivantes pour le nommage des noms de fonctions :

- ``*_process_event`` : Récupère le dernier évenement, et le traite si nécessaire.

- ``*_update`` : Met à jour le comportement de l'objet graphique (déplacement, tirs, etc...).

- ``*_draw`` : Dessine l'objet, et ses sous-objets s'il en contient.

### Event

Afin de limiter le passage en paramètre d'un événement à tous les objets présents dans le jeu (terrain, boutons, inventaire), nous avons fait le choix de stocker le dernier événement dans une variable statique dans ce module. 

L'événement est récupéré une fois par boucle avec la fonction ``Event_pop_event``, et peut être récupéré par les différents modules du projet avec  ``Event_get`` autant de fois que nécessaire pendant une boucle du jeu.

### Timer

Afin de gérer commodément le temps, et les minuteurs dans le jeu (espacement des monstres, délai d'activation des tours, tirs, etc...), nous avons créé un module pemettant de définir un temps à atteindre pour déclarer un timer comme terminé :

Cette fonction récupère le temps (*timestamp*) actuel, et lui ajoute le nombre nécéssaire de millisecondes à attendre :

```c
Timer Timer_new_ms(uint64_t milliseconds)
```

Pour tester si un timer est terminé, nous avons seulement à utiliser la fonction ``Timer_is_over``.

Cependant, lors de la génération des vagues de monstres qui nécessite d'espacer de façon régulière les monstres, nous avons remarqué un défaut de régularité entre eux.


La méthode que nous utilisons pour générer une vague, est assez simple : nous utilisons une boucle ``for``, et nous créons chacun des monstres avec un nouveau timer de démarrage.
Après quelques essais, nous avons remarqué que limiter le nombre d'appels à la fonction ``timespec_get`` résolvait ce problème.

Par conséquent, nous avons choisi de stocker le temps courant de la frame actuelle dans une variable statique au timer, celle-ci étant actualisée à un temps fixe et régulier évite que l'appel ``Timer_new_ms`` appelle ``timespec_get`` pour autant de monstres créés.

\pagebreak

### Grid

Afin de pouvoir organiser aisément l'affichage, l'idée est de structurer les objets selon une grille, à la manière de CSS.

Nous définissons une grille occupant toute la superficie de la fenêtre, puis nous y greffons les différentes objets graphiques par des positions relatives à la grille parente :

```c
Grid_new(
        &self->viewport,
        10, 6, 0.95, // Grille de 10x6, avec une marge de 5% aux bords
        NULL, // Ne dépend pas d'une grille parente
        (Rect) {
            .ax = 0, .ay = 0,
            .bx = win_size.width, .by = win_size.height
        }, // Position de la grille, par rapport à la fenêtre
        true, /* Nous voulons que la grille ne soit pas étirée
        (les cases doivent restées carrées) */
        MLV_COLOR_WHITE, MLV_COLOR_RED
    )
```

Nous pouvons par exemple, utiliser ceci pour placer le terrain :

```c
Grid_new(
    &self->grid,
    width, height, 1,
    parent,
    (Rect) {
        .ax = 0, .ay = 0,
        .bx = 7, .by = parent->height - 1
    },
    true, MLV_COLOR_WHITE, MLV_COLOR_BLACK
)
```

D'autre part, cette grille permet de simplifier la gestion d'un événement clic, sans devoir placer directement dans la boucle principale du jeu un test d'événement.

Pour cela, nous utilisons des fonctions de rappel (*callback*), que nous attachons après l'initialisation de la grille.

Ici, la fonction ``_Land_on_grid_click`` sera rappellée lors d'un clic, avec en argument son instance :

```c
Grid_set_on_click_handler(&self->grid, _Land_on_grid_click, self)
```

Il existe également une variante pour les survols sur les cases d'une grille :

```c
Grid_set_on_hover_handler(&self->grid, _Buttons_on_grid_hover, self);
```

\pagebreak

### DragNdrop

Pour gérer le déplacement des gemmes de grille en grille, de façon intuitive, nous avons choisi, d'implémenter un glisser-déposer.

Nous avons également pris le soin que son utilisation soit générique, et finalement cela ne complexifie pas son code.

Nous avons divisé le problème d'un glisser-déposer comme ceci :

- Ajouter un élément dans la main de l'utilisateur (le curseur)

- Appeler une fonction au relâchement

Pour cela, nous stockons dans une variable statique l'objet à déplacer (on peut avancer sans prendre de risques, que l'utilisateur a un unique curseur, et puisse déplacer un seul élément à la fois), ainsi que les fonctions à appeler au relâchement.

Il faut également prévoir le cas où l'utilisateur relâche l'objet sur une position illégale (en dehors de toute grille, voir en dehors de la fenêtre). Nous demandons donc à la fonction d'ajout, de prendre également en paramètre une fonction de rappel en cas d'échec, pour déposer l'objet à sa position initiale.

Nous avons par exemple cet appel pour gérer le début du glisser-déposer depuis l'inventaire :

```c
DragNDrop_put(
    gem, /* La gemme à ajouter */
    Gem_draw_dragndrop, /* fonction à appeler pour continuer à dessiner la gemme */
    self->game, /* instance d'objet à utiliser pour la fonction suivante */
    Game_on_gem_release, /* fonction de rappel lorsque la gemme est relâchée, 
    et renvoie false, si un échec c'est produit */
    self, /* instance d'objet à utiliser pour la fonction suivante */
    _Inventory_on_gem_release_failure /* fonction de rappel lorsque le 
    relâchement a échoué */
);
```

### Land

Dans ce module, nous stockons les objets principaux du jeu. A savoir, les monstres, tours, le chemin et la grille du terrain. Nous l'utilisons principalement pour appeler les fonctions des différents objets qu'il contient, mais également les clics sur le terrain, ainsi que l'action a réaliser lorsqu'une gemme est déposée sur une tour.

### Game

De la même façon que ``Land``, ce module sert à l'initialisation et à appeler les fonctions de ses objets.

Nous y définissions également les fonctions de rappel pour les boutons situés dans le coin haut-droit de la grille, et les actions à réaliser lorsqu'une gemme est relâchée sur une des grilles (inventaire ou terrain).

\pagebreak

### Overlay

Un prérequis essentiel l'utilisateur est de pouvoir obtenir des informations sur l'objet qu'il survole, donc de dessiner un encart.

Pour simplifier cette démarche une fonction avec un prototype semblable à ``printf`` serait idéal :

```c
void Overlay_draw(Point pos, const char *text, ...)
```

Cependant un problème se pose ici. Que ce passe-t-il si l'utilisateur de la fonction commet une erreur sur les types des paramètres ? Dans le cas courant, l'utilisateur de ``printf`` a le droit à un message d'avertissement explicite émit par le compilateur :

```c
warn_type.c:5:14: warning: format ‘%s’ expects argument of type ‘char *’,
but argument 2 has type ‘double’ [-Wformat=]
    5 |     printf("%s", (double) 42.0);
      |             ~^   ~~~~~~~~~~~~~
      |              |   |
      |              |   double
      |              char *
      |             %f
```

Pour corriger ce défaut de notre fonction, nous avons ajouté l'attribut ``format`` supporté par [``gcc`` et ``clang``](https://releases.llvm.org/3.8.0/tools/clang/docs/AttributeReference.html#:~:text=Clang%20supports%20the%20format%20attribute).

Nous avons donc ce prototype :

```c
// 2 : indice de la chaine de formatage.
// 3 : indice du premier argument à vérifier
__attribute__ ((format (printf, 2, 3)))
void Overlay_draw(Point pos, const char *text, ...);
```

Les erreurs deviennent alors plus naturelles :

```c
src/game.c:13:43: warning: format ‘%d’ expects argument of type ‘int’,
but argument 3 has type ‘double’ [-Wformat=]
   13 |     Overlay_draw(pos, "New tower\n(cost: %d mana)", 5.0);
      |                                          ~^         ~~~
      |                                           |         |
      |                                           int       double
      |                                          %f
```


## Dépendances automatisées

Avec le nombre de modules conséquent du projet, la définition manuelle des dépendances dans le Makefile devient fastidieuse. Nous avons alors opté pour l'usage d'une fonctionnalité commune à ``gcc`` et ``clang`` qui permet de les générer automatiquement, sous forme de fichiers ``.d`` pour chacun des modules.

Par conséquent, cela évite tout erreur humaine à la maintenance de ces dépendances, et allège considérablement le Makefile.

En pratique, il suffit d'ajouter l'argument ``-MMD`` au compilateur et de les inclure dans le Makefile comme ceci :

```bash
-include $(wildcard $(BUILD_DIR)/*.d)
```
