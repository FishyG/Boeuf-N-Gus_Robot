#pragma once
#include "types.h"


enum ActionStation : uint8_t {
    ACTION_PRENDRE,
    ACTION_LIVRER,
    ACTION_AVANCER,
    ACTION_RECULER,
    ACTION_ERREUR
};


void arret_init();


bool arret_nouvelle_station(Couleur c);

void arret_demarrer();

bool arret_termine();

ActionStation arret_decider(Ingredient prochain);

int8_t arret_position();
