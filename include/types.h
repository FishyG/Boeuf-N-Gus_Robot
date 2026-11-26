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

// Noms pour les logs, a garder dans le meme ordre que les enums.
inline const char* nom_couleur(Couleur c) {
    static const char* const NOMS[] = {"AUCUNE", "ROUGE", "BLEU"};
    return c < NB_COULEURS ? NOMS[c] : "?";
}

inline const char* nom_ingredient(Ingredient i) {
    static const char* const NOMS[] = {"AUCUN",  "PAIN",   "PAIN_SOUS", "FROMAGE", "VIANDE",
                                       "TOMATE", "OIGNON", "CORNICHON", "SALADE"};
    return i < NB_INGREDIENTS ? NOMS[i] : "?";
}
