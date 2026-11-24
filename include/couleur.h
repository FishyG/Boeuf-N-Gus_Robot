#pragma once
#include "config.h"
#include "types.h"


void couleur_init();

Couleur couleur_lire();

#if STUB_COULEUR
// STUB SEULEMENT (n'existe pas dans la vraie version) : dit au faux capteur
// dans quel sens le robot roule, pour qu'il simule le parcours dans le bon
// sens. Appeler a chaque tour pendant que le robot roule.
void couleur_stub_sens(Sens sens);
#endif
