// Commandes_Robot.h - Christopher Plantevin

// les includes (headers et librairies)

#pragma once

// Configurations Primaire (NE PAS MODIFIER) 

#define AVANT 2
#define ARRIERE 3

#define PIN_DEL_TEST 13

// Variables globale

extern bool Etat_Attente;

// (Définir) Les Fonctions

void Initialisation();
void Reprendre();
void Pause();
void Attente_depart();
void Verif_Commandes(); // verifie si une commande (primaire) est appelee