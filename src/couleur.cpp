#include "couleur.h"

#include <Arduino.h>

#if STUB_COULEUR
// =============================================================================
// Fausse version (stub) : simule la ligne de config.h
//
//   depart --(SEGMENT)-- [station 0] --(SEGMENT)-- [station 1] -- ...
//
// La position "x" du robot est mesuree en ms de roulage. Elle n'avance que
// pendant que couleur_lire() est appelee sans pause, comme un vrai robot qui
// ne roule que dans l'etat DEPLACEMENT, et dans le sens donne par
// couleur_stub_sens().
// =============================================================================
static const Couleur STATIONS[NB_STATIONS] = STATIONS_EN_ORDRE;
static const unsigned long PAUSE_MAX = 50;  // ms : au-dela, le robot etait arrete
static const long X_MAX = (long)(NB_STATIONS + 1) * STUB_DUREE_SEGMENT;

static long x = 0;                   // position simulee du robot
static Sens sens_stub = SENS_AVANT;
static unsigned long t_dernier = 0;  // dernier appel de couleur_lire()

static long centre_station(int8_t i) {
    return (long)(i + 1) * STUB_DUREE_SEGMENT;
}

static Couleur couleur_en(long position) {
    for (int8_t i = 0; i < NB_STATIONS; i++) {
        if (labs(position - centre_station(i)) <= (long)STUB_LARGEUR_STATION / 2) return STATIONS[i];
    }
    return COULEUR_AUCUNE;
}

void couleur_init() {
    x = centre_station(POSITION_DEPART);  // POSITION_DEPART = -1 : x = 0
    sens_stub = SENS_AVANT;
    t_dernier = millis();
}

void couleur_stub_sens(Sens sens) {
    sens_stub = sens;
}

Couleur couleur_lire() {
    unsigned long maintenant = millis();
    long dt = maintenant - t_dernier;
    t_dernier = maintenant;
    if (dt < (long)PAUSE_MAX) x += (sens_stub == SENS_AVANT) ? dt : -dt;
    x = constrain(x, 0L, X_MAX);  // le robot ne sort pas de la ligne
    return couleur_en(x);
}

#else
// =============================================================================
// Vraie version : TCS34725 (T3.2 brancher les fonctions de Jessy,
// T3.3 classement, anti-rebond, calibration)
// =============================================================================
#error "couleur.cpp : vraie version pas encore ecrite (T3.2). Utiliser STUB_COULEUR=1."
#endif
