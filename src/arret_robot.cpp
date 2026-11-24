#include "arret_robot.h"

#include <Arduino.h>

#include "config.h"

static const Couleur STATIONS[NB_STATIONS] = STATIONS_EN_ORDRE;
static const int8_t HORS_PARCOURS = -2;

static Couleur derniere_station = COULEUR_AUCUNE;
static int8_t position = POSITION_DEPART;
static unsigned long t_arret = 0;

static Couleur station_de(Ingredient ing) {
    switch (ing) {
        case ING_PAIN:      return STATION_PAIN;
        case ING_PAIN_SOUS: return STATION_PAIN_SOUS;
        case ING_FROMAGE:   return STATION_FROMAGE;
        case ING_VIANDE:    return STATION_VIANDE;
        case ING_TOMATE:    return STATION_TOMATE;
        case ING_OIGNON:    return STATION_OIGNON;
        case ING_CORNICHON: return STATION_CORNICHON;
        case ING_SALADE:    return STATION_SALADE;
        default:            return COULEUR_AUCUNE;
    }
}

static int8_t indice_de(Couleur c) {
    for (int8_t i = 0; i < NB_STATIONS; i++) {
        if (STATIONS[i] == c) return i;
    }
    return HORS_PARCOURS;
}

static ActionStation aller_vers(Couleur cible, ActionStation si_arrive) {
    int8_t i_cible = indice_de(cible);
    if (i_cible == HORS_PARCOURS) return ACTION_ERREUR;
    if (i_cible == position) return si_arrive;
    return i_cible > position ? ACTION_AVANCER : ACTION_RECULER;
}

void arret_init() {
    derniere_station = COULEUR_AUCUNE;
    position = POSITION_DEPART;
    t_arret = 0;
}

bool arret_nouvelle_station(Couleur c) {
    if (c == COULEUR_AUCUNE) {
        derniere_station = COULEUR_AUCUNE;
        return false;
    }
    if (c == derniere_station) return false;
    derniere_station = c;
    int8_t i = indice_de(c);
    if (i != HORS_PARCOURS) position = i;
    return true;
}

void arret_demarrer() {
    t_arret = millis();
}

bool arret_termine() {
    return millis() - t_arret >= DUREE_ARRET;
}

ActionStation arret_decider(Ingredient prochain) {
    if (prochain == ING_AUCUN) return aller_vers(STATION_CLIENT, ACTION_LIVRER);
    return aller_vers(station_de(prochain), ACTION_PRENDRE);
}

int8_t arret_position() {
    return position;
}
