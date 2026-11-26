#include "suiveur_ligne.h"

#include <LibRobus.h>

#if STUB_SUIVEUR
// Fausse version : pas de moteurs. Le stub du capteur de couleur "roule".
void suiveur_init() {}
void suiveur_suivre(Sens sens, float vitesse) {}
void suiveur_arreter() {}
bool suiveur_ligne_perdue() { return false; }

#else
// =============================================================================
// 1. Ecart a la ligne : (gauche - droite) / (gauche + centre + droite),
//    entre -1 et 1. > 0 : la ligne est a gauche.
// 2. Correction proportionnelle : une roue ralentit, l'autre accelere.
//    Meme formule en marche arriere : seule la vitesse de base change de signe.
// 3. Ligne perdue : tout droit pendant DELAI_RECHERCHE_LIGNE (un ruban de
//    couleur cache la ligne), puis tourner du cote ou on l'a vue en dernier
//    (idee de Jessy).
// 4. Marche arriere : en ligne droite. Les encodeurs gardent le cap, la ligne
//    corrige dans l'AUTRE sens qu'en avant : les capteurs sont a l'arriere du
//    mouvement, tourner le nez vers la ligne ferait deriver le robot.
// =============================================================================
static float derniere_correction = 0;  // > 0 : tournait a gauche
static unsigned long t_ligne_vue = 0;

float vitesse_pi;
float acceleration_pi;


static bool en_route = false;          // false : arrete, le chrono "perdue" repart

static void moteurs(float gauche, float droite) {
    MOTOR_SetSpeed(LEFT, constrain(gauche, -1.0f, 1.0f));
    MOTOR_SetSpeed(RIGHT, constrain(droite, -1.0f, 1.0f));
}

void suiveur_init() {
    suiveur_arreter();
}

void suiveur_suivre(Sens sens, float vitesse) {
    if (!en_route) {
        en_route = true;
        t_ligne_vue = millis();
        ENCODER_Reset(LEFT);
        ENCODER_Reset(RIGHT);
    }
    int g = analogRead(PIN_LIGNE_GAUCHE);
    int c = analogRead(PIN_LIGNE_CENTRE);
    int d = analogRead(PIN_LIGNE_DROITE);

    bool vue = g > SEUIL_LIGNE || c > SEUIL_LIGNE || d > SEUIL_LIGNE;
    float ecart = vue ? (float)(g - d) / (g + c + d) : 0;
    if (vue) t_ligne_vue = millis();

    float correction;
    if (sens == SENS_ARRIERE) {
        // Roue gauche en avance (plus negative) : ecart_roues < 0, elle ralentit.
        long ecart_roues = ENCODER_Read(LEFT) - ENCODER_Read(RIGHT);
        correction = GAIN_RECUL_DROIT * ecart_roues - GAIN_RECUL_LIGNE * ecart * vitesse;
    } else if (vue) {
        correction = GAIN_SUIVEUR * ecart * vitesse;
        derniere_correction = correction;
    } else if (millis() - t_ligne_vue < DELAI_RECHERCHE_LIGNE) {
        correction = 0;  // sur un ruban : tout droit
    } else {
        correction = (derniere_correction >= 0) ? vitesse : -vitesse;
    }

    float base = (sens == SENS_AVANT) ? vitesse : -vitesse;
    // correction > 0 : le nez du robot va a gauche, en avant comme en arriere.
    moteurs(base - correction, base + correction);

#if LOG_DEBUG
    static unsigned long t = 0;
    if (millis() - t >= PERIODE_LOG_DEBUG) {
        t = millis();
        Serial.print("[suiveur] g ");
        Serial.print(g);
        Serial.print(" c ");
        Serial.print(c);
        Serial.print(" d ");
        Serial.print(d);
        Serial.print(vue ? "  ligne" : "  PERDUE");
        if (sens == SENS_ARRIERE) {
            Serial.print("  recul enc G ");
            Serial.print(ENCODER_Read(LEFT));
            Serial.print(" D ");
            Serial.print(ENCODER_Read(RIGHT));
        }
        Serial.print("  moteurs G ");
        Serial.print(base - correction);
        Serial.print(" D ");
        Serial.println(base + correction);
    }
#endif
}

void suiveur_arreter(){

    int temps;

    vitesse_pi = VITESSE_BASE;
    
    moteurs(vitesse_pi, vitesse_pi);

    acceleration_pi = vitesse_pi / TEMPS_ARRET;

    temps = TEMPS_ARRET;

    while(temps != 0){
        
        delay(TEMPS_INTERVALLE / 1000)
        vitesse_pi = vitesse_pi - acceleration_pi;

        moteurs(vitesse_pi, vitesse_pi);

        temps = temps - TEMPS_INTERVALLE;

    }

    moteurs(0, 0);

    en_route = false;  // le temps a l'arret ne compte pas comme "perdue"
}

bool suiveur_ligne_perdue() {
    return en_route && millis() - t_ligne_vue > DUREE_LIGNE_PERDUE;
}
#endif
