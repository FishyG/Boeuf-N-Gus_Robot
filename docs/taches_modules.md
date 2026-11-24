# Tâches des modules — comment tout s'emboîte

Rédigé par Karolann (intégration). Complète le guide *PIRUS_Pratiques_programmation* et le
planning Excel. Les modules de Christopher, Lyévan, Christian et Karolann sont **reliés** :
ce document dit ce que chacun doit fournir aux autres.


Les noms de fonctions ci-dessous sont **des propositions à valider ensemble** au jalon J1
(interfaces figées). Chacun écrit son `.h` et son `.cpp`.

---

## 0. Deux mots à comprendre d'abord

### La machine à états (`src/etats.cpp`)
C'est **le cerveau** du robot. Le robot est toujours dans **un seul état** : « j'attends une
commande », « je roule », « je prends un ingrédient »… À chaque tour de `loop()`, la fonction
`etats_executer()` :
1. fait le travail de l'état courant (ex. en `DEPLACEMENT` : suivre la ligne, lire la couleur) ;
2. vérifie si une condition est vraie (ex. « une station est détectée ») ;
3. si oui, passe à l'état suivant (ex. `ARRET_STATION`).

C'est **le seul fichier qui appelle plusieurs modules**. Les modules ne se parlent jamais
entre eux. Par exemple, `arret_robot` répond « RECULER », et c'est `etats` qui demande
ensuite à `suiveur_ligne` de reculer. Grâce à ça, chacun peut coder son module seul.

Tant qu'un module n'est pas livré, son appel est en commentaire dans `etats.cpp` :
```cpp
// TODO(Christopher) : suiveur_suivre(sens, VITESSE_BASE);
```
Quand ton module arrive, Karolann enlève le `//` et le projet utilise ton vrai code.

### Un stub (une « fausse version »)
Un stub est une **fausse version** d'une fonction : elle a le même nom, les mêmes paramètres,
et retourne une réponse simple, sans le vrai matériel. Exemple pour la commande :
```cpp
#if STUB_COMMANDE
// Fausse version : une commande fixe, toujours prete
bool commande_disponible() { return true; }
Commande commande_lire() {
    Commande c = {{ING_PAIN_SOUS, ING_VIANDE, ING_PAIN}, 3};
    return c;
}
#else
// Vraie version : decodage IR NEC
...
#endif
```
**À quoi ça sert :** la machine à états peut tourner sur la table **avant** que le lecteur
IR, le bras ou la DEL soient prêts. Personne n'attend personne. On passe à la vraie
version en mettant `STUB_COMMANDE` à `0` dans `config.h`.

Exemple déjà dans le projet : le stub de `couleur` simule une ligne avec 6 stations. Il
« roule » dans le sens que lui donne la machine à états. C'est ce qui permet de tester
toute une commande, marche arrière comprise, sans robot.

---

## 1. Démarrer ton module

```
include/                 les interfaces (.h)
  config.h  types.h      partagés par tous
  <module>.h
src/                     les implémentations (.cpp)
  main.cpp               BoardInit() + etats_init() + etats_executer(), rien d'autre
  etats.cpp              la machine à états (appelle les modules)
  <module>.cpp
  tests/test_<module>.cpp   programme de test de ton module (remplace main.cpp)
lib/LibRobUS/            librairie externe du cours (on n'y touche pas)
```

**Pourquoi `include/` et pas `lib/` ?** Dans PlatformIO, `lib/` sert aux **librairies
indépendantes** du projet (comme LibRobUS), chacune dans son propre dossier. Une librairie
ne doit pas dépendre du projet, et elle ne voit pas `config.h` ni `types.h`. Nos modules
ont besoin de ces deux fichiers : ce sont des parties du projet, donc `.h` dans `include/`
et `.cpp` dans `src/`. PlatformIO trouve alors `#include "config.h"` tout seul. Pas de
chemin comme `"..\lib\x.h"` : les `\` ne marchent que sous Windows.

## 2. Le parcours et la marche arrière

```
[ROUGE]   [ORANGE]   [BLEU]     [VERT]      [JAUNE]          [MAUVE]
station 0 station 1  station 2  station 3   station 4        station 5
pains     fromage    boulette   salade      tomate, oignon   client
                                cornichon
