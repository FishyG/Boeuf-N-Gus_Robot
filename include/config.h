#pragma once
#include "types.h"

// =============================================================================
// Stubs : 1 = fausse version, 0 = vrai materiel
// =============================================================================
// Chaque responsable ajoute ICI le drapeau de son module (STUB_COMMANDE,
// STUB_INGREDIENTS, STUB_DEL). platformio.ini peut les forcer (d'ou les #ifndef).
#ifndef STUB_COULEUR
#define STUB_COULEUR 0 
#endif
#ifndef STUB_SUIVEUR
#define STUB_SUIVEUR 0
#endif

// =============================================================================
// Parcours (A CONFIRMER avec le vrai parcours)
// =============================================================================
// Une station par ingredient. Seulement 2 couleurs, en alternance : la
// couleur ne dit pas OU est le robot, arret_robot COMPTE les stations.
// L'alternance sert de verification (meme couleur 2 fois = station manquee).
//
//   position :  0      1       2        3       4       5       6          7
//   couleur  :  ROUGE  BLEU    ROUGE    BLEU    ROUGE   BLEU    ROUGE      BLEU
//   station  :  pains  viande  fromage  salade  tomate  oignon  cornichon  client
#define NB_STATIONS        8
#define POSITION_DEPART    -1  // -1 = le robot part AVANT la 1re station

#define COULEUR_STATION_PAIRE    COULEUR_ROUGE  // positions 0, 2, 4, 6
#define COULEUR_STATION_IMPAIRE  COULEUR_BLEU   // positions 1, 3, 5, 7
#define COULEUR_STATION(pos)     ((pos) % 2 == 0 ? COULEUR_STATION_PAIRE : COULEUR_STATION_IMPAIRE)

#define POS_PAIN       0
#define POS_VIANDE     1
#define POS_FROMAGE    2
#define POS_SALADE     3
#define POS_TOMATE     4
#define POS_OIGNON     5
#define POS_CORNICHON  6
#define POS_CLIENT     7

// =============================================================================
// Broches (A AJUSTER selon le cablage)
// =============================================================================
// LibRobus utilise deja : IR 37, Serial2 (Bluetooth), Serial3 (audio).
// Chaque responsable ajoute ICI les broches de son module (PIN_xxx).
// Capteur de couleur TCS34725 : I2C (SDA 20, SCL 21), pas de broche a choisir.
// Suiveur de ligne : 3 capteurs analogiques, vus de dessus robot vers l'avant.
#define PIN_LIGNE_GAUCHE  A1
#define PIN_LIGNE_CENTRE  A2
#define PIN_LIGNE_DROITE  A3

// =============================================================================
// Reglages
// =============================================================================
#define VITESSE_BASE        0.2f
#define VITESSE_RECUL       0.15f
#ifndef DUREE_ARRET
#define DUREE_ARRET         500UL   // ms d'arret a chaque station
#endif
#define DUREE_AFFICHAGE_PRET 3000UL

// Bips (TEMPORAIRE : en attendant les DEL de Christian)
#define BIP_PRISE_FREQ       1000  // Hz, bonne station
#define BIP_PRISE_DUREE      300   // ms
#define BIP_LIVRE_FREQ       1500  // Hz, burger livre au client
#define BIP_LIVRE_DUREE      1000  // ms

// Suiveur de ligne
#define SEUIL_LIGNE          400     // analogRead > 400 : le capteur voit la ligne
#define GAIN_SUIVEUR         1.0f    // force de la correction (plus haut = tourne plus fort)
#define DUREE_LIGNE_PERDUE   2000UL  // ms sans voir la ligne : ERREUR
#define DELAI_RECHERCHE_LIGNE 500UL  // ms tout droit avant de chercher (ruban sur la ligne)
// Marche arriere en ligne droite
#define GAIN_RECUL_DROIT     0.0005f // encodeurs : garde le cap du depart
#define GAIN_RECUL_LIGNE     1.0f    // ramene vers la ligne (0 = encodeurs seulement)

#define TEMPS_ARRET 0.5f // temps d'arret
#define TEMPS_INTERVALLE 0.01f // temps d'intervalle

// =============================================================================
// Capteur de couleur TCS34725
// =============================================================================
#define COULEUR_INTEGRATION   TCS34725_INTEGRATIONTIME_24MS  // une lecture = ~24 ms
#define COULEUR_GAIN          TCS34725_GAIN_60X

// Reference mesuree au demarrage par couleur_init() : la surface sous le robot
// (le plancher, avant la 1re station). Rien a calibrer a la main.
#define NB_MESURES_REFERENCE  10

// Valeurs divisees par la reference : 1.0 = autant que le plancher.
#define RAPPORT_MIN     1.10f  // ROUGE si rouge/bleu >= 1.10, BLEU si bleu/rouge >= 1.10
#define LUMINOSITE_MIN  0.10f  // plus sombre que 10 % du plancher : ligne noire ou rien
#define LECTURES_ANTIREBOND   3     // lectures identiques de suite avant de changer de couleur

// =============================================================================
// Reglages des stubs
// =============================================================================
#define STUB_DUREE_SEGMENT   1500UL
#define STUB_LARGEUR_STATION 300UL

// =============================================================================
// Demo (TEMPORAIRE : en attendant les modules commande et ingredients de Lyevan)
// =============================================================================
// Recette de Patrick, dans l'ordre de prise (du bas vers le haut).
// 2 boulettes et 2 tomates : le robot doit revenir en arriere.
#define RECETTE_DEMO \
    {ING_PAIN_SOUS, ING_TOMATE, ING_VIANDE, ING_TOMATE, ING_VIANDE, ING_FROMAGE, ING_PAIN}
#define NB_RECETTE_DEMO      7
#define DELAI_COMMANDE_DEMO  2000UL   // ms avant de lancer la demo
#ifndef DUREE_PRISE_DEMO
#define DUREE_PRISE_DEMO     3000UL   // ms sur une bonne station
#endif

#ifndef LOG_ACTIF
#define LOG_ACTIF 1
#endif

// Logs detailles (env complet_debug) : capteurs, moteurs et decisions, 5 fois/s.
#ifndef LOG_DEBUG
#define LOG_DEBUG 0
#endif
#define PERIODE_LOG_DEBUG  200UL
#define BAUD_DEBUG         115200  // 9600 est trop lent : les logs ralentiraient le robot

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
