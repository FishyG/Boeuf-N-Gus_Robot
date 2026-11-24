// Deplacement_Robot.h - Christopher Plantevin

// les includes (headers et librairies)

#pragma once
#include "types.h"

#define SEUIL_LIGNE 400
#define SEUIL_DETECTION_D 400
#define SEUIL_DETECTION_G 400

// (Définir) Les Fonctions

void SuiveurLigne();
void Avancer(int distance);
void Reculer(int distance);
void Tourner(int angle);