```
(ordre et couleurs **à confirmer**, tout est dans `config.h` : `STATIONS_EN_ORDRE`, `STATION_xxx`)

- Le parcours est une **boucle avec 3 tournants** : c'est le travail de `suiveur_ligne`
  (Christopher). `arret_robot` ne voit que les stations, dans l'ordre.
- Le capteur reconnaît **6 couleurs**, donc plusieurs ingrédients partagent une
  station (les 2 pains sont au même endroit).
- Chaque couleur n'apparaît **qu'une fois** : la couleur lue dit **où** est le robot.
- Le robot doit pouvoir **reculer**. Exemple : « pain, tomate, boulette, oignon, pain » se
  prend du bas vers le haut : pain du dessous (0) → oignon (4) → **recule** à la boulette (2)
  → tomate (4) → **recule** au pain du dessus (0) → client (5).
- Le sens est un type commun dans `types.h` : `enum Sens { SENS_AVANT, SENS_ARRIERE }`.

---

## 3. `arret_robot`

### CU-5 : S'arrêter à une station et décider la suite
- **Acteur :** machine à états.
- **Scénario principal :**
  1. En `DEPLACEMENT`, `etats` donne chaque couleur lue à `arret_nouvelle_station(c)`.
  2. Quand le robot arrive sur une **nouvelle** station, la fonction retourne `true` (une
     seule fois par station) et retient la position du robot.
  3. `etats` arrête les moteurs ; le robot reste arrêté `DUREE_ARRET` ms (`arret_termine()`).
  4. `arret_decider(prochain_ingredient)` compare la position du robot à
     la station visée et répond : `PRENDRE`, `LIVRER`, `AVANCER`, `RECULER` ou `ERREUR`.
- **Cas alternatifs :**
  - Le prochain ingrédient est à la même station (2 pains, tomate puis oignon) : `PRENDRE`
    tout de suite, sans bouger.
  - Ingrédient sans station dans `config.h` : `ERREUR`.

| Fonction | Rôle |
|---|---|
| `void arret_init()` | robot à `POSITION_DEPART`, appelée par `etats_init()` |
| `bool arret_nouvelle_station(Couleur c)` | `true` une fois en arrivant sur une station |
| `void arret_demarrer()` / `bool arret_termine()` | chronomètre d'arrêt, non bloquant |
| `ActionStation arret_decider(Ingredient prochain)` | prendre, livrer, avancer, reculer (`ING_AUCUN` = tout est pris, aller livrer) |
| `int8_t arret_position()` | position actuelle (pour les `LOG`) |

---

## 4. `couleur`

### CU-4 : Détecter une station
- **Acteur :** machine à états, à chaque tour en `DEPLACEMENT`.
- **Scénario :** `couleur_lire()` lit le TCS34725, classe la couleur (seuils de `config.h`)
  et la retourne seulement après `LECTURES_ANTIREBOND` lectures identiques.
  Sur la ligne normale : `COULEUR_AUCUNE`.
- **Le capteur lit en continu** : `couleur_lire()` est appelée à chaque tour en `DEPLACEMENT`.
  La fonction couleur déjà écrite par l'équipe sera adaptée ici (T3.2).
- **Test :** `pio run -e test_couleur -t upload -t monitor` affiche chaque changement de couleur.
- **Fini quand :** chaque station reconnue 10 fois sur 10, y compris **en reculant**.

---

## 5. `suiveur_ligne`

### CU-3a : Avancer le long de la ligne
- **Acteur :** machine à états, état `DEPLACEMENT`, à chaque tour de `loop()`.
- **Préconditions :** le robot est sur la ligne ; `suiveur_init()` a été appelée.
- **À savoir :** le parcours est une boucle avec 3 tournants.
- **Scénario principal :**
  1. `etats` appelle `suiveur_suivre(SENS_AVANT, VITESSE_BASE)`.
  2. Le module lit les capteurs de ligne et corrige **une fois** la vitesse des moteurs.
  3. Il rend la main tout de suite (aucun `delay()`), pour que `etats` lise la couleur.
- **Postconditions :** les moteurs tournent ; le robot reste sur la ligne.

### CU-3b : Reculer le long de la ligne
- **Acteur :** machine à états, quand `arret_robot` répond `RECULER`.
- **Scénario principal :** comme CU-3a, avec `suiveur_suivre(SENS_ARRIERE, VITESSE_RECUL)`.
- **À choisir par Christopher :** reculer en suivant la ligne avec les mêmes capteurs, ou
  faire un demi-tour puis avancer. Avec un demi-tour, il faudra prévenir Karolann :
  après le demi-tour, la station suivante est atteinte en avançant.
- **Fini quand :** le robot recule d'une station à la précédente sans perdre la ligne.

### CU-3c : S'arrêter
- `suiveur_arreter()` coupe les deux moteurs **tout de suite**. Appelée en arrivant en station et en `ERREUR`.

### Cas alternatif commun : ligne perdue
À discuter. Proposition : `suiveur_ligne_perdue()` retourne `true`, et `etats` passe en `ERREUR`.

| Fonction | Quand | Bloquant ? |
|---|---|---|
| `void suiveur_init()` | `etats_init()` | non |
| `void suiveur_suivre(Sens sens, float vitesse)` | chaque tour, en `DEPLACEMENT` | **non** |
| `void suiveur_arreter()` | arrivée en station, `ERREUR` | non |

**Stub attendu :** ne touche pas aux moteurs ; un `LOG` seulement quand le robot change
d'état (roule en avant, roule en arrière, arrêté). Pas à chaque tour, sinon le moniteur
série est inondé.

**Fini quand :** un tour complet sans perdre la ligne (T1.3), en avançant **et** en reculant.

**À noter :** la branche `SuiveurLigne` utilise `SuiveurLigne.h` ; le guide demande
`suiveur_ligne.h` (minuscules, snake_case).

---

## 6. `commande`

Chaîne complète : carte du chef → **manette** (lecteur RFID) → code **IR NEC** → récepteur IR
du robot (broche 37, `REMOTE_read()` de LibRobus) → `Commande`.
Ce module est le **côté robot** : il reçoit le code NEC et le traduit.

### CU-2 : Recevoir une commande
- **Acteurs :** le client (présente sa carte) et la manette (envoie le code).
- **Préconditions :** le robot est en `ATTENTE_COMMANDE`.
- **Scénario principal :**
  1. La manette lit la carte et envoie un code NEC.
  2. À chaque tour, `etats` appelle `commande_disponible()`, qui lit `REMOTE_read()`.
  3. Le code est reconnu → `commande_disponible()` retourne `true`.
  4. `etats` appelle `commande_lire()` et reçoit une `Commande` (voir `types.h`).
- **Cas alternatifs :**
  - Code d'une autre télécommande (mauvaise adresse NEC) : ignoré.
  - Code de répétition (`0xFFFFFFFF`, touche maintenue) : ignoré.
  - Code inconnu : ignoré, avec un `LOG`.
- **Postconditions :** la commande est marquée comme lue ; `commande_disponible()` redevient
  `false` jusqu'à la prochaine carte.

| Fonction | Quand | Bloquant ? |
|---|---|---|
| `void commande_init()` | `etats_init()`, donc **après** `BoardInit()` (qui démarre déjà le récepteur IR) | non |
| `bool commande_disponible()` | chaque tour, en `ATTENTE_COMMANDE` | **non** |
| `Commande commande_lire()` | seulement après `commande_disponible() == true` | non |

**Format de la `Commande` (important pour `arret_robot`) :** `liste[]` est dans l'**ordre
de prise** : `liste[0]` = pain du dessous, `liste[nb - 1]` = pain du dessus. C'est l'inverse
de l'ordre écrit sur la carte (`extraire_ingredients()` fait déjà cette inversion).
Utiliser les valeurs `ING_...` de `types.h` (pas des `#define PAIN 3`).

