#include <Arduino.h>

// Définition des constantes pour l'état du robot----------------------------------------------------
#define ETAT_ARRET 0
#define ETAT_MARCHE 1

// Relier la pin pour le bouton poussoir (Analog)-----------------------------------------------------
#define PIN_BOUTON A0

// Fonction permettant d'arreter/demarrer le robot avec un bouton poussoir
void arreterDemarrerRobot( int etatCourant); 

// Fonction permettant de vérifier l'appui sur un bouton poussoir entre en paramètre
int boutonPoussoir(int boutonPin);

