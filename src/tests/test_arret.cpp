// test_arret.cpp - Programme de test du module arret_robot
// N'utilise que l'interface publique (arret_robot.h), sans capteur ni moteur.
//
// 1. Rejoue la recette de Patrick (2 boulettes, 2 tomates) : le robot passe
//    par chaque station, en avancant ou en reculant, et compte sa position.
// 2. Simule une station manquee : la position doit etre corrigee.
// 3. Simule un arret un peu apres le ruban, puis une marche arriere : le robot
//    repasse sur le meme ruban, qui ne doit pas compter.

#include <LibRobus.h>

#include "arret_robot.h"
#include "config.h"


// Recette de Patrick, ordre de prise (du bas vers le haut)
static const Ingredient RECETTE[] = {ING_PAIN_SOUS, ING_TOMATE, ING_VIANDE, ING_TOMATE,
                                     ING_VIANDE,    ING_FROMAGE, ING_PAIN};
static const uint8_t NB = sizeof(RECETTE) / sizeof(RECETTE[0]);

// Le robot quitte sa station (ligne) puis voit la couleur de la station suivante.
static bool passer_une_station(Sens sens) {
    int8_t suivante = arret_position() + (sens == SENS_AVANT ? 1 : -1);
    arret_nouvelle_station(COULEUR_AUCUNE, sens);
    return arret_nouvelle_station(COULEUR_STATION(suivante), sens);
}

// Roule station par station tant que arret_decider() dit d'avancer ou de reculer.
static ActionStation rouler(Ingredient ing) {
    ActionStation a = arret_decider(ing);
    while (a == ACTION_AVANCER || a == ACTION_RECULER) {
        passer_une_station(a == ACTION_AVANCER ? SENS_AVANT : SENS_ARRIERE);
        a = arret_decider(ing);
    }
    return a;
}

static void tester_recette() {
    LOG("== Recette de Patrick ==");
    arret_init();
    for (uint8_t i = 0; i <= NB; i++) {
        Ingredient ing = (i < NB) ? RECETTE[i] : ING_AUCUN;
        LOG_VAL("depuis la position", arret_position());
        LOG_VAL("  decision", arret_nom_action(arret_decider(ing)));
        ActionStation a = rouler(ing);
        LOG_VAL("  arrive a", arret_position());
        LOG_VAL("  action", arret_nom_action(a));
    }
    // Attendu : positions 0, 4, 1, 4, 1, 2, 0 (PRENDRE), puis 7 (LIVRER)
}

static void tester_station_manquee() {
    LOG("== Station manquee ==");
    arret_init();
    passer_une_station(SENS_AVANT);  // position 0, ROUGE
    // Le capteur rate la station 1 (BLEU) et voit la 2 (ROUGE) :
    arret_nouvelle_station(COULEUR_AUCUNE, SENS_AVANT);
    arret_nouvelle_station(COULEUR_STATION(2), SENS_AVANT);
    LOG_VAL("position (attendu 2)", arret_position());
}

// Lectures vues par le robot apres son arret sur le ROUGE 4, en reculant.
static void reculer_depuis_4(const Couleur* lu, uint8_t nb, const char* cas) {
    arret_init();
    for (uint8_t i = 0; i < 5; i++) passer_une_station(SENS_AVANT);  // position 4, ROUGE
    for (uint8_t i = 0; i < nb; i++) arret_nouvelle_station(lu[i], SENS_ARRIERE);
    LOG_VAL(cas, arret_position());
}

static void tester_depassement() {
    LOG("== Arret sur ou apres le ruban, puis recul ==");
    // Arrete APRES le ruban : 1re lecture encore ROUGE (anti-rebond), puis il
    // repasse sur le ROUGE 4 avant d'arriver au BLEU 3 (log du 25 nov.).
    const Couleur apres[] = {COULEUR_ROUGE, COULEUR_AUCUNE, COULEUR_ROUGE, COULEUR_AUCUNE, COULEUR_BLEU};
    reculer_depuis_4(apres, 5, "apres le ruban (attendu 3)");
    // Arrete SUR le ruban : il le quitte, puis arrive au BLEU 3.
    const Couleur dessus[] = {COULEUR_ROUGE, COULEUR_AUCUNE, COULEUR_BLEU};
    reculer_depuis_4(dessus, 3, "sur le ruban (attendu 3)");
}

void setup() {
    BoardInit();
}

void loop() {
    tester_recette();
    tester_station_manquee();
    tester_depassement();
    LOG("-----");
    delay(10000);
}