**Stub attendu :** une nouvelle commande environ 2 s après la précédente, en alternant
quelques recettes, dont une qui oblige à reculer (pour la répétition T8.4).
En attendant ce module, `etats.cpp` utilise une commande de démonstration marquée `TEMPORAIRE`.

**Fini quand :** chaque carte lance la bonne commande (T4.4) ; `docs/codes_nec.md` est validé (T4.1).

**À décider :** ce que contient le code NEC (proposition : le numéro du chef, et le robot
garde la table des recettes) ; l'adresse NEC de l'équipe ; si la manette est un 2ᵉ Arduino.

---

## 7. `ingredients`

### CU-6 : Prendre un ingrédient
- **Acteur :** machine à états, état `PRISE_INGREDIENT`.
- **Préconditions :** le robot est **arrêté** sur la station de cet ingrédient.
- **Scénario principal :**
  1. `etats` appelle `ingredients_prendre(ING_FROMAGE)` (par exemple).
  2. Le bras et la spatule font la séquence de prise (fonctions de Xavier, T6.3).
  3. Le bras revient en position de départ.
  4. La fonction retourne `true`.
- **Cas alternatifs :** la prise échoue → retourne `false`, et `etats` passe en `ERREUR`.
- **Postconditions :** l'ingrédient est sur le burger. Si le suivant est à **la même
  station** (ex. 2ᵉ pain), le robot le prend sans repartir : la station peut contenir
  plusieurs ingrédients, le module doit savoir lequel prendre.

