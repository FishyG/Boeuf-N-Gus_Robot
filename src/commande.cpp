#include <stdio.h>
#include "commande.h"
#include "ingredients.h"


static int UID_comparer(const char *a, const char *b) {

    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return 0;
        }
        i++;
    }
    return a[i] == b[i];
}

void trouver_chef(const char *UID_scanne, const struct Chef chefs[],
                  struct Chef *chef_actuel) {

    for (int i = 0; i < 2; i++) {
        if (UID_comparer(chefs[i].tag_1, UID_scanne) ||
            UID_comparer(chefs[i].tag_2, UID_scanne)) {
            *chef_actuel = chefs[i];
            return;
        }
    }
}

void extraire_ingredients(const struct Chef *chef_actuel, int ingredients[8]) {

    for (int i = 0; i < TAILLE_RECETTE; i++) {
        ingredients[i] = chef_actuel->recette[TAILLE_RECETTE - 1 - i];
    }
}