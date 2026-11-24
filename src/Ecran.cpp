#include "..\lib\Ecran.h"


// Fonction du setup de l'écran LCD
void setupLCD(LiquidCrystal &ecran, unsigned long &previousMillis, int &etape) {
  // Définir la taille de l'écran
  ecran.begin(16, 4);
  // Set les variables pour les clignements
  etape = 0;
  previousMillis = millis();
  // Automatiquement ouvert
  ecrireEcran(OEIL_OUVERT, 0, ecran);

}


// Fonction pour déterminer la taille du message à afficher
int tailleMsg(String msg) {
  int taille = 0;
  while (msg[taille] != '\0') {
    taille++;
  }
  return taille;  // Retourne la taille
}

//"    .------.       !   oo   !      !   oo   !       '------'    "


// Fonction permettant d'écrire sur l'écran LCD
void ecrireEcran(String msg, int ligne, LiquidCrystal &ecran) {
  // Nettoyer l'écran
  ecran.clear();
  int colonne = 0;
  // Écrire un caratère à la fois
  for (int i = 0; i < tailleMsg(msg); i++) {
    if (msg[i] == '\n') {
      ligne++;
      if (ligne > 3) {
        break;
      }
      colonne = 0;
      ecran.setCursor(colonne, ligne);
    } else {
      ecran.setCursor(colonne, ligne);
      ecran.print(msg[i]);
      colonne++;
    }
  }
}


// Fonction pour remplacer la fonction delay()
int intervalMillis(int interval, unsigned long &previousMillis) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    return 1; // L'intervalle de temps est écoulé
  }
  return 0; // L'intervalle de temps n'est pas encore écoulé
}


// Fonction qui cligne - il faut prevoir une place dans la mémoire pour un int etape et 
// un int previousMillis, leur valeur est initialisée dans la fonction setup
void oeilCligner(LiquidCrystal &ecran, int &etape, unsigned long &previousMillis ) {

  switch (etape) {

    case 0: 
      if (intervalMillis(3000, previousMillis)) {
      ecrireEcran(OEIL_MI_OUVERT, 0, ecran);
      etape++;
      }
      break;
    case 1:
      if(intervalMillis(200, previousMillis)) {
        ecrireEcran(OEIL_FERME, 0, ecran);
        etape++;
      }
      break;
    case 2:
      if(intervalMillis(500, previousMillis)) {
        ecrireEcran(OEIL_MI_OUVERT, 0, ecran);
        etape++;
      }
      break;
    case 3:
    if(intervalMillis(200, previousMillis)) {
        ecrireEcran(OEIL_OUVERT, 0, ecran);
        etape = 0; // Réinitialiser l'étape pour recommencer le clignement
      }
      break;
  }

}

void fermerOeil(LiquidCrystal &ecran) {
  ecrireEcran(OEIL_MI_OUVERT, 0, ecran);
  delay(200);
  ecrireEcran(OEIL_FERME, 0, ecran);
}

void ouvrirOeil(LiquidCrystal &ecran) {
  ecrireEcran(OEIL_MI_OUVERT, 0, ecran);
  delay(200);
  ecrireEcran(OEIL_OUVERT, 0, ecran);
}