| Fonction | Quand | Bloquant ? |
|---|---|---|
| `void ingredients_init()` | `etats_init()` : bras en position de départ | oui (permis) |
| `bool ingredients_prendre(Ingredient ing)` | `PRISE_INGREDIENT` | **oui, permis** (robot arrêté) |

**Stub attendu :** `LOG` de l'ingrédient, attente d'environ 0,8 s, retourne `true`.

**Fini quand :** chaque ingrédient pris 3 fois sur 3.

---

## 8. `del_rgb`

### CU-7 : Montrer l'état du robot
- **Acteur :** machine à états, à chaque tour.
- **Scénario principal :**
  1. `etats` appelle `del_afficher(DEL_EN_ROUTE)` (par exemple) **à chaque tour**.
  2. Si l'état est le même qu'au tour précédent : rien à faire, on sort tout de suite.
  3. Sinon : la DEL change de couleur.
- **Les 5 états** (`EtatDel` dans `types.h`) : `DEL_ATTENTE`, `DEL_EN_ROUTE`, `DEL_PRISE`,
  `DEL_PRET`, `DEL_ERREUR`. Les couleurs (et un clignotement éventuel) sont ton choix : un
  clignotement doit utiliser `millis()`, pas `delay()`.

### CU-8 : Signaler la fin de la commande
- **Acteur :** machine à états, état `COMMANDE_PRETE`.
- **Scénario :** `del_afficher(DEL_PRET)` → le signal est **visible à 2 m** (T7.3).

| Fonction | Quand | Bloquant ? |
|---|---|---|
| `void del_init()` | `etats_init()` | non |
| `void del_afficher(EtatDel e)` | chaque tour, dans tous les états | **non** |

**Stub attendu :** `LOG` de l'état **seulement quand il change**.

**Fini quand :** les 5 états s'affichent ; schéma électrique dans `docs/`.

---

## 9. Résumé : qui appelle quoi, dans quel état

| État (`etats.cpp`) | suiveur_ligne | couleur | arret_robot | commande | ingredients | del_rgb |
|---|---|---|---|---|---|---|
| ATTENTE_COMMANDE | | | | `disponible`, `lire` | | `ATTENTE` |
| DECISION | | | `decider` | | | |
| DEPLACEMENT | `suivre(sens, …)` | `lire` | `nouvelle_station` | | | `EN_ROUTE` |
| ARRET_STATION | `arreter` (en entrant) | | `demarrer`, `termine` | | | |
| PRISE_INGREDIENT | | | | | `prendre` | `PRISE` |
| COMMANDE_PRETE | | | | | | `PRET` |
| ERREUR | `arreter` | | | | | `ERREUR` |

## 10. À décider en équipe
- Capteur IR : détecter que le burger est ramassé (sortie de `COMMANDE_PRETE`).
- Sortie de l'état `ERREUR` : nouvelle commande ? bouton ?
- Ligne perdue : délai maximum en `DEPLACEMENT` ?
- Reculer en suivant la ligne, ou faire un demi-tour ?
- Ordre réel des stations et les 6 couleurs du parcours.
- `LOG_ACTIF` remplace `DEBUG` du guide : `IRremote.h` (LibRobUS) définit déjà `DEBUG 0`.
