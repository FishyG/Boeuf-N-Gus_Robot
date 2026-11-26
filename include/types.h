#pragma once
#include <Arduino.h>

enum Ingredient : uint8_t {
    ING_AUCUN = 0,
    ING_PAIN,
    ING_PAIN_SOUS, 
    ING_FROMAGE,
    ING_VIANDE,
    ING_TOMATE,
    ING_OIGNON,
    ING_CORNICHON,
    ING_SALADE,
    NB_INGREDIENTS
};


enum Couleur : uint8_t {
    COULEUR_AUCUNE = 0,
    COULEUR_ROUGE,
    COULEUR_BLEU,
    NB_COULEURS
};


enum Sens : uint8_t {
    SENS_AVANT,
    SENS_ARRIERE
};

#define MAX_INGREDIENTS 8

struct Commande {
    Ingredient liste[MAX_INGREDIENTS];
    uint8_t nb;
};


enum EtatDel : uint8_t {
    DEL_ATTENTE,
    DEL_EN_ROUTE,
    DEL_PRISE,
    DEL_PRET,
    DEL_ERREUR
};
