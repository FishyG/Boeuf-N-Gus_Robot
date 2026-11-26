#include "arret_robot.h"

#include <Arduino.h>

#include "config.h"

static const int8_t HORS_PARCOURS = -2;

static Couleur derniere_couleur = COULEUR_AUCUNE;
static int8_t position = POSITION_DEPART;
static unsigned long t_arret = 0;

static int8_t position_de(Ingredient ing) {
    switch (ing) {
        case ING_PAIN:
        case ING_PAIN_SOUS: return POS_PAIN;
        case ING_VIANDE:    return POS_VIANDE;
        case ING_FROMAGE:   return POS_FROMAGE;
        case ING_SALADE:    return POS_SALADE;
        case ING_TOMATE:    return POS_TOMATE;
        case ING_OIGNON:    return POS_OIGNON;
        case ING_CORNICHON: return POS_CORNICHON;
        default:            return HORS_PARCOURS;
    }
}

static ActionStation aller_vers(int8_t cible, ActionStation si_arrive) {
    if (cible == HORS_PARCOURS) return ACTION_ERREUR;
    if (cible == position) return si_arrive;
    return cible > position ? ACTION_AVANCER : ACTION_RECULER;
}

void arret_init() {
    derniere_couleur = COULEUR_AUCUNE;
    position = POSITION_DEPART;
    t_arret = 0;
}

bool arret_nouvelle_station(Couleur c, Sens sens) {
    if (c == COULEUR_AUCUNE) { 
        derniere_couleur = COULEUR_AUCUNE;
        return false;
    }
    if (c == derniere_couleur) return false;
    derniere_couleur = c;

    int8_t pas = (sens == SENS_AVANT) ? 1 : -1;
    position += pas;
    if (c != COULEUR_STATION(position)) {
        position += pas;
        LOG("Station manquee : position corrigee");
    }
    position = constrain(position, 0, NB_STATIONS - 1);
    return true;
}

void arret_demarrer() {
    t_arret = millis();
}

bool arret_termine() {
    return millis() - t_arret >= DUREE_ARRET;
}

ActionStation arret_decider(Ingredient prochain) {
    if (prochain == ING_AUCUN) return aller_vers(POS_CLIENT, ACTION_LIVRER);
    return aller_vers(position_de(prochain), ACTION_PRENDRE);
}

int8_t arret_position() {
    return position;
}
