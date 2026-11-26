// test_stations.cpp - Test de l'arret a chaque station, sans moteurs
// Vrai capteur de couleur + arret_robot. Le robot ne bouge pas : passer
// soi-meme des feuilles ROUGE et BLEUE sous le capteur, avec du blanc ou la
// ligne entre deux, comme sur le parcours.
//   bumper AVANT   : le robot "avance"  (position +1 a chaque station)
//   bumper ARRIERE : le robot "recule"  (position -1 a chaque station)
//   bumper GAUCHE  : remise a zero (position -1, avant la 1re station)

#include <LibRobus.h>

#include "arret_robot.h"
#include "config.h"
#include "couleur.h"


static Sens sens = SENS_AVANT;
static Couleur precedente = NB_COULEURS;  // force le 1er affichage

static void lire_bumpers() {
    if (ROBUS_IsBumper(FRONT) && sens != SENS_AVANT) {
        sens = SENS_AVANT;
        LOG("Sens : AVANT");
    }
    if (ROBUS_IsBumper(REAR) && sens != SENS_ARRIERE) {
        sens = SENS_ARRIERE;
        LOG("Sens : ARRIERE");
    }
    if (ROBUS_IsBumper(LEFT)) {
        arret_init();
        LOG("Remise a zero : position -1");
        delay(300);  // permis : programme de test (anti-rebond du bumper)
    }
}

void setup() {
    BoardInit();
    couleur_init();
    arret_init();
    LOG("test_stations : passer ROUGE / BLEU sous le capteur");
}

void loop() {
    lire_bumpers();
#if STUB_COULEUR
    couleur_stub_sens(sens);
#endif
    Couleur c = couleur_lire();
    if (c != precedente) LOG_VAL("couleur", nom_couleur(c));
    precedente = c;

    if (arret_nouvelle_station(c, sens)) {
        Serial.print(">>> STATION ");
        Serial.print(nom_couleur(c));
        Serial.print("  position ");
        Serial.println(arret_position());
    }
}
