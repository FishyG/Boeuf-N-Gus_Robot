// test_couleur.cpp - Programme de test du capteur de couleur
// Affiche, toutes les NB_MESURES mesures, la moyenne des valeurs brutes, les
// valeurs divisees par la reference et la couleur reconnue.
// Demarrer le robot SUR LE PLANCHER : c'est la reference (1.0 / 1.0 / 1.0).
//   - Sur une station ROUGE : rouge/bleu >= RAPPORT_MIN (config.h).
//   - Sur une station BLEUE : rouge/bleu <= 1 / RAPPORT_MIN.
//   - Sur la ligne et le sol : "reconnue : AUCUNE".

#include <LibRobus.h>

#include "config.h"
#include "couleur.h"

static const uint8_t NB_MESURES = 30;

void setup() {
    BoardInit();
    couleur_init();
}

#if STUB_COULEUR
// Le faux capteur parcourt la ligne : on affiche chaque changement.
void loop() {
    static Couleur precedente = NB_COULEURS;
    couleur_stub_sens(SENS_AVANT);
    Couleur c = couleur_lire();
    if (c != precedente) LOG_VAL("couleur", nom_couleur(c));
    precedente = c;
}

#else
void loop() {
    float r = 0, g = 0, b = 0, rouge = 0, vert = 0, bleu = 0;
    for (uint8_t i = 0; i < NB_MESURES; i++) {
        MesureCouleur m;
        if (!couleur_mesurer(m)) {
            LOG("Capteur muet : verifier le branchement I2C");
            delay(1000);
            return;
        }
        r += m.r;
        g += m.g;
        b += m.b;
        rouge += m.rouge;
        vert += m.vert;
        bleu += m.bleu;
    }
    Serial.print("brut R ");
    Serial.print(r / NB_MESURES, 0);
    Serial.print(" G ");
    Serial.print(g / NB_MESURES, 0);
    Serial.print(" B ");
    Serial.print(b / NB_MESURES, 0);
    Serial.print("  | / plancher : rouge ");
    Serial.print(rouge / NB_MESURES);
    Serial.print(" vert ");
    Serial.print(vert / NB_MESURES);
    Serial.print(" bleu ");
    Serial.print(bleu / NB_MESURES);
    Serial.print("  | rouge/bleu ");
    Serial.print(rouge / bleu);
    Serial.print("  | reconnue : ");
    Serial.println(nom_couleur(couleur_lire()));
}
#endif
