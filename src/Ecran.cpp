#include "..\lib\Ecran.h"


// Fonction du setup de l'écran LCD
void setupLCD(LiquidCrystal &ecran) {
  // Définir la taille de l'écran
  ecran.begin(16, 4);
  ecrireEcran(OEIL_OUVERT, 0, ecran);

}

int tailleMsg(String msg) {
  int taille = 0;
  while (msg[taille] != '\0') {
    taille++;
  }
  return taille;
}

//"    .------.       !   oo   !      !   oo   !       '------'    "

// Fonction permettant d'écrire sur l'écran LCD
void ecrireEcran(String msg, int ligne, LiquidCrystal &ecran) {
  ecran.clear();
  int colonne = 0;
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

void oeilCligner(LiquidCrystal &ecran) {
  unsigned long currentMillis = millis();

  ecrireEcran(OEIL_OUVERT, 0, ecran);

  ecrireEcran(OEIL_MI_OUVERT, 0, ecran);
  delay(200);

  ecrireEcran(OEIL_FERME, 0, ecran);
  delay(500);

  ecrireEcran(OEIL_MI_OUVERT, 0, ecran);
  delay(200);

  ecrireEcran(OEIL_OUVERT, 0, ecran);
  delay(3000);
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