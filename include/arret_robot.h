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

// Recoit la couleur lue et le sens ou roule le robot. Retourne true UNE fois
// en arrivant sur une station, et compte la position : +1 en avancant,
// -1 en reculant. Si la couleur n'est pas celle attendue (alternance
// ROUGE/BLEU), une station a ete manquee et la position est corrigee.
bool arret_nouvelle_station(Couleur c, Sens sens);

void arret_demarrer();

bool arret_termine();

// ING_AUCUN = tout est pris : aller livrer a POS_CLIENT.
ActionStation arret_decider(Ingredient prochain);

int8_t arret_position();

// Nom de l'action, pour les logs.
const char* arret_nom_action(ActionStation a);
