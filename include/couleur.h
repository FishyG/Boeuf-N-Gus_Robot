#pragma once
#include "config.h"
#include "types.h"

// Une mesure du capteur, pour le diagnostic (test_couleur).
struct MesureCouleur {
    uint16_t r, g, b;           // valeurs brutes du TCS34725
    float rouge, vert, bleu;    // divisees par la reference : 1.0 = plancher
};

// Demarre le capteur et mesure le plancher sous le robot comme reference.
// Appelee une fois par etats_init(), le robot pose sur le plancher.
void couleur_init();

// Lit en continu la couleur sous le robot, filtree (anti-rebond) :
// COULEUR_ROUGE, COULEUR_BLEU, ou COULEUR_AUCUNE hors station.
// Prend environ 24 ms (temps d'integration du capteur).
Couleur couleur_lire();

// Une mesure brute, sans classement ni filtre. Pour le diagnostic.
// Retourne false si le capteur ne repond pas (ou en stub).
bool couleur_mesurer(MesureCouleur& m);

#if STUB_COULEUR
// STUB SEULEMENT : dit au faux capteur dans quel sens le robot roule.
void couleur_stub_sens(Sens sens);
#endif
