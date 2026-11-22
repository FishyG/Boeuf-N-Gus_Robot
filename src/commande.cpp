#include "..\lib\commande.h"
#include "..\lib\ingredients.h"

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
static int trouver_taille(const struct Chef *chef_actuel, int taille_max){
	int taille=0;
	for ( int i=0; i< taille_max; i++){
		if (chef_actuel->recette[i] != 0){
			taille++;
		}
	}
	return taille;
}
void trouver_chef(const char *UID_scanne, const struct Chef chefs[],
                  struct Chef *chef_actuel) {

    for (int i = 0; i < NOMBRE_CHEFS; i++) {
        if (UID_comparer(chefs[i].tag_1, UID_scanne) ||
            UID_comparer(chefs[i].tag_2, UID_scanne)) {
            *chef_actuel = chefs[i];
            return;
        }
    }
}

void extraire_ingredients(const struct Chef *chef_actuel, int ingredients[8]) {
    int taille_actuelle = trouver_taille(chef_actuel, INGREDIENTS_MAX);
    for (int i = 0; i < taille_actuelle; i++) {
        ingredients[i] = chef_actuel->recette[taille_actuelle - 1 - i];
    }
}