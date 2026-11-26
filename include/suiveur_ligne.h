#pragma once
#include "config.h"
#include "types.h"

// Suiveur de ligne : 3 capteurs analogiques a l'avant (gauche, centre, droite).
// Non bloquant : etats appelle suiveur_suivre() a chaque tour en DEPLACEMENT.

// Arrete les moteurs. Appelee une fois par etats_init().
void suiveur_init();

// Un pas de suivi : lit les 3 capteurs et ajuste les 2 moteurs.
// sens : SENS_AVANT ou SENS_ARRIERE. vitesse : 0.0 a 1.0.
void suiveur_suivre(Sens sens, float vitesse);

// Coupe les deux moteurs tout de suite.
void suiveur_arreter();

// true si aucun capteur n'a vu la ligne depuis DUREE_LIGNE_PERDUE.
bool suiveur_ligne_perdue();
