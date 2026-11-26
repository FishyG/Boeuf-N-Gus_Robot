// deplacement_robot.h - Christopher Plantevin
#pragma once

//#include "config.h"
//#include "types.h"

#define SEUIL_LIGNE 400
#define SEUIL_DETECTION_D 400
#define SEUIL_DETECTION_G 400

#define PROPORTION_DETECTION 100

#define PIN_A A1
#define PIN_B A2
#define PIN_C A3

#define VITESSE_CORRECTION_DROITE 0.2
#define VITESSE_CORRECTION_GAUCHE 0.2
#define VITESSE_AVANT 0.2
#define VITESSE_ARRIERE 0.2


extern bool Etat_Attente;

// (Définir) Les Fonctions

void SuiveurLigne();
void Avancer(int distance);
void Reculer(int distance);
void Tourner(int angle);
void Arret();