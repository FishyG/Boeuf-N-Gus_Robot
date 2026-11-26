// test_suiveur.cpp - Test du suiveur de ligne seul (sans couleurs ni stations)
//   bumper AVANT   : suivre la ligne en avant
//   bumper ARRIERE : suivre la ligne en marche arriere
//   bumper GAUCHE  : arreter
//
// Affiche les 3 capteurs 4 fois par seconde : sur la ligne > SEUIL_LIGNE,
// a cote < SEUIL_LIGNE. Ajuster SEUIL_LIGNE et GAIN_SUIVEUR dans config.h.

#include <LibRobus.h>

#include "config.h"
#include "suiveur_ligne.h"

static bool en_route = false;
static Sens sens = SENS_AVANT;

static void lire_bumpers() {
    if (ROBUS_IsBumper(FRONT)) {
        sens = SENS_AVANT;
        en_route = true;
        LOG("AVANT");
    }
    if (ROBUS_IsBumper(REAR)) {
        sens = SENS_ARRIERE;
        en_route = true;
        LOG("ARRIERE");
    }
    if (ROBUS_IsBumper(LEFT)) {
        en_route = false;
        suiveur_arreter();
        LOG("ARRET");
    }
}

static void afficher_capteurs() {
    static unsigned long t = 0;
    if (millis() - t < 250) return;
    t = millis();
    Serial.print("gauche ");
    Serial.print(analogRead(PIN_LIGNE_GAUCHE));
    Serial.print("  centre ");
    Serial.print(analogRead(PIN_LIGNE_CENTRE));
    Serial.print("  droite ");
    Serial.println(analogRead(PIN_LIGNE_DROITE));
}

void setup() {
    BoardInit();
    suiveur_init();
    LOG("test_suiveur : bumper AVANT / ARRIERE pour partir, GAUCHE pour arreter");
}

void loop() {
    lire_bumpers();
    afficher_capteurs();
    if (!en_route) return;
    suiveur_suivre(sens, sens == SENS_AVANT ? VITESSE_BASE : VITESSE_RECUL);
    if (suiveur_ligne_perdue()) {
        en_route = false;
        suiveur_arreter();
        LOG("Ligne perdue : arret");
    }
    delay(20);  // permis : programme de test (le temps d'une lecture de couleur)
}
