#include "couleur.h"

#include <Arduino.h>

#if STUB_COULEUR
// =============================================================================
// Fausse version (stub) : simule le parcours de config.h
//
//   depart --(SEGMENT)-- [station 0] --(SEGMENT)-- [station 1] -- ...
//
// La position "x" du robot est mesuree en ms de roulage. Elle n'avance que
// pendant que couleur_lire() est appelee sans pause (le robot roule), dans le
// sens donne par couleur_stub_sens().
// =============================================================================
static const unsigned long PAUSE_MAX = 50;  // ms : au-dela, le robot etait arrete
static const long X_MAX = (long)(NB_STATIONS + 1) * STUB_DUREE_SEGMENT;

static long x = 0;
static Sens sens_stub = SENS_AVANT;
static unsigned long t_dernier = 0;

static long centre_station(int8_t pos) {
    return (long)(pos + 1) * STUB_DUREE_SEGMENT;
}

static Couleur couleur_en(long position) {
    for (int8_t pos = 0; pos < NB_STATIONS; pos++) {
        if (labs(position - centre_station(pos)) <= (long)STUB_LARGEUR_STATION / 2) return COULEUR_STATION(pos);
    }
    return COULEUR_AUCUNE;
}

void couleur_init() {
    x = centre_station(POSITION_DEPART);
    sens_stub = SENS_AVANT;
    t_dernier = millis();
}

void couleur_stub_sens(Sens sens) {
    sens_stub = sens;
}

Couleur couleur_lire() {
    unsigned long maintenant = millis();
    long dt = maintenant - t_dernier;
    t_dernier = maintenant;
    if (dt < (long)PAUSE_MAX) x += (sens_stub == SENS_AVANT) ? dt : -dt;
    x = constrain(x, 0L, X_MAX);
    return couleur_en(x);
}

bool couleur_mesurer(MesureCouleur& m) {
    return false;
}

#else
// =============================================================================
// 0. Au demarrage, mesure la surface sous le robot (le plancher) : reference.
// 1. Lecture brute R, G, B, divisee par la reference (plancher = 1.0).
// 2. Rapport rouge / bleu : ROUGE si le rouge depasse le bleu de RAPPORT_MIN,
//    BLEU si c'est l'inverse. Le plancher donne 1.0 : AUCUNE.
// 3. Anti-rebond : LECTURES_ANTIREBOND lectures identiques avant de changer.
// =============================================================================
#include <Adafruit_TCS34725.h>
#include <Wire.h>

static Adafruit_TCS34725 capteur(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_60X);

static float ref_r = 1, ref_g = 1, ref_b = 1;  // plancher, mesure par couleur_init()

static Couleur candidate = COULEUR_AUCUNE;  // derniere couleur classee
static uint8_t nb_pareilles = 0;           // lectures identiques de suite
static Couleur stable = COULEUR_AUCUNE;     // couleur retournee

static Couleur classer(const MesureCouleur& m) {
    if (max(m.rouge, max(m.vert, m.bleu)) < LUMINOSITE_MIN) return COULEUR_AUCUNE;
    if (m.rouge >= RAPPORT_MIN * m.bleu) return COULEUR_ROUGE;
    if (m.bleu >= RAPPORT_MIN * m.rouge) return COULEUR_BLEU;
    return COULEUR_AUCUNE;  // blanc, gris, ou couleur pas assez franche
}

// Anti-rebond : une couleur doit etre vue plusieurs fois de suite.
static Couleur filtrer(Couleur c) {
    if (c == candidate) {
        if (nb_pareilles < LECTURES_ANTIREBOND) nb_pareilles++;
    } else {
        candidate = c;
        nb_pareilles = 1;
    }
    if (nb_pareilles >= LECTURES_ANTIREBOND) stable = candidate;
    return stable;
}

static void mesurer_reference() {
    for (uint8_t i = 0; i < 3; i++) {
        uint16_t mr, mg, mb, mc;
        capteur.getRawData(&mr, &mg, &mb, &mc);
    }
    float r = 0, g = 0, b = 0;
    for (uint8_t i = 0; i < NB_MESURES_REFERENCE; i++) {
        uint16_t mr, mg, mb, mc;
        capteur.getRawData(&mr, &mg, &mb, &mc);
        r += mr;
        g += mg;
        b += mb;
    }
    ref_r = max(r / NB_MESURES_REFERENCE, 1.0f);
    ref_g = max(g / NB_MESURES_REFERENCE, 1.0f);
    ref_b = max(b / NB_MESURES_REFERENCE, 1.0f);
    LOG(String("Reference plancher RGB ") + (int)ref_r + " " + (int)ref_g + " " + (int)ref_b);
}

void couleur_init() {
    bool trouve = false;
    for (uint8_t essai = 0; essai < 5 && !trouve; essai++) {
        trouve = capteur.begin();
        if (!trouve) delay(50);
    }
    if (trouve) {
        mesurer_reference();
    } else {
        LOG("ERREUR : capteur de couleur introuvable (I2C) : verifier SDA 20, SCL 21, 3.3V/5V, GND");
    }
    candidate = COULEUR_AUCUNE;
    nb_pareilles = 0;
    stable = COULEUR_AUCUNE;
}

bool couleur_mesurer(MesureCouleur& m) {
    uint16_t c;
    capteur.getRawData(&m.r, &m.g, &m.b, &c);
    m.rouge = m.r / ref_r;
    m.vert = m.g / ref_g;
    m.bleu = m.b / ref_b;
    return c > 0;
}

Couleur couleur_lire() {
    MesureCouleur m;
    Couleur c = couleur_mesurer(m) ? classer(m) : COULEUR_AUCUNE;
    return filtrer(c);
}
#endif
