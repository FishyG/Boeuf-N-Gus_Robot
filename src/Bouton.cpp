#include "..\lib\Bouton.h"

// Fonction permettant d'arreter le robot avec un bouton poussoir
void arreterDemarrerRobot( int etatCourant) {
  // Vérifier si le bouton est appuyé
  if (boutonPoussoir(PIN_BOUTON)) {
    switch (etatCourant) {
      case ETAT_ARRET:
        // Allumer le robot
        etatCourant = ETAT_MARCHE;
        break;
      case ETAT_MARCHE:
        // Arreter le robot
        etatCourant = ETAT_ARRET;
        break;
    }
  }
}

// Fonction permettant de vérifier l'appui sur le bouton poussoir
int boutonPoussoir(int boutonPin) {
  // Vérifier si le bouton est appuyé
  if (digitalRead(boutonPin) == HIGH) {
    // Attendre que le bouton soit relâché pour éviter les rebonds
    while (digitalRead(boutonPin) == HIGH) {
      delay(10);
    }
    return 1; // Le bouton a été appuyé
  }
  return 0; // Le bouton n'a pas été appuyé
}

