#pragma once
#include "types.h"

// =============================================================================
// Stubs : 1 = fausse version, 0 = vrai materiel
// =============================================================================
// Valeurs pour [env:complet]. Quand ton vrai module marche, passe ton
// drapeau a 0 dans ta pull request. Les environnements de platformio.ini
// peuvent les forcer (d'ou les #ifndef).
// Chaque responsable ajoute ICI le drapeau de son module, sur ce modele
// (noms attendus : STUB_SUIVEUR, STUB_COMMANDE, STUB_INGREDIENTS, STUB_DEL).
#ifndef STUB_COULEUR
#define STUB_COULEUR 1
#endif

// =============================================================================
// Parcours (A CONFIRMER avec le vrai parcours)
// =============================================================================
// Ordre des stations le long de la ligne, dans le sens ou le robot avance.
// Le parcours est une boucle (3 tournants, geres par suiveur_ligne).
// Chaque couleur UNE seule fois : la couleur lue dit OU est le robot.
// 6 couleurs : plusieurs ingredients partagent une station (ex. les 2 pains).
//
//   [ROUGE]  [ORANGE]  [BLEU]     [VERT]      [JAUNE]   [MAUVE]
//   pains    fromage   boulette   salade      tomate    client
//                                 cornichon   oignon
#define NB_STATIONS        6
#define STATIONS_EN_ORDRE  {COULEUR_ROUGE, COULEUR_ORANGE, COULEUR_BLEU, \
                            COULEUR_VERT, COULEUR_JAUNE, COULEUR_MAUVE}
#define POSITION_DEPART    -1  // -1 = le robot part AVANT la 1re station

// Station (couleur) de chaque ingredient
#define STATION_PAIN       COULEUR_ROUGE
#define STATION_PAIN_SOUS  COULEUR_ROUGE
#define STATION_FROMAGE    COULEUR_ORANGE
#define STATION_VIANDE     COULEUR_BLEU
#define STATION_SALADE     COULEUR_VERT
#define STATION_CORNICHON  COULEUR_VERT
#define STATION_TOMATE     COULEUR_JAUNE
#define STATION_OIGNON     COULEUR_JAUNE
#define STATION_CLIENT     COULEUR_MAUVE

// =============================================================================
// Broches (A AJUSTER selon le cablage)
// =============================================================================
// LibRobus utilise deja : IR 37, Serial2 (Bluetooth), Serial3 (audio).
// Chaque responsable ajoute ICI les broches de son module (PIN_xxx).
// Capteur de couleur TCS34725 : I2C (SDA 20, SCL 21), pas de broche a choisir.

// =============================================================================
// Reglages
// =============================================================================
#define VITESSE_BASE        0.3f
#define VITESSE_RECUL       0.2f
#define DUREE_ARRET         500UL
#define DUREE_AFFICHAGE_PRET 3000UL


#define SEUIL_HAUT          600    
#define SEUIL_BAS           200    
#define LECTURES_ANTIREBOND 3

// =============================================================================
// Reglages des stubs
// =============================================================================
#define STUB_DUREE_SEGMENT   1500UL
#define STUB_LARGEUR_STATION 300UL
#define DELAI_COMMANDE_DEMO  2000UL

#ifndef LOG_ACTIF
#define LOG_ACTIF 1
#endif

#if LOG_ACTIF

#define LOG(x) Serial.println(x)

#define LOG_VAL(nom, val)    \
    do {                     \
        Serial.print(nom);   \
        Serial.print(" = "); \
        Serial.println(val); \
    } while (0)
#else
#define LOG(x) \
    do {       \
    } while (0)
#define LOG_VAL(nom, val) \
    do {                  \
    } while (0)
#endif
